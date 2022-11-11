#pragma once

#include <QtCore/QAbstractTableModel>
#include <QtCore/QHash>

#include "ClusterConfig.h"

struct ConsumerGroupInfo
{
    QString group;
    QString state;
    struct Member
    {};
    QVector<Member> members;
};
Q_DECLARE_METATYPE(ConsumerGroupInfo)

/**!
 * Show consumer groups
 */
class ConsumerModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Roles { Group = Qt::UserRole + 1, State, Members, PartitionTopics };

    explicit ConsumerModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setConfig(const ClusterConfig &broker);

private slots:

    void setGroups(QVector<ConsumerGroupInfo> groups);

private:
    void loadGroups();

private:
    QVector<QString> m_headers;
    ClusterConfig m_config;
    QVector<ConsumerGroupInfo> m_groups;
};
