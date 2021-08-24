#pragma once

#include <QtCore/QObject>

#include "ClusterConfig.h"
#include "ErrorWrap.h"

/**!
 * TopicCreator helper for create topic
 */
class TopicCreator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name)
    Q_PROPERTY(int partitions MEMBER m_partitions)
    Q_PROPERTY(int replicationFactor MEMBER m_replicationFactor)
    Q_PROPERTY(bool retention MEMBER m_retention)
    Q_PROPERTY(bool compaction MEMBER m_compaction)
    Q_PROPERTY(ClusterConfig broker READ config WRITE setConfig)

public:
    explicit TopicCreator(QObject *parent = nullptr);

    Q_INVOKABLE ErrorWrap create();

    ClusterConfig config() const;
    void setConfig(const ClusterConfig &cfg);

private:
    QString m_name;
    int m_partitions;
    int m_replicationFactor;
    bool m_retention;
    bool m_compaction;
    ClusterConfig m_config;
};