#include <spdlog/spdlog.h>
#include <QtCore/QDebug>
#include <QtCore/QMutexLocker>
#include <QtCore/QTimerEvent>

#include "KafkaConsumer.h"

namespace core {

KafkaConsumer::KafkaConsumer(ClusterConfig cfg, QStringList topics, QObject *parent)
    : QObject(parent)
    , m_timerId(0)
    , m_cfg(cfg)
    , m_topics(topics)
    , m_toBeginning(false)
    , m_limiter(new AbstractLimiter)
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
        ConsumerRecords received;
        received.reserve(records.size());
        for (const auto &record : records) {
            auto rec = std::make_unique<ConsumerRecord>();
            rec->topic = m_topicMapper[record.topic()];
            rec->partition = record.partition();
            rec->offset = record.offset();
            rec->key.append(static_cast<const char *>(record.key().data()), record.key().size());
            rec->value.append(static_cast<const char *>(record.value().data()),
                              record.value().size());
            rec->timestamp = record.timestamp();
            const auto headers = record.headers();
            rec->headers.reserve(headers.size());
            for (const auto &header : headers) {
                rec->headers.push_back(
                    Header{QString::fromStdString(header.key),
                           QByteArray(static_cast<const char *>(header.value.data()),
                                      header.value.size())});
            }

            if (m_filter != nullptr && !m_filter->IsAcceptable(rec.get())) {
                continue;
            }

            if (!m_limiter->ExcessOfLimit(rec.get())) {
                received.push_back(rec.release());
            } else {
                manualStop();
                break;
            }
        }

        append(received);
    } catch (const kafka::KafkaException &e) {
        spdlog::error("Unexpected exception caught: {}", e.what());
    }
}

void KafkaConsumer::createConsumer()
{
    qDebug() << m_cfg.bootstrap;
    qDebug() << m_topics;

    try {
        kafka::ConsumerConfig props(m_cfg.properties->map());
        props.put(kafka::ConsumerConfig::BOOTSTRAP_SERVERS, m_cfg.bootstrap.toStdString());

        auto consumer = std::make_unique<kafka::KafkaManualCommitConsumer>(props);
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
    QMutexLocker locker(&m_mutex);
    m_records.swap(out);
}

void KafkaConsumer::append(ConsumerRecords &records)
{
    QMutexLocker locker(&m_mutex);

    if (m_records.empty()) {
        m_records.swap(records);
        emit received();
        return;
    }

    m_records.append(records);
    emit received();
}

void KafkaConsumer::seekOnTimestamp(const QDateTime tm)
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

void KafkaConsumer::manualStop()
{
    stop();
    emit stopped();
}
} // namespace core
