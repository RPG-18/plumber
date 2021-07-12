#include <memory>

#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>

#include <kafka/KafkaClient.h>

#include "Cluster.h"
#include "ConfigModel.h"
#include "Consumer.h"
#include "ConsumerHelperModels.h"
#include "KafkaConnectivityTester.h"
#include "Registry.h"
#include "utils/KafkaUtility.h"

void init()
{
    auto cfg = std::make_shared<ConfigurationService>();
    cfg->load();
    Services->setConfiguration(cfg);
}

int main(int argc, char *argv[])
{
    qmlRegisterType<KafkaConnectivityTester>("kafkaui", 1, 0, "ConnectivityTester");
    qmlRegisterType<ConfigModel>("kafkaui", 1, 0, "ConfigModel");
    qmlRegisterType<Cluster>("kafkaui", 1, 0, "Cluster");
    qmlRegisterType<TopicFilterModel>("kafkaui", 1, 0, "TopicFilterModel");
    qmlRegisterType<Consumer>("kafkaui", 1, 0, "Consumer");
    qmlRegisterType<ConsumerTypesModel>("kafkaui", 1, 0, "ConsumerTypesModel");
    qmlRegisterType<StartFromTimeBasedModel>("kafkaui", 1, 0, "StartFromTimeBasedModel");
    qmlRegisterType<FiltersModel>("kafkaui", 1, 0, "FiltersModel");
    qmlRegisterType<LimitModel>("kafkaui", 1, 0, "LimitModel");
    qmlRegisterType<MessageWrapper>("kafkaui", 1, 0, "MessageWrapper");
    qmlRegisterType<HeaderTableModel>("kafkaui", 1, 0, "HeaderTableModel");

    qmlRegisterAnonymousType<BrokerModel>("kafkaui", 1);
    qmlRegisterAnonymousType<TopicModel>("kafkaui", 1);
    qmlRegisterAnonymousType<MessageModel>("kafkaui", 1);

    kafka::KafkaClient::setGlobalLogger(KafkaSpdLogger);
    QCoreApplication::setApplicationName("kafkaui");

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    init();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}