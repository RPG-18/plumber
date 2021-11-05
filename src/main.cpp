#include <array>
#include <memory>

#include <QtCore/QDebug>
#include <QtCore/QFile>

#include <QtGui/QFontDatabase>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>

#include <QtWidgets/QApplication>

#include <kafka/KafkaClient.h>

#include "Cluster.h"
#include "ConfigModel.h"
#include "Consumer.h"
#include "ConsumerHelperModels.h"
#include "KafkaConnectivityTester.h"
#include "Producer.h"
#include "Registry.h"
#include "TopicCreator.h"
#include "protobuf/ProtobufMessagesModel.h"
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
                                           ":/fonts/Roboto-BoldItalic.ttf",
                                           ":/fonts/Roboto-Bold.ttf",
                                           ":/fonts/Roboto-Italic.ttf",
                                           ":/fonts/Roboto-LightItalic.ttf",
                                           ":/fonts/Roboto-Light.ttf",
                                           ":/fonts/Roboto-MediumItalic.ttf",
                                           ":/fonts/Roboto-Medium.ttf",
                                           ":/fonts/Roboto-Regular.ttf",
                                           ":/fonts/Roboto-ThinItalic.ttf",
                                           ":/fonts/Roboto-Thin.ttf"};
    for (const auto &font : fonts) {
        const auto res = QFontDatabase::addApplicationFont(font);
        if (res == -1) {
            qDebug() << "failed load font" << font;
        }
    }
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(protobuf);

    qmlRegisterType<KafkaConnectivityTester>("plumber", 1, 0, "ConnectivityTester");
    qmlRegisterType<ConfigModel>("plumber", 1, 0, "ConfigModel");
    qmlRegisterType<Cluster>("plumber", 1, 0, "Cluster");
    qmlRegisterType<TopicFilterModel>("plumber", 1, 0, "TopicFilterModel");
    qmlRegisterType<Consumer>("plumber", 1, 0, "Consumer");
    qmlRegisterType<ConsumerTypeSelector>("plumber", 1, 0, "ConsumerTypeSelector");
    qmlRegisterType<ConsumerLimiterSelector>("plumber", 1, 0, "ConsumerLimiterSelector");
    qmlRegisterType<ConsumerFilterSelector>("plumber", 1, 0, "ConsumerFilterSelector");
    qmlRegisterType<ConsumerBeginningSelector>("plumber", 1, 0, "ConsumerBeginningSelector");
    qmlRegisterType<TopicCreator>("plumber", 1, 0, "TopicCreator");
    qmlRegisterType<formats::protobuf::ProtobufMessagesModel>("plumber",
                                                              1,
                                                              0,
                                                              "ProtobufMessagesModel");

    qmlRegisterType<Producer>("plumber", 1, 0, "Producer");
    qmlRegisterType<ProducerOptions>("plumber", 1, 0, "ProducerOptions");

    qmlRegisterType<ConsumerTypesModel>("plumber", 1, 0, "ConsumerTypesModel");
    qmlRegisterType<CustomTypesModel>("plumber", 1, 0, "CustomTypesModel");
    qmlRegisterType<StartFromTimeBasedModel>("plumber", 1, 0, "StartFromTimeBasedModel");
    qmlRegisterType<FiltersModel>("plumber", 1, 0, "FiltersModel");
    qmlRegisterType<LimitModel>("plumber", 1, 0, "LimitModel");
    qmlRegisterType<MessageWrapper>("plumber", 1, 0, "MessageWrapper");
    qmlRegisterType<HeaderTableModel>("plumber", 1, 0, "HeaderTableModel");

    qmlRegisterAnonymousType<BrokerModel>("plumber", 1);
    qmlRegisterAnonymousType<TopicModel>("plumber", 1);
    qmlRegisterAnonymousType<MessageModel>("plumber", 1);

    kafka::KafkaClient::setGlobalLogger(KafkaSpdLogger);
    QCoreApplication::setApplicationName("plumber");

    QApplication app(argc, argv);
    init();
    loadFonts();

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

    return QApplication::exec();
}
