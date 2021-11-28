#pragma once

#include <QtCore/QAbstractListModel>
#include <QtCore/QObject>

#include "ClusterConfig.h"
#include "ErrorWrap.h"
#include "ProducerRecord.h"
#include "ProtoOption.h"
#include "Types.h"

namespace core {
class KafkaProducer;
}

class ProducerOptions;
class ProducerLogModel;

/**!
 * Producer wrapper
 */
class Producer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ProducerOptions *options READ options WRITE setOptions)
    Q_PROPERTY(Types keyType MEMBER m_keyType)
    Q_PROPERTY(Types valueType MEMBER m_valueType)
    Q_PROPERTY(QString topic MEMBER m_topic)
    Q_PROPERTY(ClusterConfig broker READ broker WRITE setBroker NOTIFY brokerChanged)
    Q_PROPERTY(ProducerLogModel *log READ log NOTIFY logChanged)
    Q_PROPERTY(ProtoOption *protoKey READ protoKey WRITE setProtoKey)
    Q_PROPERTY(ProtoOption *protoValue READ protoValue WRITE setProtoValue)

public:
    enum Compression { NoneCompression, GZip, Snappy, LZ4, ZStd };
    Q_ENUM(Compression)

    enum Ack { NoAck, Leader, All };
    Q_ENUM(Ack)

    explicit Producer(QObject *parent = nullptr);

    ProducerOptions *options();
    void setOptions(ProducerOptions *option);

    ClusterConfig broker() const;
    void setBroker(const ClusterConfig &broker);

    ProducerLogModel *log() noexcept;
    Q_INVOKABLE ErrorWrap send(const QString &key, const QString &value);

    ProtoOption *protoKey();
    void setProtoKey(ProtoOption *option);

    ProtoOption *protoValue();
    void setProtoValue(ProtoOption *option);

signals:

    void brokerChanged();
    void logChanged();

private slots:

    void onOptionsChanged();

private:
    void createProducer();

private:
    core::KafkaProducer *m_producer;
    ProducerOptions *m_options;
    Types m_keyType;
    Types m_valueType;
    QString m_topic;
    ClusterConfig m_broker;
    ProducerLogModel *m_logModel;
    ProtoOption *m_keyProto;
    ProtoOption *m_valueProto;
};

/**!
 * Producer options
 */
class ProducerOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Producer::Compression compression READ compression WRITE setCompression)
    Q_PROPERTY(Producer::Ack ack READ ack WRITE setAck)
    Q_PROPERTY(bool idempotence READ idempotence WRITE setIdempotence)

public:
    explicit ProducerOptions(QObject *parent = nullptr);

    Producer::Compression compression() const noexcept;
    void setCompression(Producer::Compression compression) noexcept;

    Producer::Ack ack() const noexcept;
    void setAck(Producer::Ack ack);

    bool idempotence() const noexcept;
    void setIdempotence(bool value) noexcept;

signals:

    void changed();

private:
    Producer::Compression m_compression;
    Producer::Ack m_ack;
    bool m_idempotence;
};

/**!
 * ProducerLogModel
 */
class ProducerLogModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool isEmpty MEMBER m_isEmpty NOTIFY isEmptyChanged)
public:
    static constexpr int Reserve = 100;

    enum Roles { Topic = Qt::UserRole + 1, Partition, Offset, Timestamp };

    explicit ProducerLogModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    /*!
     * append records
     */
    void append(core::ProducerMetadata &&metadata);

signals:

    void isEmptyChanged();

private:
    void updateEmptyFlag();

private:
    QVector<core::ProducerMetadata> m_meta;
    bool m_isEmpty;
};