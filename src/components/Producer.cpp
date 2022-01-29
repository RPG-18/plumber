#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#include "spdlog/spdlog.h"

#include "AbstractConverter.h"
#include "KafkaProducer.h"
#include "Producer.h"
#include "ProtoOption.h"
#include "Types.h"

Producer::Producer(QObject *parent)
    : QObject(parent)
    , m_producer(nullptr)
    , m_options(nullptr)
    , m_keyType(String)
    , m_valueType(String)
    , m_logModel(new ProducerLogModel(this))
    , m_keyProto(nullptr)
    , m_valueProto(nullptr)
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
    if (m_producer == nullptr) {
        createProducer();
    }

    QByteArray keyData;
    if (m_keyType == Protobuf) {
        if (m_keyConverter == nullptr) {
            auto [converter, err] = m_keyProto->converter();
            if (converter == nullptr) {
                return err;
            }
            m_keyConverter.swap(converter);
        }
        keyData = m_keyConverter->fromJSON(key.toUtf8());
    } else {
        keyData = bytes(m_keyType, key);
    }

    QByteArray valueData;
    if (m_valueType == Protobuf) {
        if (m_valueConverter == nullptr) {
            auto [converter, err] = m_valueProto->converter();
            if (converter == nullptr) {
                return err;
            }
            m_valueConverter.swap(converter);
        }
        valueData = m_valueConverter->fromJSON(value.toUtf8());
    } else {
        valueData = bytes(m_valueType, value);
    }

    core::ProducerRecord rec{m_topic, keyData, valueData};
    if (auto meta = m_producer->send(rec)) {
        m_logModel->append(std::move(*meta));
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

ProducerLogModel *Producer::log() noexcept
{
    return m_logModel;
}

ProtoOption *Producer::protoKey()
{
    return m_keyProto;
}

void Producer::setProtoKey(ProtoOption *option)
{
    if (m_keyProto != nullptr) {
        m_keyProto->disconnect();
        resetKeyConverter();
    }
    m_keyProto = option;
    connect(m_keyProto, &ProtoOption::changed, this, &Producer::resetKeyConverter);
}

ProtoOption *Producer::protoValue()
{
    return m_valueProto;
}

void Producer::setProtoValue(ProtoOption *option)
{
    if (m_valueProto != nullptr) {
        m_valueProto->disconnect();
        resetValueConverter();
    }

    m_valueProto = option;
    connect(m_valueProto, &ProtoOption::changed, this, &Producer::resetValueConverter);
}

void Producer::resetKeyConverter()
{
    m_keyConverter.release();
}

void Producer::resetValueConverter()
{
    m_valueConverter.release();
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

ProducerLogModel::ProducerLogModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_isEmpty(true)
{
    m_meta.reserve(Reserve);
}

int ProducerLogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return int(m_meta.size());
}

QVariant ProducerLogModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::DisplayRole) {
        return {};
    }
    if (index.row() >= m_meta.size()) {
        return {};
    }

    const auto &meta = m_meta[index.row()];
    switch (role) {
    case Topic:
        return meta.topic;
    case Partition:
        return meta.partition;
    case Offset:
        return qint64(meta.offset);
    case Timestamp: {
        const auto msgTime = QDateTime::fromMSecsSinceEpoch(meta.timestamp.msSinceEpoch);
        return msgTime.toString("yyyy-MM-dd hh:mm:ss");
    }
    default:
        return {};
    }
}

QHash<int, QByteArray> ProducerLogModel::roleNames() const
{
    static QHash<int, QByteArray> roles{{Topic, "topic"},
                                        {Partition, "partition"},
                                        {Offset, "offset"},
                                        {Timestamp, "timestamp"}};
    return roles;
}

void ProducerLogModel::append(core::ProducerMetadata &&metadata)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_meta.push_front(metadata);
    endInsertRows();
    updateEmptyFlag();
}

void ProducerLogModel::updateEmptyFlag()
{
    const auto old = m_isEmpty;
    m_isEmpty = m_meta.isEmpty();
    if (old != m_isEmpty) {
        emit isEmptyChanged();
    }
}
