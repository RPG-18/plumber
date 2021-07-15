#include <thread>

#include "AdminClient.hpp"
#include "BrokerModel.h"
#include "spdlog/spdlog.h"

using namespace kafka;

BrokerModel::BrokerModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int BrokerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_nodes.size();
}

QVariant BrokerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    const auto row = index.row();
    switch (role) {
    case Node:
        return m_nodes[row];
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BrokerModel::roleNames() const
{
    static QHash<int, QByteArray> roles{
        {Node, "node"},
    };
    return roles;
}

int BrokerModel::brokers() const noexcept
{
    return m_nodes.size();
}

void BrokerModel::setConfig(const ClusterConfig &broker)
{
    m_config = broker;
    loadMetaData();
}

void BrokerModel::loadMetaData()
{
    std::thread t([this, config = m_config]() {
        AdminClientConfig cfg(config.properties->map());
        cfg.put(AdminClientConfig::BOOTSTRAP_SERVERS, config.bootstrap.toStdString());

        core::AdminClient client(cfg);
        const auto timeout = std::chrono::milliseconds(10000);
        if (auto md = client.fetchNodesMetadata(timeout)) {
            auto ptr = std::make_shared<BrokerMetadata>("");
            *ptr = *md;

            std::atomic_store(&this->m_md, ptr);
            QMetaObject::invokeMethod(this, "received", Qt::QueuedConnection);
        }
    });
    t.detach();
}

void BrokerModel::received()
{
    beginResetModel();
    for (const auto &node : m_md->nodes()) {
        m_nodes.push_back(QString::fromStdString(node->toString()));
    }
    endResetModel();
    emit brokersChanged();
}
