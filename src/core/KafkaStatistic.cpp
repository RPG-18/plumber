#include <iostream>

#include <QtCore/QDebug>
#include <QtCore/QTimerEvent>

#include <kafka/KafkaConsumer.h>

#include "KafkaStatistic.h"

namespace core {
KafkaStatistic::KafkaStatistic(ClusterConfig cfg, QObject *parent)
    : QObject(parent)
    , m_timerId(0)
    , m_cfg(std::move(cfg))
{}

void KafkaStatistic::start()
{
    if (m_timerId != 0) {
        return;
    }

    m_timerId = startTimer(PoolInterval);
}

void KafkaStatistic::stop()
{
    if (m_timerId == 0) {
        return;
    }
    killTimer(m_timerId);
    m_timerId = 0;
}

void KafkaStatistic::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != m_timerId) {
        return;
    }
    pool();
}

void KafkaStatistic::pool()
{
    if (m_client == nullptr) {
        createClient();
    }

    qDebug() << "pool";
    m_client->poll(PoolTimeout);
    //m_client->pollEvents(PoolTimeout);
}

void KafkaStatistic::createClient()
{
    using namespace kafka::clients::consumer;

    Config props(m_cfg.properties->map());
    props.put(Config::BOOTSTRAP_SERVERS, m_cfg.bootstrap.toStdString());
    props.put("statistics.interval.ms", "1000");
    m_client = std::make_unique<kafka::clients::KafkaConsumer>(
        props, kafka::clients::KafkaClient::EventsPollingOption::Auto);
    m_client->setStatsCallback([](const std::string &data) { std::cout << data << std::endl; });
}

} // namespace core