#include <QtCore/QDebug>

#include "Cluster.h"

Cluster::Cluster(QObject *parent)
    : QObject(parent)
    , m_brokerModel(new BrokerModel(this))
    , m_topicModel(new TopicModel(this))
    , m_consumerModel(new ConsumerModel(this))
{}

ClusterConfig Cluster::broker() const
{
    return m_broker;
}

void Cluster::setBroker(const ClusterConfig &broker)
{
    if (broker.name.isEmpty()) {
        return;
    }

    m_broker = broker;
    emit brokerChanged();

    m_brokerModel->setConfig(m_broker);
    m_topicModel->setConfig(m_broker);
    m_consumerModel->setConfig(m_broker);
}

BrokerModel *Cluster::brokerModel()
{
    return m_brokerModel;
}

TopicModel *Cluster::topicModel()
{
    return m_topicModel;
}

ConsumerModel *Cluster::consumerModel()
{
    return m_consumerModel;
}