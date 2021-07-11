#pragma once

#include <QtCore/QString>
#include <QtCore/QVector>

#include "ClusterConfig.h"

/**
 * Configuration container for settings
 */
class Configuration
{
public:
    using ClusterList = QVector<ClusterConfig>;

    explicit Configuration();

    /**
   * load settings from file
   * @param path path to file
   * @return true if no errors
   */
    bool loadFromFile(const QString &path) noexcept;

    /**!
   * save settings to file
   * @param path path to file
   * @return true if no errors
   */
    bool saveToFile(const QString &path) noexcept;

    /**!
   * append broker
   * @param broker
   */
    void append(const ClusterConfig &broker);

    /**!
   * remove broker
   * @param index item position
   */
    void remove(int index);

    /**!
   * get brockers
   * @return  brokers
   */
    ClusterList brokers() const;

private:
    ClusterList m_brokers;
};
