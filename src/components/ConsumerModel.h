#pragma once

#include <QtCore/QAbstractTableModel>
#include <QtCore/QHash>

#include "ClusterConfig.h"

struct ConsumerGroupInfo
{
    enum class State { Dead, Empty, Rebalance, Stable, Unknown };

    QString group;
    State state;
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
    Q_PROPERTY(int inActive READ inActive NOTIFY inActiveChanged)
    Q_PROPERTY(int inEmpty READ inEmpty NOTIFY inEmptyChanged)
    Q_PROPERTY(int inRebalancing READ inRebalancing NOTIFY inRebalancingChanged)
    Q_PROPERTY(int inDead READ inDead NOTIFY inDeadChanged)

public:
    enum Roles { Group = Qt::UserRole + 1, State, Members, PartitionTopics };

    explicit ConsumerModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setConfig(const ClusterConfig &broker);

    int inActive() const;
    int inEmpty() const;
    int inRebalancing() const;
    int inDead() const;

signals:
    void inActiveChanged();
    void inEmptyChanged();
    void inRebalancingChanged();
    void inDeadChanged();

private slots:

    void setGroups(QVector<ConsumerGroupInfo> groups);

private:
    void loadGroups();
    void setInActive(int val);
    void setInEmpty(int val);
    void setInRebalancing(int val);
    void setInDead(int val);

private:
    QVector<QString> m_headers;
    ClusterConfig m_config;
    QVector<ConsumerGroupInfo> m_groups;
    int m_inActive;
    int m_inEmpty;
    int m_inRebalancing;
    int m_inDead;
};
