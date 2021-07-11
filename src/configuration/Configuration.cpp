#include <kafka/Properties.h>
#include <spdlog/spdlog.h>

#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include "Configuration.h"

namespace {

const QString BOOTSTRAP("bootstrap");
const QString NAME("name");
const QString COLOR("color");
const QString PROPERTIES("properties");
const QString BROKERS("brokers");

QJsonObject toJson(const ClusterConfig &cfg) noexcept
{
    QJsonObject obj;

    obj[NAME] = cfg.name;
    obj[BOOTSTRAP] = cfg.bootstrap;
    obj[COLOR] = cfg.color;

    {
        QJsonObject opt;
        for (const auto &[k, v] : cfg.properties->map()) {
            opt[k.c_str()] = v.c_str();
        }
        obj[PROPERTIES] = opt;
    }

    return obj;
}

//todo std::optional for errors
ClusterConfig fromJson(const QJsonObject &obj) noexcept
{
    ClusterConfig cfg;

    cfg.name = obj[NAME].toString();
    cfg.bootstrap = obj[BOOTSTRAP].toString();
    cfg.color = obj[COLOR].toString();
    const auto properties = obj[PROPERTIES].toObject();
    for (auto iter = properties.constBegin(); iter != properties.constEnd(); iter++) {
        const auto key = iter.key();
        const auto value = iter.value().toString();
        cfg.properties->put(key.toStdString(), value.toStdString());
    }

    return cfg;
}

} // namespace

Configuration::Configuration() {}

bool Configuration::loadFromFile(const QString &path) noexcept
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        spdlog::error("open config file {}: {}",
                      path.toStdString(),
                      file.errorString().toStdString());
        return false;
    }

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(file.readAll(), &err);
    if (err.error != QJsonParseError::ParseError::NoError) {
        spdlog::error("parse configuration from json file {}: {}",
                      path.toStdString(),
                      err.errorString().toStdString());
        return false;
    }

    if (!doc.isObject()) {
        spdlog::error("configuration is not json object");
        return false;
    }

    auto obj = doc.object();
    if (!obj[BROKERS].isArray()) {
        spdlog::error("invalid section brokers");
        return false;
    }
    const auto list = obj[BROKERS].toArray();
    m_brokers.reserve(list.size());
    for (const auto broker : list) {
        if (!broker.isObject()) {
            spdlog::error("read configuration, expected json object");
            return false;
        }
        const auto b = fromJson(broker.toObject());
        m_brokers.push_back(b);
    }

    return true;
}

bool Configuration::saveToFile(const QString &path) noexcept
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        spdlog::error("open configuration file {}: {}",
                      path.toStdString(),
                      file.errorString().toStdString());
        return false;
    }

    QJsonObject obj;
    QJsonArray brokersArr;
    for (const auto &b : m_brokers) {
        brokersArr.push_back(toJson(b));
    }
    obj[BROKERS] = brokersArr;

    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    if (file.error() != QFileDevice::FileError::NoError) {
        spdlog::error("write configuration to file {}: {}",
                      path.toStdString(),
                      file.errorString().toStdString());
        return false;
    }

    return true;
}

void Configuration::append(const ClusterConfig &broker)
{
    m_brokers.push_back(broker);
}

void Configuration::remove(int index)
{
    m_brokers.remove(index, 1);
}

Configuration::ClusterList Configuration::brokers() const
{
    return m_brokers;
}
