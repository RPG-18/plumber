#include "TopicCreator.h"
#include "AdminClient.hpp"
#include "KafkaAdmin.h"
#include "spdlog/spdlog.h"

TopicCreator::TopicCreator(QObject *parent)
    : QObject(parent)
    , m_partitions(3)
    , m_replicationFactor(1)
    , m_retention(true)
    , m_compaction(false)
{}

ErrorWrap TopicCreator::create()
{
    const QString when("topic create");

    QStringList policy;
    if (m_retention) {
        policy.push_back("delete");
    }
    if (m_compaction) {
        policy.push_back("compact");
    }

    kafka::Properties topicConfig;

    topicConfig.put("cleanup.policy", policy.join(',').toStdString());

    core::KafkaAdmin admin(m_config);
    auto err = admin.createTopics({m_name}, m_partitions, m_replicationFactor, topicConfig);
    if (err) {
        return ErrorWrap{err->where, err->what};
    }
    return ErrorWrap{};
}

ClusterConfig TopicCreator::config() const
{
    return m_config;
}

void TopicCreator::setConfig(const ClusterConfig &cfg)
{
    m_config = cfg;
}
