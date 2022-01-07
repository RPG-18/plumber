#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>

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

private:
    ClusterConfig m_cfg;
};
} // namespace core