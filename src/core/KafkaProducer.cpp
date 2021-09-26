#include "KafkaProducer.h"
#include "spdlog/spdlog.h"

namespace core {
KafkaProducer::KafkaProducer(ClusterConfig cfg, QObject *parent)
    : QObject(parent)
    , m_cfg(std::move(cfg))
{
    createProducer();
}

void KafkaProducer::createProducer()
{
    if (m_producer != nullptr) {
        return;
    }

    try {
        kafka::ProducerConfig props(m_cfg.properties->map());
        props.put(kafka::ProducerConfig::BOOTSTRAP_SERVERS, m_cfg.bootstrap.toStdString());

        auto producer = std::make_unique<kafka::KafkaSyncProducer>(props);
        m_producer = std::move(producer);
    } catch (const kafka::KafkaException &e) {
        spdlog::error("Unexpected exception caught: {}", e.what());
    }
}

std::optional<ProducerMetadata> KafkaProducer::send(const ProducerRecord &record)
{
    resetError();
    try {
        auto key = kafka::NullKey;
        if (!record.key.isEmpty()) {
            key = kafka::Key(record.key.data(), record.key.size());
        }
        auto value = kafka::NullValue;
        if (!record.value.isEmpty()) {
            value = kafka::Value(record.value.data(), record.value.size());
        }
        auto msg = kafka::ProducerRecord(record.topic.toStdString(), key, value);
        const auto meta = m_producer->send(msg);
        ProducerMetadata md{meta.partition(),
                            0,
                            QString::fromStdString(meta.topic()),
                            meta.timestamp()};
        if (auto offset = meta.offset()) {
            md.offset = *offset;
        }
        return md;
    } catch (const kafka::KafkaException &e) {
        m_lastError = QString::fromStdString(e.error().toString());
        spdlog::error("Unexpected exception caught: {}", e.what());
    }

    return std::nullopt;
}

void KafkaProducer::resetError()
{
    m_lastError.clear();
}

const QString &KafkaProducer::lastError()
{
    return m_lastError;
}
} // namespace core