#include <spdlog/spdlog.h>

#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "ConfigurationService.h"
#include <QtCore/QStandardPaths>

const QString ConfigurationService::ConfigName("config.json");

bool ConfigurationService::load()
{
    const auto path = QStandardPaths::locate(QStandardPaths::AppConfigLocation, ConfigName);
    if (path.isEmpty()) {
        createDirs();
        return true;
    }

    return m_cfg.loadFromFile(path);
}

bool ConfigurationService::save()
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    const QDir dir(path);
    return m_cfg.saveToFile(dir.filePath(ConfigName));
    ;
}

void ConfigurationService::createDirs()
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir(path);
    if (dir.exists()) {
        return;
    }

    if (!dir.mkdir(path)) {
        spdlog::error("directory is exist: {}", path.toStdString());
    }
}

void ConfigurationService::append(const ClusterConfig &broker)
{
    m_cfg.append(broker);
    save();
    emit configurationChanged();
}

void ConfigurationService::remove(int index)
{
    m_cfg.remove(index);
    save();
    emit configurationChanged();
}

Configuration::ClusterList ConfigurationService::brokers() const
{
    return m_cfg.brokers();
};
