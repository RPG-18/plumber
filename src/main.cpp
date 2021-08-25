#include <array>
#include <memory>

#include <QtCore/QDebug>
#include <QtCore/QFile>

#include <QtGui/QFontDatabase>
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
#include "TopicCreator.h"
#include "utils/KafkaUtility.h"

void init()
{
    auto cfg = std::make_shared<ConfigurationService>();
    cfg->load();
    Services->setConfiguration(cfg);
}

void loadFonts()
{
    const std::array<QString, 12> fonts = {":/fonts/Roboto-BlackItalic.ttf",
                                           ":/fonts/Roboto-Black.ttf",
                                           ":/fonts//Roboto-BoldItalic.ttf",
                                           ":/fonts/Roboto-Bold.ttf",
                                           ":/fonts//Roboto-Italic.ttf",
                                           ":/fonts//Roboto-LightItalic.ttf",
                                           ":/fonts//Roboto-Light.ttf",
                                           ":/fonts//Roboto-MediumItalic.ttf",
                                           ":/fonts//Roboto-Medium.ttf",
                                           ":/fonts//Roboto-Regular.ttf",
                                           ":/fonts//Roboto-ThinItalic.ttf",
                                           ":/fonts//Roboto-Thin.ttf"};
    for (const auto &font : fonts) {
        const auto res = QFontDatabase::addApplicationFont(font);
        if (res == -1) {
            qDebug() << "failed load font" << font;
        }
    }
}

int main(int argc, char *argv[])
{
    qmlRegisterType<KafkaConnectivityTester>("kafkaui", 1, 0, "ConnectivityTester");
    qmlRegisterType<ConfigModel>("kafkaui", 1, 0, "ConfigModel");
    qmlRegisterType<Cluster>("kafkaui", 1, 0, "Cluster");
    qmlRegisterType<TopicFilterModel>("kafkaui", 1, 0, "TopicFilterModel");
    qmlRegisterType<Consumer>("kafkaui", 1, 0, "Consumer");
    qmlRegisterType<ConsumerTypeSelector>("kafkaui", 1, 0, "ConsumerTypeSelector");
    qmlRegisterType<ConsumerLimiterSelector>("kafkaui", 1, 0, "ConsumerLimiterSelector");
    qmlRegisterType<ConsumerFilterSelector>("kafkaui", 1, 0, "ConsumerFilterSelector");
    qmlRegisterType<ConsumerBeginningSelector>("kafkaui", 1, 0, "ConsumerBeginningSelector");
    qmlRegisterType<TopicCreator>("kafkaui", 1, 0, "TopicCreator");

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
    //loadFonts();

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/qml/main.qml");
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
}
