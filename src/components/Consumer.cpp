#include "spdlog/spdlog.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#include "Consumer.h"
#include "KafkaConsumer.h"

Consumer::Consumer(QObject *parent)
    : QObject(parent)
    , m_isRunning(false)
    , m_keyType(Types::String)
    , m_valueType(Types::String)
    , m_startFromTime(StartFromTimeBased::Now)
    , m_filters(NoneFilter)
    , m_consumer(nullptr)
    , m_limit(Limit::NoneLimit)
    , m_numberOfRecords(10)
    , m_maxSize(1024)
    , m_messageModel(new MessageModel(this))
{}

Consumer::~Consumer()
{
    if (m_consumerThread.isRunning()) {
        m_consumerThread.quit();
        m_consumerThread.wait();
    }
}

Types Consumer::keyType() const
{
    return m_keyType;
}
void Consumer::setKeyType(Types type)
{
    m_keyType = type;
    m_messageModel->setKeyType(m_keyType);
}

Types Consumer::valueType() const
{
    return m_valueType;
}

void Consumer::setValueType(Types type)
{
    m_valueType = type;
    m_messageModel->setValueType(m_valueType);
}

Consumer::Filters Consumer::filters() const
{
    return m_filters;
}

void Consumer::setFilters(Filters filter)
{
    qDebug() << filter;
    m_filters = filter;
}

MessageModel *Consumer::messageModel() const
{
    return m_messageModel;
}

bool Consumer::isRunning() const
{
    return m_isRunning;
}

void Consumer::setRunning(bool val)
{
    m_isRunning = val;
    emit isRunningChanged();
}

void Consumer::start()
{
    if (m_consumer == nullptr) {
        m_consumer = new core::KafkaConsumer(m_broker, {m_topic}, nullptr);
        setStartOnTime();
        setFilter();
        setLimit();

        connect(&m_consumerThread, &QThread::finished, m_consumer, &QObject::deleteLater);
        connect(m_consumer, &core::KafkaConsumer::received, this, &Consumer::onReceived);
        connect(m_consumer, &core::KafkaConsumer::stopped, this, &Consumer::onStopped);

        m_consumer->moveToThread(&m_consumerThread);
        m_consumerThread.start();
    }
    QMetaObject::invokeMethod(m_consumer, "start", Qt::QueuedConnection);
    setRunning(true);
}

void Consumer::stop()
{
    QMetaObject::invokeMethod(m_consumer, "stop", Qt::QueuedConnection);
    setRunning(false);
}

ClusterConfig Consumer::broker() const
{
    return m_broker;
}

void Consumer::setBroker(const ClusterConfig &broker)
{
    m_broker = broker;
}

void Consumer::onReceived()
{
    core::ConsumerRecords records;
    m_consumer->records(records);
    m_messageModel->append(std::move(records));
}

void Consumer::onStopped()
{
    setRunning(false);
};

void Consumer::setStartOnTime()
{
    const int secInHour = 60 * 60;
    QDateTime dt;
    switch (m_startFromTime) {
    case Now:
        return;
    case LastHour: {
        const auto tp = QDateTime::currentDateTime();
        dt = tp.addSecs(-secInHour);
    } break;
    case Today: {
        const auto date = QDate::currentDate();
        dt.setDate(date);
        dt.setTime(QTime(0, 0, 0, 0));
    } break;
    case Yesterday: {
        auto date = QDate::currentDate();
        date = date.addDays(-1);
        dt.setDate(date);
        dt.setTime(QTime(0, 0, 0, 0));
    } break;
    case SpecificDate: {
        const auto tp = QDateTime::fromString(m_specificDate, "yyyy-MM-dd hh:mm:ss");
        if (!tp.isValid()) {
            spdlog::error("failed parse time {}", m_specificDate.toStdString());
            return;
        }
        dt = tp;
    } break;
    case Earliest: {
        m_consumer->seekToBeginning();
    } break;
    default:
        qDebug() << "Unknown value" << m_startFromTime;
    }

    if (!dt.isNull()) {
        m_consumer->seekOnTimestamp(dt);
    }
}

void Consumer::setLimit()
{
    std::unique_ptr<core::AbstractLimiter> limiter;
    switch (m_limit) {
    case Limit::NoneLimit:
        return;
    case Limit::NumbersOfRecords: {
        limiter = std::make_unique<core::NumberOfRecordsLimiter>(m_numberOfRecords);
    } break;
    case Limit::MaxSizeBytes: {
        limiter = std::make_unique<core::MaxSizeLimiter>(m_maxSize);
    } break;
    case Limit::SpecificDateLimit: {
        const auto tp = QDateTime::fromString(m_limitSpecificDate, "yyyy-MM-dd hh:mm:ss");
        limiter = std::make_unique<core::DateLimiter>(tp);
    } break;
    default: {
        qWarning() << "Unknown limit" << m_limit;
        return;
    }
    }

    m_consumer->setLimiter(std::move(limiter));
}

QByteArray toBytes(Types type, const QString &value)
{
    switch (type) {
    case Types::JSON:
    case Types::String:
        return value.toUtf8();
    case Types::Base64:
        return QByteArray::fromBase64(value.toLatin1());
    case Types::Float: {
        bool ok = false;
        auto val = value.toFloat(&ok);
        if (ok) {
            QByteArray data;
            QDataStream stream(&data, QIODeviceBase::WriteOnly);
            stream.setByteOrder(QDataStream::BigEndian);
            stream << val;
            return data;
        }
        spdlog::error("failed convert string {} to float", value.toStdString());
    } break;
    case Types::Double: {
        bool ok = false;
        auto val = value.toDouble(&ok);
        if (ok) {
            QByteArray data;
            QDataStream stream(&data, QIODeviceBase::WriteOnly);
            stream.setByteOrder(QDataStream::BigEndian);
            stream << val;
            return data;
        }
        spdlog::error("failed convert string {} to float", value.toStdString());
    } break;
    case Types::Long: {
        bool ok = false;
        auto val = value.toLongLong(&ok);
        if (ok) {
            QByteArray data;
            QDataStream stream(&data, QIODeviceBase::WriteOnly);
            stream.setByteOrder(QDataStream::BigEndian);
            stream << val;
            return data;
        }
        spdlog::error("failed convert string {} to long", value.toStdString());
    } break;
    case Types::NoneType:
        return QByteArray();
    }
    return QByteArray();
}

void Consumer::setFilter()
{
    std::unique_ptr<core::AbstractFilter> filter;
    switch (m_filters) {
    case Filters::Contains: {
        filter = std::make_unique<core::ContainFilter>();
    } break;
    case Filters::NotContains: {
        filter = std::make_unique<core::NotContainFilter>();
    } break;
    case Filters::Equals: {
        filter = std::make_unique<core::EqualFilter>();
    } break;
    default:
        return;
    }

    if (!m_keyFilter.isEmpty()) {
        filter->setKey(toBytes(m_keyType, m_keyFilter));
    }

    if (!m_valueFilter.isEmpty()) {
        filter->setValue(toBytes(m_valueType, m_valueFilter));
    }

    if (!m_headerKeyFilter.isEmpty() || !m_headerValueFilter.isEmpty()) {
        filter->setHeader(core::Header{m_headerKeyFilter, m_headerValueFilter});
    }

    if (!filter->isEmpty()) {
        m_consumer->setFilter(std::move(filter));
    }
}
