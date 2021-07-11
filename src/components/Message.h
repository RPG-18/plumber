#pragma once

#include <KafkaConsumer.h>

#include "Types.h"

/*!
 * Extended Consumer Record
 */
struct Message final : public core::ConsumerRecord
{
    Types keyType;
    Types valueType;
};
Q_DECLARE_METATYPE(Message)

/**!
 * Wrapper for extract data
 */
class MessageWrapper : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(Message message READ message WRITE setMessage)
    Q_PROPERTY(QString key READ key NOTIFY keyChanged)
    Q_PROPERTY(QString value READ value NOTIFY valueChanged)
    Q_PROPERTY(QString topic READ topic NOTIFY topicChanged)
    Q_PROPERTY(quint64 offset READ offset NOTIFY offsetChanged)
    Q_PROPERTY(int partition READ partition NOTIFY partitionChanged)
    Q_PROPERTY(QString timestamp READ timestamp NOTIFY timestampChanged)
    Q_PROPERTY(QString timestampType READ timestampType NOTIFY timestampTypeChanged)

    explicit MessageWrapper(QObject *parent = nullptr);

    const Message &message() const;
    void setMessage(const Message &message);

    QString key() const;
    QString value() const;
    QString topic() const;
    quint64 offset() const;
    int partition() const;
    QString timestamp() const;
    QString timestampType() const;

signals:

    void keyChanged();
    void valueChanged();
    void offsetChanged();
    void topicChanged();
    void partitionChanged();
    void timestampChanged();
    void timestampTypeChanged();

private:
    void notifyChanges();

private:
    Message m_message;
};

/**!
 * Show headers as table
 */
class HeaderTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(Message message READ message WRITE setMessage)
public:
    HeaderTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override final;
    QHash<int, QByteArray> roleNames() const override;

    const Message &message() const;
    void setMessage(const Message &message);

private:
    Message m_message;
};