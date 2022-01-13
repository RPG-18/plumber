#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>

#include <kafka/BrokerMetadata.h>

#include "ClusterConfig.h"

namespace core {

/**!
 * KafkaAdmin wrapper
 */
class KafkaAdmin : public QObject
{
    Q_OBJECT
public:
    static constexpr int DEFAULT_COMMAND_TIMEOUT_MS = 30000;
    using Topics = QVector<QString>;

    using BrokerMetadata = kafka::BrokerMetadata;

    struct Error
    {
        explicit Error();
        explicit Error(QString wh, QString wt);

        bool isError;
        QString where;
        QString what;
    };

    KafkaAdmin(ClusterConfig cfg, QObject *parent = nullptr);

    /**!
     * list topics
     * @param timeout operation time out
     * @return topics and error
     */
    std::tuple<Topics, Error> listTopics(
        std::chrono::milliseconds timeout = std::chrono::milliseconds(DEFAULT_COMMAND_TIMEOUT_MS));

    /**!
     * delete topic
     * @param topics list
     * @param timeout operation time out
     * @return error
     */
    std::optional<Error> deleteTopics(
        const Topics &topics,
        std::chrono::milliseconds timeout = std::chrono::milliseconds(DEFAULT_COMMAND_TIMEOUT_MS));

    /**!
     * fetch brokers
     * @param timeout
     * @return brokers and error
     */
    std::tuple<std::optional<BrokerMetadata>, Error> fetchNodesMetadata(
        std::chrono::milliseconds timeout = std::chrono::milliseconds(DEFAULT_COMMAND_TIMEOUT_MS));

    /**!
     * create topics
     * @param topics topic list
     * @param numPartitions partitions count
     * @param replicationFactor replication factor
     * @param topicConfig configuration
     * @param timeout timeout
     * @return error
     */
    std::optional<Error> createTopics(
        const Topics& topics,
        int numPartitions,
        int replicationFactor,
        const kafka::Properties &topicConfig,
        std::chrono::milliseconds timeout = std::chrono::milliseconds(DEFAULT_COMMAND_TIMEOUT_MS));

private:
    ClusterConfig m_cfg;
};
} // namespace core