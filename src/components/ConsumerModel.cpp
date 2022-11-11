#include <iostream>
#include <thread>

#include "ConsumerModel.h"
#include "KafkaAdmin.h"

namespace {

QVector<ConsumerGroupInfo> convertGroup(std::vector<core::GroupInfo> &groups)
{
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
        info.state = QString::fromStdString(group.state);
        out.emplace_back(info);
    }
    return out;
}

} // namespace

ConsumerModel::ConsumerModel(QObject *parent)
    : QAbstractTableModel(parent)
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
    beginResetModel();
    m_groups.swap(groups);
    endResetModel();
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
        return m_groups[row].state;

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