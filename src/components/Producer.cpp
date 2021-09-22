#include <QtCore/QDebug>

#include "KafkaProducer.h"
#include "Producer.h"
#include "Types.h"
#include "spdlog/spdlog.h"

Producer::Producer(QObject *parent)
    : QObject(parent)
    , m_producer(nullptr)
    , m_options(nullptr)
    , m_keyType(String)
    , m_valueType(String)
{}

ProducerOptions *Producer::options()
{
    return m_options;
}

void Producer::setOptions(ProducerOptions *option)
{
    m_options = option;
    QObject::connect(m_options, &ProducerOptions::changed, this, &Producer::onOptionsChanged);
}

ErrorWrap Producer::send(const QString &key, const QString &value)
{
    qDebug() << "Send" << m_topic << m_keyType << key << m_valueType << value << m_options->ack()
             << m_options->compression() << "idempotence" << m_options->idempotence();
    if (m_producer == nullptr) {
        createProducer();
    }

    auto keyData = bytes(m_keyType, key);
    auto valueData = bytes(m_valueType, value);

    core::ProducerRecord rec{m_topic, keyData, valueData};
    if (auto meta = m_producer->send(rec)) {
        auto md = *meta;
        qDebug() << md.topic << md.offset;
        return ErrorWrap{};
    }
    return ErrorWrap("producer", m_producer->lastError());
}

void Producer::onOptionsChanged()
{
    if (m_producer == nullptr) {
        return;
    }
    m_producer->deleteLater();
    m_producer = nullptr;
}

void Producer::createProducer()
{
    auto cfg = m_broker;
    if (m_options->idempotence()) {
        cfg.properties->put("enable.idempotence ", "true");
        cfg.properties->put("request.required.acks", "all");
    } else {
        static const std::vector<std::string> values = {"0", "1", "-1"};
        const auto indx = static_cast<size_t>(m_options->ack());
        if (indx < values.size()) {
            cfg.properties->put("request.required.acks", values[indx]);
        } else {
            spdlog::error("unexpected ack {}", m_options->ack());
        }
    }

    {
        static const std::vector<std::string> values = {"none", "gzip", "snappy", "lz4", "zstd"};
        const auto indx = static_cast<size_t>(m_options->compression());
        if (indx < values.size()) {
            cfg.properties->put("compression.codec", values[indx]);
        } else {
            spdlog::error("unexpected compression {}", m_options->compression());
        }
    }

    m_producer = new core::KafkaProducer(cfg, this);
}

ClusterConfig Producer::broker() const
{
    return m_broker;
}

void Producer::setBroker(const ClusterConfig &broker)
{
    m_broker = broker;
    emit brokerChanged();
}

ProducerOptions::ProducerOptions(QObject *parent)
    : QObject(parent)
    , m_compression(Producer::Compression::NoneCompression)
    , m_ack(Producer::Ack::NoAck)
    , m_idempotence(false)
{}

Producer::Compression ProducerOptions::compression() const noexcept
{
    return m_compression;
}

void ProducerOptions::setCompression(Producer::Compression compression) noexcept
{
    m_compression = compression;
    emit changed();
}

Producer::Ack ProducerOptions::ack() const noexcept
{
    return m_ack;
}

void ProducerOptions::setAck(Producer::Ack ack)
{
    m_ack = ack;
    emit changed();
}

bool ProducerOptions::idempotence() const noexcept
{
    return m_idempotence;
}

void ProducerOptions::setIdempotence(bool value) noexcept
{
    m_idempotence = value;
    emit changed();
}