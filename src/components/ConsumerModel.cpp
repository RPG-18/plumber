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
        std::cout << group.group << " " << group.members.size() << std::endl;
        std::cout << "  state:" << group.state << std::endl;
        std::cout << "  protocol:" << group.protocol << std::endl;
        for (auto &member : group.members) {
            std::cout << "    " << member.clientId << " - " << member.memberId << " "
                      << member.assignment.size() << std::endl;
        }
        ConsumerGroupInfo info;
        info.group = QString::fromStdString(group.group);
        info.state = string2state.value(group.state, ConsumerGroupInfo::State::Unknown);
        //info.state = QString::fromStdString(group.state);
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
    switch (role) {
    case Group:
        return m_groups[row].group;

    case State:
        return groupStateToString(m_groups[row].state);

    case Members:
        return m_groups[row].members.size();

    case PartitionTopics:
        return "0/0";

    default:
        return QString{};
    }
}

QHash<int, QByteArray> ConsumerModel::roleNames() const
{
    static QHash<int, QByteArray> roles{
        {Qt::DisplayRole, "display"},
        {Group, "group"},
        {State, "state"},
        {Members, "members"},
        {PartitionTopics, "partitionTopics"},
    };

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
