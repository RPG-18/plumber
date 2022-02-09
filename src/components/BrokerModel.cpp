#include <thread>

#include "BrokerModel.h"
#include "KafkaAdmin.h"
#include "spdlog/spdlog.h"

using namespace kafka;

BrokerModel::BrokerModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int BrokerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return int(m_nodes.size());
}

QVariant BrokerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    const auto row = index.row();
    if (role == Node) {
        return m_nodes[row];
    }

    return {};
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
    return int(m_nodes.size());
}

void BrokerModel::setConfig(const ClusterConfig &broker)
{
    m_config = broker;
    loadMetaData();
}

void BrokerModel::loadMetaData()
{
    using namespace core;

    std::thread t([this, config = m_config]() {
        KafkaAdmin admin(config);

        auto [md, err] = admin.fetchNodesMetadata();
        if (md) {
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
