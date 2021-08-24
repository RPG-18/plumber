#include "TopicCreator.h"
#include "spdlog/spdlog.h"

#include <kafka/AdminClient.h>

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

    try {
        kafka::AdminClientConfig cfg(m_config.properties->map());
        cfg.put(kafka::AdminClientConfig::BOOTSTRAP_SERVERS, m_config.bootstrap.toStdString());

        kafka::AdminClient client(cfg);
        kafka::Properties topicConfig;

        QStringList policy;
        if (m_retention) {
            policy.push_back("delete");
        }
        if (m_compaction) {
            policy.push_back("compact");
        }
        topicConfig.put("cleanup.policy", policy.join(',').toStdString());

        auto result = client.createTopics({m_name.toStdString()},
                                          m_partitions,
                                          m_replicationFactor,
                                          topicConfig);
        if (result.errorCode()) {
            spdlog::error("topic create error {}", result.message());
            return ErrorWrap{when, QString::fromStdString(result.message())};
        }
    } catch (const kafka::KafkaException &e) {
        spdlog::error("topic create exception {}", e.what());
        return ErrorWrap{when, QString::fromStdString(e.what())};
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