#include <kafka/AdminClient.h>
#include <thread>

#include <QtCore/QDebug>
#include <QtCore/QMetaObject>
#include <QtCore/QTextStream>

#include "ClusterConfig.h"
#include "KafkaConnectivityTester.h"
#include "spdlog/spdlog.h"

using namespace kafka;

KafkaConnectivityTester::KafkaConnectivityTester(QObject *parent)
    : QObject(parent)
{}

void KafkaConnectivityTester::test(QString url, QString props) noexcept
{
    if (url.isEmpty()) {
        return;
    }

    std::thread t([this, url, props]() {
        AdminClientConfig cfg;
        cfg.put(AdminClientConfig::BOOTSTRAP_SERVERS, url.toStdString());
        const QChar sep('=');

        QString toParse = props;
        QTextStream stream(&toParse, QIODeviceBase::ReadOnly);
        stream.skipWhiteSpace();

        while (!stream.atEnd()) {
            const auto line = stream.readLine();
            const auto kv = line.split(sep);
            if (kv.length() != 2) {
                spdlog::warn("invalid properties string {}", line.toStdString());
                continue;
            }
            const auto key = kv[0].simplified().toStdString();
            const auto value = kv[1].simplified().toStdString();
            cfg.put(key, value);
        }
        bool noErr = false;
        try {
            AdminClient client(cfg);
            const auto res = client.listTopics(std::chrono::milliseconds(10000));
            noErr = !res.errorCode();
        } catch (const kafka::KafkaException &e) {
            spdlog::error("Unexpected exception caught: {}", e.what());
            QMetaObject::invokeMethod(this, "tested", Qt::QueuedConnection, Q_ARG(bool, false));
        }
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
        try {
            AdminClientConfig cfg(broker.properties->map());
            cfg.put(AdminClientConfig::BOOTSTRAP_SERVERS, broker.bootstrap.toStdString());

            AdminClient client(cfg);
            const auto res = client.listTopics(std::chrono::milliseconds(10000));
            const bool noErr = !res.errorCode();
            QMetaObject::invokeMethod(this, "tested", Qt::QueuedConnection, Q_ARG(bool, noErr));

        } catch (const kafka::KafkaException &e) {
            spdlog::error("Unexpected exception caught: {}", e.what());
            QMetaObject::invokeMethod(this, "tested", Qt::QueuedConnection, Q_ARG(bool, false));
        }
    });
    t.detach();
}
