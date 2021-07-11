#include <QtCore/QAbstractTableModel>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#include "Message.h"

MessageWrapper::MessageWrapper(QObject *parent)
    : QObject(parent)
{}

const Message &MessageWrapper::message() const
{
    return m_message;
}

void MessageWrapper::setMessage(const Message &message)
{
    m_message = message;
    notifyChanges();
}

QString MessageWrapper::key() const
{
    return format(m_message.keyType, m_message.key);
}

QString MessageWrapper::value() const
{
    return format(m_message.valueType, m_message.value);
}

QString MessageWrapper::topic() const
{
    return m_message.topic;
}

quint64 MessageWrapper::offset() const
{
    return m_message.offset;
}

int MessageWrapper::partition() const
{
    return m_message.partition;
}

QString MessageWrapper::timestamp() const
{
    const auto dt = QDateTime::fromMSecsSinceEpoch(m_message.timestamp.msSinceEpoch);
    const auto out = QString("%1 (epoch: %2)")
                         .arg(dt.toString("yyyy-MM-dd hh:mm:ss"))
                         .arg(m_message.timestamp.msSinceEpoch);
    return out;
}

QString MessageWrapper::timestampType() const
{
    switch (m_message.timestamp.type) {
    case kafka::Timestamp::Type::NotAvailable:
        return QString("NotAvailable");

    case kafka::Timestamp::Type::CreateTime:
        return QString("CreateTime");

    case kafka::Timestamp::Type::LogAppendTime:
        return QString("LogAppendTime");

    default:
        return QString();
    }
}

void MessageWrapper::notifyChanges()
{
    emit keyChanged();
    emit valueChanged();
    emit offsetChanged();
    emit topicChanged();
    emit partitionChanged();
    emit timestampChanged();
    emit timestampTypeChanged();
}

HeaderTableModel::HeaderTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

int HeaderTableModel::rowCount(const QModelIndex &) const
{
    return m_message.headers.size();
}

int HeaderTableModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant HeaderTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    const auto row = index.row();
    const auto col = index.column();
    switch (col) {
    case 0:
        return m_message.headers[row].key;
    case 1:
        return m_message.headers[row].value;
    }
    return QVariant();
}

QHash<int, QByteArray> HeaderTableModel::roleNames() const
{
    static QHash<int, QByteArray> roles{{Qt::DisplayRole, "display"}};
    return roles;
}

const Message &HeaderTableModel::message() const
{
    return m_message;
}

void HeaderTableModel::setMessage(const Message &message)
{
    beginResetModel();
    m_message = message;
    endResetModel();
}
