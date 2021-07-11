#pragma once

#include <QtCore/QAbstractTableModel>

#include "Message.h"

/*!
 * Table/List model
 */
class MessageModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    static constexpr int MaxMessages = 1000;
    enum Roles { Topic = Qt::UserRole + 1, Partition, Offset, Timestamp, Key, Value, Headers };

    MessageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override final;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setKeyType(Types type);
    void setValueType(Types type);

    /*!
     *
     * @param records
     */
    void append(core::ConsumerRecords &&records);

    Q_INVOKABLE Message getMessage(int index) const;

private:
    QVector<QString> m_headers;
    core::ConsumerRecords m_records;
    Types m_keyType;
    Types m_valueType;
};
