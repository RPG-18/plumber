#pragma once

#include <QtCore/QAbstractListModel>
#include <QtCore/QObject>
#include <QtCore/QThread>

#include "ClusterConfig.h"
#include "MessageModel.h"
#include "Types.h"

namespace core {
class KafkaConsumer;
}

class Consumer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MessageModel *messages READ messageModel NOTIFY messagesChanged)
    Q_PROPERTY(QString specificDate MEMBER m_specificDate)
    Q_PROPERTY(QString keyFilter MEMBER m_keyFilter)
    Q_PROPERTY(QString valueFilter MEMBER m_valueFilter)
    Q_PROPERTY(QString headerKeyFilter MEMBER m_headerKeyFilter)
    Q_PROPERTY(QString headerValueFilter MEMBER m_headerValueFilter)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(ClusterConfig broker READ broker WRITE setBroker NOTIFY brokerChanged)
    Q_PROPERTY(QString topic MEMBER m_topic)
    Q_PROPERTY(int numberOfRecords MEMBER m_numberOfRecords)
    Q_PROPERTY(int maxSize MEMBER m_maxSize)
    Q_PROPERTY(QString limitSpecificDate MEMBER m_limitSpecificDate)

public:
    enum StartFromTimeBased { Now, LastHour, Today, Yesterday, SpecificDate, Earliest };
    Q_ENUM(StartFromTimeBased)

    enum Filters { NoneFilter, Contains, NotContains, Equals };
    Q_ENUM(Filters)

    enum Limit { NoneLimit, NumbersOfRecords, SpecificDateLimit, MaxSizeBytes };
    Q_ENUM(Limit)

    Q_PROPERTY(Types keyType READ keyType WRITE setKeyType)
    Q_PROPERTY(Types valueType READ valueType WRITE setValueType)

    Q_PROPERTY(StartFromTimeBased startFromTimeBased MEMBER m_startFromTime)
    Q_PROPERTY(Limit limit MEMBER m_limit)
    Q_PROPERTY(Filters filters READ filters WRITE setFilters)

    explicit Consumer(QObject *parent = nullptr);
    virtual ~Consumer();

    Types keyType() const;
    void setKeyType(Types type);

    Types valueType() const;
    void setValueType(Types type);

    Filters filters() const;
    void setFilters(Filters filter);

    ClusterConfig broker() const;
    void setBroker(const ClusterConfig &broker);

    MessageModel *messageModel() const;

    bool isRunning() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:

    void messagesChanged();
    void brokerChanged();
    void isRunningChanged();

private slots:

    void onReceived();
    void onStopped();

private:
    void setRunning(bool val);
    void setStartOnTime();
    void setLimit();
    void setFilter();

private:
    bool m_isRunning;
    Types m_keyType;
    Types m_valueType;
    StartFromTimeBased m_startFromTime;
    Filters m_filters;
    QString m_specificDate;
    QString m_keyFilter;
    QString m_valueFilter;
    QString m_headerKeyFilter;
    QString m_headerValueFilter;
    QString m_topic;
    ClusterConfig m_broker;
    core::KafkaConsumer *m_consumer;
    QThread m_consumerThread;
    Limit m_limit;
    int m_numberOfRecords;
    int m_maxSize;
    QString m_limitSpecificDate;

    MessageModel *m_messageModel;
};
Q_DECLARE_METATYPE(Consumer::StartFromTimeBased)
Q_DECLARE_METATYPE(Consumer::Filters)