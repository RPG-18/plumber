#include <QtEndian>

#include <QtCore/QDateTime>
#include <QtCore/QTextStream>

#include "MessageModel.h"

namespace {
const QString NULL_VALUE("(null)");

}

MessageModel::MessageModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_keyType(Types::String)
    , m_valueType(Types::String)
{
    m_headers << "Topic"
              << "Par"
              << "Offset"
              << "Timestamp"
              << "Key"
              << "Value"
              << "Headers";
}

int MessageModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return m_records.size();
}

int MessageModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return m_headers.size();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)

    if (role < Qt::DisplayRole) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        role = Topic + index.column();
    }

    auto *const record = m_records[index.row()];
    switch (role) {
    case Topic:
        return record->topic;

    case Partition:
        return QString::number(record->partition);

    case Offset:
        return QString::number(record->offset);

    case Timestamp: {
        const auto msgTime = QDateTime::fromMSecsSinceEpoch(record->timestamp.msSinceEpoch);
        return msgTime.toString("yyyy-MM-dd hh:mm:ss");
    }

    case Key: {
        if (record->key.isEmpty()) {
            return NULL_VALUE;
        }

        return format(m_keyType, record->key);
    }

    case Value: {
        if (record->value.isEmpty()) {
            return NULL_VALUE;
        }
        return format(m_valueType, record->value);
    }

    case Headers:
        if (!record->headers.empty()) {
            QString str;
            QTextStream stream(&str, QIODeviceBase::WriteOnly);
            for (const auto &header : record->headers) {
                stream << header.key << ":" << header.value << "\n";
            }
            stream.flush();
            return str;
        }
        return QString();

    default:
        return QString();
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    static QHash<int, QByteArray> roles{{Qt::DisplayRole, "display"},
                                        {Topic, "topic"},
                                        {Partition, "partition"},
                                        {Offset, "offset"},
                                        {Timestamp, "timestamp"},
                                        {Key, "key"},
                                        {Value, "value"},
                                        {Headers, "headers"}};
    return roles;
}

QVariant MessageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation != Qt::Horizontal) {
        return section;
    }

    return m_headers.at(section);
}

void MessageModel::append(core::ConsumerRecords &&records)
{
    if (records.isEmpty()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, int(records.size() - 1));
    for (const auto &record : records) {
        m_records.push_front(record);
    }
    endInsertRows();

    if (m_records.size() > MaxMessages) {
        const auto delta = m_records.size() - MaxMessages;
        beginRemoveRows(QModelIndex(), MaxMessages, int(MaxMessages + delta));
        while (m_records.size() > MaxMessages) {
            delete m_records.back();
            m_records.removeLast();
        }
        endRemoveRows();
    }
}

void MessageModel::setKeyType(Types type)
{
    m_keyType = type;
}

void MessageModel::setValueType(Types type)
{
    m_valueType = type;
}

Message MessageModel::getMessage(int index) const
{
    if (index > m_records.size()) {
        return Message();
    }
    Message rec;
    *static_cast<core::ConsumerRecord *>(&rec) = *m_records[index];
    rec.keyType = m_keyType;
    rec.valueType = m_valueType;
    return rec;
}