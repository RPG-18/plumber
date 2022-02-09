#pragma once

#include <QtCore/QObject>

#include "ClusterConfig.h"

namespace kafka {
namespace clients {
class KafkaConsumer;
}
} // namespace kafka

namespace core {

class KafkaStatistic : public QObject
{
    Q_OBJECT
public:
    static constexpr auto PoolInterval = std::chrono::milliseconds(1000);
    static constexpr auto PoolTimeout = std::chrono::milliseconds(30000);

    KafkaStatistic(ClusterConfig cfg, QObject *parent = nullptr);
    ~KafkaStatistic() = default;

public slots:

    /*!
     * start pooling
     */
    void start();

    /*!
     * stop pooling
     */
    void stop();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void pool();
    void createClient();

private:
    int m_timerId;
    ClusterConfig m_cfg;
    std::unique_ptr<kafka::clients::KafkaConsumer> m_client;
};
} // namespace core