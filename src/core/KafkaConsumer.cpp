#include <boost/circular_buffer.hpp>

#include <QtCore/QDebug>
#include <QtCore/QMutexLocker>
#include <QtCore/QTimerEvent>

#include "KafkaConsumer.h"
#include "spdlog/spdlog.h"
#include "utils/KafkaUtility.h"

namespace core {
KafkaConsumer::KafkaConsumer(ClusterConfig cfg, const QStringList &topics, QObject *parent)
    : QObject(parent)
    , m_timerId(0)
    , m_cfg(std::move(cfg))
    , m_topics(topics)
    , m_toBeginning(false)
    , m_limiter(new AbstractLimiter)
    , m_buff(MaxMessages)
{
    for (const auto &topic : topics) {
        m_topicMapper.insert(topic.toStdString(), topic);
    }
}

void KafkaConsumer::start()
{
    if (m_timerId == 0) {
        m_timerId = startTimer(PoolInterval);
    }
    if (m_consumer != nullptr) {
        m_consumer->resume();
    }
}

void KafkaConsumer::stop()
{
    if (m_timerId != 0) {
        killTimer(m_timerId);
        m_timerId = 0;
        if (m_consumer != nullptr) {
            m_consumer->pause();
        }
    }
}

void KafkaConsumer::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != m_timerId) {
        return;
    }
    pool();
}

void KafkaConsumer::pool()
{
    if (m_consumer == nullptr) {
        createConsumer();
    }

    try {
        auto records = m_consumer->poll(PoolTimeout);
        uint64_t messages = 0;
        uint64_t bytes = 0;

        for (const auto &record : records) {
            ++messages;
            auto rec = std::make_unique<ConsumerRecord>();
            rec->topic = m_topicMapper[record.topic()];
            rec->partition = record.partition();
            rec->offset = record.offset();
            rec->key.append(static_cast<const char *>(record.key().data()),
                            qsizetype(record.key().size()));
            rec->value.append(static_cast<const char *>(record.value().data()),
                              qsizetype(record.value().size()));
            rec->timestamp = record.timestamp();
            const auto headers = record.headers();
            rec->headers.reserve(qsizetype(headers.size()));
            for (const auto &header : headers) {
                rec->headers.push_back(
                    Header{QString::fromStdString(header.key),
                           QByteArray(static_cast<const char *>(header.value.data()),
                                      qsizetype(header.value.size()))});
            }

            if (m_keyConverter) {
                rec->key = m_keyConverter->toJSON(std::move(rec->key));
            }

            if (m_valueConverter) {
                rec->value = m_valueConverter->toJSON(std::move(rec->value));
            }

            bytes += rec->key.size() + rec->value.size();
            if (m_filter != nullptr && !m_filter->IsAcceptable(rec)) {
                continue;
            }

            if (!m_limiter->ExcessOfLimit(rec)) {
                if (m_recorder) {
                    m_recorder->writeRecord(rec.get());
                }

                m_buff.push(std::move(rec));
            } else {
                manualStop();
                break;
            }
        }

        updateStat(messages, bytes);
    } catch (const kafka::KafkaException &e) {
        spdlog::error("Unexpected exception caught: {}", e.what());
    }
}

void KafkaConsumer::createConsumer()
{
    using namespace kafka::clients;
    try {
        consumer::ConsumerConfig props(m_cfg.properties->map());
        props.put(consumer::ConsumerConfig::LOG_CB, KafkaSpdLogger);
        props.put(consumer::ConsumerConfig::BOOTSTRAP_SERVERS, m_cfg.bootstrap.toStdString());

        auto consumer = std::make_unique<consumer::KafkaConsumer>(props);
        m_consumer = std::move(consumer);
        kafka::Topics topics;
        for (const auto &topic : m_topics) {
            topics.insert(topic.toStdString());
        }
        m_consumer->subscribe(topics);
        setOnTimeOffset();
    } catch (const kafka::KafkaException &e) {
        spdlog::error("Unexpected exception caught: {}", e.what());
    }
}

void KafkaConsumer::records(ConsumerRecords &out)
{
    auto data = m_buff.records();
    out.swap(data);
}

void KafkaConsumer::seekOnTimestamp(const QDateTime &tm)
{
    m_seekOnTime = tm;
}

void KafkaConsumer::setOnTimeOffset()
{
    if (m_toBeginning) {
        m_consumer->seekToBeginning();
        return;
    }

    using namespace std::chrono;

    if (!m_seekOnTime.isValid()) {
        return;
    }

    const auto partitions = m_consumer->assignment();
    const auto unixTimeStamp = milliseconds(m_seekOnTime.toMSecsSinceEpoch());
    const auto tp = time_point<system_clock, milliseconds>(unixTimeStamp);

    const auto offsets = m_consumer->offsetsForTime(partitions, tp);
    for (const auto &[topic, offset] : offsets) {
        spdlog::info("topic:{} part:{} offset: {}", topic.first, topic.second, offset);
        m_consumer->seek(topic, offset);
    }
}

void KafkaConsumer::seekToBeginning()
{
    m_toBeginning = true;
}

void KafkaConsumer::setLimiter(std::unique_ptr<core::AbstractLimiter> limiter)
{
    m_limiter = std::move(limiter);
}

void KafkaConsumer::setFilter(std::unique_ptr<core::AbstractFilter> filter)
{
    m_filter = std::move(filter);
}

void KafkaConsumer::setRecorder(std::unique_ptr<core::AbstractConsumerExporter> recorder)
{
    m_recorder = std::move(recorder);
}

void KafkaConsumer::manualStop()
{
    stop();
    emit stopped();
}

using Lock = std::lock_guard<std::mutex>;

void KafkaConsumer::updateStat(uint64_t messages, uint64_t bytes)
{
    Lock l(m_statMu);
    m_stat.messages += messages;
    m_stat.bytes += bytes;
}

KafkaConsumer::ConsumeStat KafkaConsumer::stat()
{
    Lock l(m_statMu);
    return m_stat;
}

void KafkaConsumer::setKeyConverter(ConverterPtr &&key)
{
    m_keyConverter = std::move(key);
}

void KafkaConsumer::setValueConverter(ConverterPtr &&value)
{
    m_valueConverter = std::move(value);
}

} // namespace core
