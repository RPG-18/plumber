#pragma once

#include <kafka/KafkaProducer.h>

#include <QtCore/QObject>

#include "ClusterConfig.h"
#include "ProducerRecord.h"

namespace core {

/**!
 * KafkaProducer wrapper
 */
class KafkaProducer : public QObject
{
    Q_OBJECT
public:
    KafkaProducer(ClusterConfig cfg, QObject *parent = nullptr);

    /**!
     * send to topic
     * @param record record
     * @return null if catch error
     */
    std::optional<ProducerMetadata> send(const ProducerRecord &record);

    const QString &lastError();

private:
    void createProducer();
    void resetError();

private:
    ClusterConfig m_cfg;
    std::unique_ptr<kafka::KafkaSyncProducer> m_producer;
    QString m_lastError;
};
} // namespace core