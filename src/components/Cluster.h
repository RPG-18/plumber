#pragma once

#include <QtCore/QObject>

#include "BrokerModel.h"
#include "ClusterConfig.h"
#include "ConsumerModel.h"
#include "TopicModel.h"

/**!
 * Cluster component
 */
class Cluster : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ClusterConfig broker READ broker WRITE setBroker NOTIFY brokerChanged)

public:
    explicit Cluster(QObject *parent = nullptr);

    ClusterConfig broker() const;
    void setBroker(const ClusterConfig &broker);

    Q_INVOKABLE BrokerModel *brokerModel();
    Q_INVOKABLE TopicModel *topicModel();
    Q_INVOKABLE ConsumerModel *consumerModel();

signals:

    void brokerChanged();

private:
    ClusterConfig m_broker;
    BrokerModel *m_brokerModel;
    TopicModel *m_topicModel;
    ConsumerModel *m_consumerModel;
};
