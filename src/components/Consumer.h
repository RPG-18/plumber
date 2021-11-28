#pragma once

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QUrl>

#include "ClusterConfig.h"
#include "ErrorWrap.h"
#include "MessageModel.h"
#include "ProtoOption.h"
#include "Types.h"

namespace core {
class KafkaConsumer;
}

class QTimer;

/**!
 * Key Value type selector
 */
class ConsumerTypeSelector : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(Types keyType READ keyType WRITE setKeyType)
    Q_PROPERTY(Types valueType READ valueType WRITE setValueType)

    Q_PROPERTY(ProtoOption *protoKey READ protoKey WRITE setProtoKey)
    Q_PROPERTY(ProtoOption *protoValue READ protoValue WRITE setProtoValue)

    explicit ConsumerTypeSelector(QObject *parent = nullptr);

    Types keyType() const;
    void setKeyType(Types type);

    Types valueType() const;
    void setValueType(Types type);

    ProtoOption *protoKey();
    void setProtoKey(ProtoOption *option);

    ProtoOption *protoValue();
    void setProtoValue(ProtoOption *option);

signals:

    void typesChanged();

private:
    Types m_keyType;
    Types m_valueType;

    ProtoOption *m_keyProto;
    ProtoOption *m_valueProto;
};

class ConsumerLimiterSelector;
class ConsumerFilterSelector;
class ConsumerBeginningSelector;

/**!
 * Consumer wrapper for core::KafkaConsumer
 */
class Consumer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MessageModel *messages READ messageModel NOTIFY messagesChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(ClusterConfig broker READ broker WRITE setBroker NOTIFY brokerChanged)
    Q_PROPERTY(QString topic MEMBER m_topic)
    Q_PROPERTY(QStringList topics MEMBER m_topics)

    Q_PROPERTY(ConsumerTypeSelector *types READ typeSelector WRITE setTypeSelector)
    Q_PROPERTY(ConsumerLimiterSelector *limiter READ limiter WRITE setLimiter)
    Q_PROPERTY(ConsumerFilterSelector *filter READ filter WRITE setFilter)
    Q_PROPERTY(ConsumerBeginningSelector *beginning READ beginning WRITE setBeginning)

    Q_PROPERTY(QString stat READ stat NOTIFY statChanged)

public:
    static constexpr auto RefreshInterval = std::chrono::seconds(1);

    enum StartFromTimeBased { Now, LastHour, Today, Yesterday, SpecificDate, Earliest };
    Q_ENUM(StartFromTimeBased)

    enum Filters { NoneFilter, Contains, NotContains, Equals };
    Q_ENUM(Filters)

    enum Limit { NoneLimit, NumbersOfRecords, SpecificDateLimit, MaxSizeBytes };
    Q_ENUM(Limit)

    explicit Consumer(QObject *parent = nullptr);
    virtual ~Consumer();

    ClusterConfig broker() const;
    void setBroker(const ClusterConfig &broker);

    MessageModel *messageModel() const;

    bool isRunning() const;

    Q_INVOKABLE ErrorWrap start();
    Q_INVOKABLE void stop();

    ConsumerTypeSelector *typeSelector();
    void setTypeSelector(ConsumerTypeSelector *selector);

    ConsumerLimiterSelector *limiter();
    void setLimiter(ConsumerLimiterSelector *limiter);

    ConsumerFilterSelector *filter();
    void setFilter(ConsumerFilterSelector *filter);

    ConsumerBeginningSelector *beginning();
    void setBeginning(ConsumerBeginningSelector *beginning);

    QString stat() const;

signals:

    void messagesChanged();
    void brokerChanged();
    void isRunningChanged();
    void statChanged();

private slots:

    void onReceived();
    void onStopped();
    void onKeyValueChanged();
    void onLimiterChanged();
    void onFilterChanged();
    void onBeginningChanged();

private:
    void setRunning(bool val);
    void setStartOnTime();
    void setLimit();
    void setFilter();
    void updateStat();
    ErrorWrap setConverter();

private:
    bool m_isRunning;
    bool m_argsChanged;

    ConsumerTypeSelector *m_typeSelector;
    ConsumerLimiterSelector *m_limiter;
    ConsumerFilterSelector *m_filter;
    ConsumerBeginningSelector *m_beginning;

    QString m_topic;
    QStringList m_topics;
    ClusterConfig m_broker;
    core::KafkaConsumer *m_consumer;
    QThread m_consumerThread;
    QList<QMetaObject::Connection> m_connections;
    QTimer *m_refresh;
    core::KafkaConsumer::ConsumeStat m_stat;

    MessageModel *m_messageModel;
};
Q_DECLARE_METATYPE(Consumer::StartFromTimeBased)
Q_DECLARE_METATYPE(Consumer::Filters)
Q_DECLARE_METATYPE(Consumer::Limit)

/**!
 * Limiter selector
 */
class ConsumerLimiterSelector : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(Consumer::Limit limit READ limit WRITE setLimit)
    Q_PROPERTY(int maxSize READ maxSize WRITE setMaxSize)
    Q_PROPERTY(int numberOfRecords READ numberOfRecords WRITE setNumberOfRecords)
    Q_PROPERTY(QString specificDate READ specificDate WRITE setSpecificDate)

    explicit ConsumerLimiterSelector(QObject *parent = nullptr);

    Consumer::Limit limit() const noexcept;
    void setLimit(Consumer::Limit limit) noexcept;

    int maxSize() const noexcept;
    void setMaxSize(int size) noexcept;

    int numberOfRecords() const noexcept;
    void setNumberOfRecords(int records) noexcept;

    const QString &specificDate() const;
    void setSpecificDate(const QString &limit);

    QDateTime specificDateTimePoint() const;

signals:

    void limitChanged();

public:
    Consumer::Limit m_limit;
    int m_numberOfRecords;
    int m_maxSize;
    QString m_specificDate;
};

/**!
 * Filter selector
 */
class ConsumerFilterSelector : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(Consumer::Filters filters READ filters WRITE setFilters)
    Q_PROPERTY(QString key READ key WRITE setKey)
    Q_PROPERTY(QString value READ value WRITE setValue)
    Q_PROPERTY(QString headerKey READ headerKey WRITE setHeaderKey)
    Q_PROPERTY(QString headerValue READ headerValue WRITE setHeaderValue)

    explicit ConsumerFilterSelector(QObject *parent = nullptr);

    Consumer::Filters filters() const noexcept;
    void setFilters(Consumer::Filters filters) noexcept;

    const QString &key() const;
    void setKey(const QString &key);

    const QString &value() const;
    void setValue(const QString &value);

    const QString &headerKey() const;
    void setHeaderKey(const QString &key);

    const QString &headerValue() const;
    void setHeaderValue(const QString &value);

signals:

    void filterChanged();

private:
    Consumer::Filters m_filters;
    QString m_key;
    QString m_value;
    QString m_headerKey;
    QString m_headerValue;
};

/**!
 * Setup offsets
 */
class ConsumerBeginningSelector : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(Consumer::StartFromTimeBased startFrom READ startFrom WRITE setStartFrom)
    Q_PROPERTY(QString specificDate READ specificDate WRITE setSpecificDate)

    explicit ConsumerBeginningSelector(QObject *parent = nullptr);

    const QString &specificDate() const;
    void setSpecificDate(const QString &date);

    Consumer::StartFromTimeBased startFrom() const;
    void setStartFrom(Consumer::StartFromTimeBased from);

    QDateTime specificDateTimePoint() const;

signals:

    void beginningChanged();

private:
    Consumer::StartFromTimeBased m_startFromTime;
    QString m_specificDate;
};