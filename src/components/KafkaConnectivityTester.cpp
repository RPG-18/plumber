#include <kafka/AdminClient.h>
#include <spdlog/spdlog.h>
#include <thread>

#include <QtCore/QDebug>
#include <QtCore/QMetaObject>

#include "ClusterConfig.h"
#include "KafkaConnectivityTester.h"

using namespace kafka;

KafkaConnectivityTester::KafkaConnectivityTester(QObject *parent)
    : QObject(parent)
{}

void KafkaConnectivityTester::test(QString url) noexcept
{
    if (url.isEmpty()) {
        return;
    }

    std::thread t([this, url]() {
        AdminClientConfig cfg;
        cfg.put(AdminClientConfig::BOOTSTRAP_SERVERS, url.toStdString());

        AdminClient client(cfg);
        const auto res = client.listTopics(std::chrono::milliseconds(10000));
        const bool noErr = !res.errorCode();
        QMetaObject::invokeMethod(this, "tested", Qt::QueuedConnection, Q_ARG(bool, noErr));
    });
    t.detach();
}

void KafkaConnectivityTester::tested(bool result)
{
    emit isTested(result);
}

void KafkaConnectivityTester::testBroker(QVariant v) noexcept
{
    if (!v.canConvert<ClusterConfig>()) {
        spdlog::error("can't convert QVariant to BrokerConfig");
        return;
    }
    const auto broker = v.value<ClusterConfig>();
    std::thread t([this, broker]() {
        AdminClientConfig cfg(broker.properties->map());
        cfg.put(AdminClientConfig::BOOTSTRAP_SERVERS, broker.bootstrap.toStdString());

        AdminClient client(cfg);
        const auto res = client.listTopics(std::chrono::milliseconds(10000));
        const bool noErr = !res.errorCode();
        QMetaObject::invokeMethod(this, "tested", Qt::QueuedConnection, Q_ARG(bool, noErr));
    });
    t.detach();
}
