#pragma once

#include <QtCore/QAbstractListModel>
#include <QtCore/QAbstractTableModel>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QSortFilterProxyModel>

#include "ClusterConfig.h"

struct ConsumerGroupInfo
{
    Q_GADGET
public:
    enum class State { Dead, Empty, Rebalance, Stable, Unknown };

    QString group;
    State state;
    QString protocol;
    struct Member
    {
        using Topic = QString;
        using Partition = int32_t;
        using TopicParition = std::pair<Topic, Partition>;

        QString id;
        QString clientID;
        QString host;
        QSet<TopicParition> topicPartitions;
    };
    QVector<Member> members;
    int topics;
    int partitions;
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
    enum Roles { Group = Qt::UserRole + 1, State, Members, PartitionTopics, GroupItem };

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

class ConsumerFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(ConsumerModel *model READ model WRITE setModel)
    Q_PROPERTY(QString filter READ filter WRITE setFilter)

public:
    ConsumerFilterModel(QObject *parent = nullptr);

    void setModel(ConsumerModel *model);
    ConsumerModel *model() const;

    QString filter() const;
    void setFilter(const QString &topic);

private:
    QString m_filter;
};

class GroupMemberModel;

class ConsumerGroupInfoItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant group WRITE setGroupInfo)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString strategy READ strategy NOTIFY strategyChanged)
    Q_PROPERTY(int topics READ topics NOTIFY topicsChanged)
    Q_PROPERTY(int partitions READ partitions NOTIFY partitionsChanged)
    Q_PROPERTY(GroupMemberModel *members READ members NOTIFY membersChanged)

public:
    explicit ConsumerGroupInfoItem(QObject *parent = nullptr);

    void setGroupInfo(const QVariant &info);
    QString name() const;
    QString state() const;
    QString strategy() const;
    int topics() const;
    int partitions() const;
    GroupMemberModel *members();

signals:

    void nameChanged();
    void stateChanged();
    void topicsChanged();
    void partitionsChanged();
    void strategyChanged();
    void membersChanged();

private:
    void sendChangeSignals();

private:
    ConsumerGroupInfo m_group;
    GroupMemberModel *m_members;
};

class GroupMemberModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles { MemberID = Qt::UserRole + 1, ClientID, HostName, Partitions, TopicsPartitions };

    explicit GroupMemberModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setMembers(const QVector<ConsumerGroupInfo::Member> &members);

private:
    QStringList topicPartitions(const QSet<ConsumerGroupInfo::Member::TopicParition> &tps) const;

private:
    QVector<ConsumerGroupInfo::Member> m_members;
};