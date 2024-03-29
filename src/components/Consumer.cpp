#include "spdlog/spdlog.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

#include "AbstractConverter.h"
#include "Consumer.h"
#include "ExportImportFabric.h"
#include "KafkaConsumer.h"
#include "KafkaStatistic.h"
#include "Registry.h"

#include "formats/protobuf/ProtobufConverter.h"

Consumer::Consumer(QObject *parent)
    : QObject(parent)
    , m_isRunning(false)
    , m_argsChanged(false)
    , m_typeSelector(nullptr)
    , m_limiter(nullptr)
    , m_filter(nullptr)
    , m_beginning(nullptr)
    , m_consumer(nullptr)
    , m_refresh(new QTimer(this))
    , m_messageModel(new MessageModel(this))
{
    m_refresh->setInterval(RefreshInterval);
    QObject::connect(m_refresh, &QTimer::timeout, this, &Consumer::onReceived);
}

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

ErrorWrap Consumer::start()
{
    auto *statThread = new QThread(this);
    statThread->start();

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
        m_consumer = new core::KafkaConsumer(m_broker, topics);
        setStartOnTime();
        setFilter();
        setLimit();
        {
            auto err = setConverter();
            if (err.isError) {
                return err;
            }
        }

        if (m_recordExporter) {
            m_consumer->setRecorder(std::move(m_recordExporter));
        }

        m_connections.push_back(
            connect(&m_consumerThread, &QThread::finished, m_consumer, &QObject::deleteLater));
        m_connections.push_back(
            connect(m_consumer, &core::KafkaConsumer::stopped, this, &Consumer::onStopped));

        m_consumer->moveToThread(&m_consumerThread);
        m_consumerThread.start();
        m_refresh->start();
    }
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    QMetaObject::invokeMethod(m_consumer, &core::KafkaConsumer::start, Qt::QueuedConnection);
    setRunning(true);
    return ErrorWrap{};
}

void Consumer::stop()
{
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    QMetaObject::invokeMethod(m_consumer, &core::KafkaConsumer::stop, Qt::QueuedConnection);
    m_refresh->stop();
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

    updateStat();
}

void Consumer::updateStat()
{
    m_stat = m_consumer->stat();
    emit statChanged();
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
        filter->setKey(bytes(m_typeSelector->keyType(), m_filter->key()));
    }

    if (!m_filter->value().isEmpty()) {
        filter->setValue(bytes(m_typeSelector->valueType(), m_filter->value()));
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

QString Consumer::stat() const
{
    static const QStringList sizes = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};

    auto bytes = double(m_stat.bytes);
    int i = 0;
    while (bytes > 1024) {
        ++i;
        bytes /= 1024;
    }
    return QString("%1 records consumed %2 %3")
        .arg(m_stat.messages)
        .arg(bytes, 0, 'f', 2)
        .arg(sizes.at(i));
}

ErrorWrap Consumer::setConverter()
{
    if (m_typeSelector == nullptr) {
        return ErrorWrap{};
    }

    m_consumer->setKeyConverter(nullptr);
    m_consumer->setValueConverter(nullptr);
    //todo refactoring
    if (m_typeSelector->keyType() == Protobuf) {
        auto [converter, err] = m_typeSelector->protoKey()->converter();
        if (converter == nullptr) {
            return err;
        }
        m_consumer->setKeyConverter(std::move(converter));
    }

    if (m_typeSelector->valueType() == Protobuf) {
        auto [converter, err] = m_typeSelector->protoValue()->converter();
        if (converter == nullptr) {
            return err;
        }
        m_consumer->setValueConverter(std::move(converter));
    }

    if (m_typeSelector->keyType() == Avro) {
        auto [converter, err] = m_typeSelector->avroKey()->converter();
        if (converter == nullptr) {
            return err;
        }
        m_consumer->setKeyConverter(std::move(converter));
    }

    if (m_typeSelector->valueType() == Avro) {
        auto [converter, err] = m_typeSelector->avroValue()->converter();
        if (converter == nullptr) {
            return err;
        }
        m_consumer->setValueConverter(std::move(converter));
    }

    return ErrorWrap{};
}

ErrorWrap Consumer::exportVisibleRows(ExportImportFabric *fabric)
{
    auto [exporter, err] = fabric->makeExporter(m_typeSelector->keyType(),
                                                m_typeSelector->valueType());
    if (exporter == nullptr) {
        return err;
    }

    m_messageModel->exportMessages(std::move(exporter));
    return ErrorWrap{};
}

ErrorWrap Consumer::exportByRestarting(ExportImportFabric *fabric)
{
    auto [exporter, err] = fabric->makeExporter(m_typeSelector->keyType(),
                                                m_typeSelector->valueType());
    if (exporter == nullptr) {
        return err;
    }

    stop();
    m_recordExporter = std::move(exporter);
    return ErrorWrap{};
}

ConsumerTypeSelector::ConsumerTypeSelector(QObject *parent)
    : QObject(parent)
    , m_keyType(Types::String)
    , m_valueType(Types::String)
    , m_keyProto(nullptr)
    , m_valueProto(nullptr)
    , m_keyAvro(nullptr)
    , m_valueAvro(nullptr)
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

ProtoOption *ConsumerTypeSelector::protoKey()
{
    return m_keyProto;
}

void ConsumerTypeSelector::setProtoKey(ProtoOption *option)
{
    m_keyProto = option;
    emit typesChanged();
    connect(m_keyProto, &ProtoOption::changed, this, &ConsumerTypeSelector::typesChanged);
}

ProtoOption *ConsumerTypeSelector::protoValue()
{
    return m_valueProto;
}

void ConsumerTypeSelector::setProtoValue(ProtoOption *option)
{
    m_valueProto = option;
    emit typesChanged();
    connect(m_valueProto, &ProtoOption::changed, this, &ConsumerTypeSelector::typesChanged);
}

AvroOption *ConsumerTypeSelector::avroKey()
{
    return m_keyAvro;
}
void ConsumerTypeSelector::setAvroKey(AvroOption *option)
{
    m_keyAvro = option;
    emit typesChanged();
    connect(m_keyAvro, &AvroOption::changed, this, &ConsumerTypeSelector::typesChanged);
}

AvroOption *ConsumerTypeSelector::avroValue()
{
    return m_valueAvro;
}

void ConsumerTypeSelector::setAvroValue(AvroOption *option)
{
    m_valueAvro = option;
    emit typesChanged();
    connect(m_valueAvro, &AvroOption::changed, this, &ConsumerTypeSelector::typesChanged);
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
