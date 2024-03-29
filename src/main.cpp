#include <array>
#include <memory>

#include <QtCore/QDebug>
#include <QtCore/QFile>

#include <QtGui/QFontDatabase>

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QtQuickControls2/QQuickStyle>

#include <QtWidgets/QApplication>

#include <kafka/KafkaClient.h>

#include "3rdparty/syntax-highlighting/src/quick/kquicksyntaxhighlightingplugin.h"
#include "Helpers.h"
#include "Registry.h"
#include "protobuf/ProtobufMessagesModel.h"
#include "utils/KafkaUtility.h"

void init()
{
    auto cfg = std::make_shared<ConfigurationService>();
    cfg->load();
    Services->setConfiguration(cfg);

    auto errs = std::make_shared<ErrorsService>();
    Services->setErrors(errs);
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
    Q_INIT_RESOURCE(syntax_data);

    KQuickSyntaxHighlightingPlugin syntaxHighlightingPlugin;
    syntaxHighlightingPlugin.registerTypes("org.kde.syntaxhighlighting");

    registerTypes();
    qmlRegisterType<formats::protobuf::ProtobufMessagesModel>("plumber",
                                                              1,
                                                              0,
                                                              "ProtobufMessagesModel");

    QCoreApplication::setApplicationName("plumber");
    QCoreApplication::setOrganizationName("plumber");

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

    engine.rootContext()->setContextProperty("errorService", &Services->errors());
    QJSEngine::setObjectOwnership(&Services->errors(), QJSEngine::CppOwnership);

    engine.load(url);

    return QApplication::exec();
}
