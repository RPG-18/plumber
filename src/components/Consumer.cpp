#include "spdlog/spdlog.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#include "Consumer.h"
#include "KafkaConsumer.h"

Consumer::Consumer(QObject *parent)
    : QObject(parent)
    , m_isRunning(false)
    , m_argsChanged(false)
    , m_typeSelector(nullptr)
    , m_limiter(nullptr)
    , m_filter(nullptr)
    , m_beginning(nullptr)
    , m_consumer(nullptr)
    , m_messageModel(new MessageModel(this))
{}

Consumer::~Consumer()
{
    if (m_consumerThread.isRunning()) {
        m_consumerThread.quit();
        m_consumerThread.wait();
    }
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
    if (m_consumer != nullptr && m_argsChanged) {
        for (auto &conn : m_connections) {
            QObject::disconnect(conn);
        }
        m_connections.clear();
        m_argsChanged = false;
        m_consumer->deleteLater();
        m_consumer = nullptr;
    }

    if (m_consumer == nullptr) {
        QStringList topics = {m_topic};
        if (!m_topics.isEmpty()) {
            topics = m_topics;
        }
        m_consumer = new core::KafkaConsumer(m_broker, topics, nullptr);
        setStartOnTime();
        setFilter();
        setLimit();

        m_connections.push_back(
            connect(&m_consumerThread, &QThread::finished, m_consumer, &QObject::deleteLater));
        m_connections.push_back(
            connect(m_consumer, &core::KafkaConsumer::received, this, &Consumer::onReceived));
        m_connections.push_back(
            connect(m_consumer, &core::KafkaConsumer::stopped, this, &Consumer::onStopped));

        m_consumer->moveToThread(&m_consumerThread);
        m_consumerThread.start();
    }
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    QMetaObject::invokeMethod(m_consumer, &core::KafkaConsumer::start, Qt::QueuedConnection);
    setRunning(true);
}

void Consumer::stop()
{
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    QMetaObject::invokeMethod(m_consumer, &core::KafkaConsumer::stop, Qt::QueuedConnection);
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
    if (m_consumer == nullptr) {
        return;
    }

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
    switch (m_beginning->startFrom()) {
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
        const auto tp = m_beginning->specificDateTimePoint();
        if (!tp.isValid()) {
            spdlog::error("failed parse time {}", m_beginning->specificDate().toStdString());
            return;
        }
        dt = tp;
    } break;
    case Earliest: {
        m_consumer->seekToBeginning();
    } break;
    default:
        qDebug() << "Unknown value" << m_beginning->startFrom();
    }

    if (!dt.isNull()) {
        m_consumer->seekOnTimestamp(dt);
    }
}

void Consumer::setLimit()
{
    if (m_limiter == nullptr) {
        spdlog::error("Limiter not set");
        return;
    }

    std::unique_ptr<core::AbstractLimiter> limiter;
    switch (m_limiter->limit()) {
    case Limit::NoneLimit:
        return;
    case Limit::NumbersOfRecords: {
        limiter = std::make_unique<core::NumberOfRecordsLimiter>(m_limiter->numberOfRecords());
    } break;
    case Limit::MaxSizeBytes: {
        limiter = std::make_unique<core::MaxSizeLimiter>(m_limiter->maxSize());
    } break;
    case Limit::SpecificDateLimit: {
        const auto tp = m_limiter->specificDateTimePoint();
        limiter = std::make_unique<core::DateLimiter>(tp);
    } break;
    default: {
        qWarning() << "Unknown limit" << m_limiter->limit();
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
    if (m_limiter == nullptr) {
        spdlog::error("Filter not set");
        return;
    }

    std::unique_ptr<core::AbstractFilter> filter;
    switch (m_filter->filters()) {
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

    if (!m_filter->key().isEmpty()) {
        filter->setKey(toBytes(m_typeSelector->keyType(), m_filter->key()));
    }

    if (!m_filter->value().isEmpty()) {
        filter->setValue(toBytes(m_typeSelector->valueType(), m_filter->value()));
    }

    if (!m_filter->headerKey().isEmpty() || !m_filter->headerValue().isEmpty()) {
        filter->setHeader(core::Header{m_filter->headerKey(), m_filter->headerValue()});
    }

    if (!filter->isEmpty()) {
        m_consumer->setFilter(std::move(filter));
    }
}

void Consumer::setTypeSelector(ConsumerTypeSelector *selector)
{
    if (m_typeSelector != nullptr) {
        return;
    }
    m_typeSelector = selector;
    QObject::connect(m_typeSelector,
                     &ConsumerTypeSelector::typesChanged,
                     this,
                     &Consumer::onKeyValueChanged);
}

ConsumerTypeSelector *Consumer::typeSelector()
{
    return m_typeSelector;
}

void Consumer::onKeyValueChanged()
{
    if (m_typeSelector) {
        m_messageModel->setKeyType(m_typeSelector->keyType());
        m_messageModel->setValueType(m_typeSelector->valueType());
    }
    m_argsChanged = true;
}

ConsumerLimiterSelector *Consumer::limiter()
{
    return m_limiter;
}

void Consumer::setLimiter(ConsumerLimiterSelector *limiter)
{
    if (m_limiter != nullptr) {
        return;
    }

    m_limiter = limiter;
    QObject::connect(m_limiter,
                     &ConsumerLimiterSelector::limitChanged,
                     this,
                     &Consumer::onLimiterChanged);
}

void Consumer::onLimiterChanged()
{
    m_argsChanged = true;
}

ConsumerFilterSelector *Consumer::filter()
{
    return m_filter;
}

void Consumer::setFilter(ConsumerFilterSelector *filter)
{
    if (m_filter != nullptr) {
        return;
    }

    m_filter = filter;
    QObject::connect(m_filter,
                     &ConsumerFilterSelector::filterChanged,
                     this,
                     &Consumer::onFilterChanged);
}

void Consumer::onFilterChanged()
{
    m_argsChanged = true;
}

ConsumerBeginningSelector *Consumer::beginning()
{
    return m_beginning;
}

void Consumer::setBeginning(ConsumerBeginningSelector *beginning)
{
    if (m_beginning != nullptr) {
        return;
    }

    m_beginning = beginning;
    QObject::connect(m_beginning,
                     &ConsumerBeginningSelector::beginningChanged,
                     this,
                     &Consumer::onBeginningChanged);
}

void Consumer::onBeginningChanged()
{
    m_argsChanged = true;
}

ConsumerTypeSelector::ConsumerTypeSelector(QObject *parent)
    : QObject(parent)
    , m_keyType(Types::String)
    , m_valueType(Types::String)
{}

Types ConsumerTypeSelector::keyType() const
{
    return m_keyType;
}

void ConsumerTypeSelector::setKeyType(Types type)
{
    m_keyType = type;
    emit typesChanged();
}

Types ConsumerTypeSelector::valueType() const
{
    return m_valueType;
}

void ConsumerTypeSelector::setValueType(Types type)
{
    m_valueType = type;
    emit typesChanged();
}

ConsumerLimiterSelector::ConsumerLimiterSelector(QObject *parent)
    : QObject(parent)
    , m_limit(Consumer::Limit::NoneLimit)
    , m_numberOfRecords(10)
    , m_maxSize(1024)
{}

Consumer::Limit ConsumerLimiterSelector::limit() const noexcept
{
    return m_limit;
}

void ConsumerLimiterSelector::setLimit(Consumer::Limit limit) noexcept
{
    m_limit = limit;
    emit limitChanged();
}

int ConsumerLimiterSelector::maxSize() const noexcept
{
    return m_maxSize;
}

void ConsumerLimiterSelector::setMaxSize(int size) noexcept
{
    m_maxSize = size;
    emit limitChanged();
}

int ConsumerLimiterSelector::numberOfRecords() const noexcept
{
    return m_numberOfRecords;
}

void ConsumerLimiterSelector::setNumberOfRecords(int records) noexcept
{
    m_numberOfRecords = records;
    emit limitChanged();
}

const QString &ConsumerLimiterSelector::specificDate() const
{
    return m_specificDate;
}

QDateTime ConsumerLimiterSelector::specificDateTimePoint() const
{
    return QDateTime::fromString(m_specificDate, "yyyy-MM-dd hh:mm:ss");
}

void ConsumerLimiterSelector::setSpecificDate(const QString &limit)
{
    m_specificDate = limit;
    emit limitChanged();
}

ConsumerFilterSelector::ConsumerFilterSelector(QObject *parent)
    : QObject(parent)
    , m_filters(Consumer::NoneFilter)
{}

Consumer::Filters ConsumerFilterSelector::filters() const noexcept
{
    return m_filters;
}

void ConsumerFilterSelector::setFilters(Consumer::Filters filters) noexcept
{
    m_filters = filters;
    emit filterChanged();
}

const QString &ConsumerFilterSelector::key() const
{
    return m_key;
}

void ConsumerFilterSelector::setKey(const QString &key)
{
    m_key = key;
    emit filterChanged();
}

const QString &ConsumerFilterSelector::value() const
{
    return m_value;
}

void ConsumerFilterSelector::setValue(const QString &value)
{
    m_value = value;
    emit filterChanged();
}

const QString &ConsumerFilterSelector::headerKey() const
{
    return m_headerKey;
}

void ConsumerFilterSelector::setHeaderKey(const QString &key)
{
    m_headerKey = key;
    emit filterChanged();
}

const QString &ConsumerFilterSelector::headerValue() const
{
    return m_headerValue;
}

void ConsumerFilterSelector::setHeaderValue(const QString &value)
{
    m_headerValue = value;
    emit filterChanged();
}

ConsumerBeginningSelector::ConsumerBeginningSelector(QObject *parent)
    : QObject(parent)
    , m_startFromTime(Consumer::StartFromTimeBased::Now)
{}

const QString &ConsumerBeginningSelector::specificDate() const
{
    return m_specificDate;
}

void ConsumerBeginningSelector::setSpecificDate(const QString &date)
{
    m_specificDate = date;
    emit beginningChanged();
}

Consumer::StartFromTimeBased ConsumerBeginningSelector::startFrom() const
{
    return m_startFromTime;
}

void ConsumerBeginningSelector::setStartFrom(Consumer::StartFromTimeBased from)
{
    m_startFromTime = from;
    emit beginningChanged();
}

QDateTime ConsumerBeginningSelector::specificDateTimePoint() const
{
    return QDateTime::fromString(m_specificDate, "yyyy-MM-dd hh:mm:ss");
}
