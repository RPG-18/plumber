#pragma once

#include <QtCore/QObject>

#include "Configuration.h"

class ConfigurationService : public QObject
{
    Q_OBJECT

public:
    static const QString ConfigName;

    ConfigurationService() = default;

    /**!
   * read configuration from disk
   * config location: QStandardPaths::AppConfigLocation/config.json
   */
    bool load();

    /**!
   * read configuration from disk
   * config location: QStandardPaths::AppConfigLocation/config.json
   */
    bool save();

    /**!
   * append broker
   * @param broker
   */
    void append(const ClusterConfig &broker);

    /**!
   * remove broker by inddex
   * @param index
   */
    void remove(int index);

    Configuration::ClusterList brokers() const;

signals:

    void configurationChanged();

private:
    static void createDirs();

private:
    Configuration m_cfg;
};
