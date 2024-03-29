#include <iostream>
#include <thread>

#include "ConsumerModel.h"
#include "KafkaAdmin.h"

namespace {

QString groupStateToString(ConsumerGroupInfo::State state)
{
    switch (state) {
    case ConsumerGroupInfo::State::Stable:
        return QLatin1String("Active");
    case ConsumerGroupInfo::State::Empty:
        return QLatin1String("Empty");
    case ConsumerGroupInfo::State::Rebalance:
        return QLatin1String("Rebalance");
    case ConsumerGroupInfo::State::Dead:
        return QLatin1String("Dead");
    default:
        return QLatin1String("Unknown state");
    }
}

QVector<ConsumerGroupInfo> convertGroup(std::vector<core::GroupInfo> &groups)
{
    static QHash<std::string, ConsumerGroupInfo::State> string2state = {
        {"Dead", ConsumerGroupInfo::State::Dead},
        {"Empty", ConsumerGroupInfo::State::Empty},
        {"Rebalance", ConsumerGroupInfo::State::Rebalance},
        {"Stable", ConsumerGroupInfo::State::Stable},
    };
    QVector<ConsumerGroupInfo> out;
    out.reserve(groups.size());

    for (auto &group : groups) {
        if (group.protocolType != "consumer") {
            continue;
        }

        ConsumerGroupInfo info;
        info.group = QString::fromStdString(group.group);
        info.state = string2state.value(group.state, ConsumerGroupInfo::State::Unknown);
        info.protocol = QString::fromStdString(group.protocol);

        QSet<QString> topics;
        int partitions = 0;
        for (auto &member : group.members) {
            ConsumerGroupInfo::Member m;
            m.host = QString::fromStdString(member.clientHost);
            m.id = QString::fromStdString(member.memberId);
            m.clientID = QString::fromStdString(member.clientId);

            core::MemberAssignmentInformation memberInfo(member.assignment);

            for (auto &tp : memberInfo.topicPartitions()) {
                auto topic = QString::fromStdString(tp.first);
                topics.insert(topic);
                m.topicPartitions.insert(std::make_pair(topic, tp.second));
                ++partitions;
            }

            info.members.emplace_back(m);
        }
        info.topics = topics.size();
        info.partitions = partitions;
        out.emplace_back(info);
    }
    return out;
}

} // namespace

ConsumerModel::ConsumerModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_inActive(0)
    , m_inEmpty(0)
    , m_inRebalancing(0)
    , m_inDead(0)
{
    m_headers << "Consumer Group"
              << "State"
              << "Members"
              << "Topics";
}

void ConsumerModel::setConfig(const ClusterConfig &broker)
{
    m_config = broker;
    loadGroups();
}

void ConsumerModel::loadGroups()
{
    std::thread loadThread([this, config = m_config]() {
        core::KafkaAdmin admin(config);

        auto [groups, err] = admin.listGroups();
        if (err.isError) {
            return;
        }

        QVector<ConsumerGroupInfo> consumers = convertGroup(groups);
        QMetaObject::invokeMethod(this,
                                  "setGroups",
                                  Qt::QueuedConnection,
                                  Q_ARG(QVector<ConsumerGroupInfo>, consumers));
    });
    loadThread.detach();
}

void ConsumerModel::setGroups(QVector<ConsumerGroupInfo> groups)
{
    int inActive = 0;
    int inEmpty = 0;
    int inRebalancing = 0;
    int inDead = 0;

    beginResetModel();
    m_groups.swap(groups);
    for (auto &group : m_groups) {
        switch (group.state) {
        case ConsumerGroupInfo::State::Stable: {
            ++inActive;
        }; break;
        case ConsumerGroupInfo::State::Empty: {
            ++inEmpty;
        }; break;
        case ConsumerGroupInfo::State::Rebalance: {
            ++inRebalancing;
        }; break;
        case ConsumerGroupInfo::State::Dead: {
            ++inDead;
        }; break;
        default: {
            // skip unknown
        }; break;
        }
    }
    endResetModel();
    setInActive(inActive);
    setInEmpty(inEmpty);
    setInRebalancing(inRebalancing);
    setInDead(inDead);
}

int ConsumerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_groups.size();
}

int ConsumerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_headers.size();
}

QVariant ConsumerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    if (index.row() >= m_groups.size()) {
        return {};
    }

    if (role < Qt::DisplayRole) {
        return {};
    }

    if (role == Qt::DisplayRole) {
        role = Group + index.column();
    }

    const auto row = index.row();
    auto &group = m_groups[row];
    switch (role) {
    case Group:
        return group.group;

    case State:
        return groupStateToString(group.state);

    case Members:
        return group.members.size();

    case PartitionTopics:
        return QString("%1 / %2").arg(group.partitions).arg(group.topics);

    case GroupItem:
        return QVariant::fromValue(m_groups[row]);

    default:
        return QString{};
    }
}

QHash<int, QByteArray> ConsumerModel::roleNames() const
{
    static QHash<int, QByteArray> roles{{Qt::DisplayRole, "display"},
                                        {Group, "group"},
                                        {State, "state"},
                                        {Members, "members"},
                                        {PartitionTopics, "partitionTopics"},
                                        {GroupItem, "groupItem"}};

    return roles;
}

int ConsumerModel::inActive() const
{
    return m_inActive;
}

void ConsumerModel::setInActive(int val)
{
    m_inActive = val;
    emit inActiveChanged();
}

int ConsumerModel::inEmpty() const
{
    return m_inEmpty;
}

void ConsumerModel::setInEmpty(int val)
{
    m_inEmpty = val;
    emit inEmptyChanged();
}

int ConsumerModel::inRebalancing() const
{
    return m_inRebalancing;
}

void ConsumerModel::setInRebalancing(int val)
{
    m_inRebalancing = val;
    emit inRebalancingChanged();
}

int ConsumerModel::inDead() const
{
    return m_inDead;
}

void ConsumerModel::setInDead(int val)
{
    m_inDead = val;
    emit inDeadChanged();
}

ConsumerFilterModel::ConsumerFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterRole(ConsumerModel::Group);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void ConsumerFilterModel::setModel(ConsumerModel *model)
{
    setSourceModel(model);
}
ConsumerModel *ConsumerFilterModel::model() const
{
    return dynamic_cast<ConsumerModel *>(sourceModel());
}

QString ConsumerFilterModel::filter() const
{
    return m_filter;
}

void ConsumerFilterModel::setFilter(const QString &topic)
{
    m_filter = topic;
    setFilterFixedString(m_filter);
}

ConsumerGroupInfoItem::ConsumerGroupInfoItem(QObject *parent)
    : QObject(parent)
    , m_members(new GroupMemberModel(this))
{}

void ConsumerGroupInfoItem::setGroupInfo(const QVariant &info)
{
    if (info.isNull() || !info.isValid()) {
        return;
    }

    m_group = info.value<ConsumerGroupInfo>();
    m_members->setMembers(m_group.members);
    sendChangeSignals();
}

void ConsumerGroupInfoItem::sendChangeSignals()
{
    emit nameChanged();
    emit stateChanged();
    emit topicsChanged();
    emit partitionsChanged();
    emit strategyChanged();
}

QString ConsumerGroupInfoItem::name() const
{
    return m_group.group;
}

QString ConsumerGroupInfoItem::strategy() const
{
    return m_group.protocol;
}

QString ConsumerGroupInfoItem::state() const
{
    switch (m_group.state) {
    case ConsumerGroupInfo::State::Stable:
        return QLatin1String("Stable");
    case ConsumerGroupInfo::State::Empty:
        return QLatin1String("Empty");
    case ConsumerGroupInfo::State::Rebalance:
        return QLatin1String("Rebalance");
    case ConsumerGroupInfo::State::Dead:
        return QLatin1String("Dead");
    default:
        return QLatin1String("Unknown state");
    }
}

int ConsumerGroupInfoItem::topics() const
{
    return m_group.topics;
}

int ConsumerGroupInfoItem::partitions() const
{
    return m_group.partitions;
}

GroupMemberModel *ConsumerGroupInfoItem::members()
{
    return m_members;
}

GroupMemberModel::GroupMemberModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int GroupMemberModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_members.size();
}

QVariant GroupMemberModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    if (index.row() >= m_members.size()) {
        return {};
    }

    if (role < Qt::DisplayRole) {
        return {};
    }

    const auto row = index.row();
    auto &member = m_members[row];

    switch (role) {
    case MemberID:
        return member.id;
    case ClientID:
        return member.clientID;
    case HostName:
        return member.host;
    case Partitions:
        return member.topicPartitions.size();
    case TopicsPartitions:
        return topicPartitions(member.topicPartitions);

    default:
        return QString{};
    }
}

void GroupMemberModel::setMembers(const QVector<ConsumerGroupInfo::Member> &members)
{
    beginResetModel();
    m_members = members;
    endResetModel();
}

QHash<int, QByteArray> GroupMemberModel::roleNames() const
{
    static QHash<int, QByteArray> roles{
        {MemberID, "memberID"},
        {ClientID, "clientID"},
        {HostName, "host"},
        {Partitions, "partitions"},
        {TopicsPartitions, "topicsPartitions"},
    };
    return roles;
}

QStringList GroupMemberModel::topicPartitions(
    const QSet<ConsumerGroupInfo::Member::TopicParition> &tps) const
{
    QStringList list;
    list.reserve(tps.size());

    for (const auto &tp : tps) {
        list << QString("%1-%2").arg(tp.first).arg(tp.second);
    }
    return list;
}