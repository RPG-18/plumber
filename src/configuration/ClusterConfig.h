#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>

namespace kafka {
class Properties;
}
/**
 * DTO for save on disk
 */
struct ClusterConfig
{
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString color MEMBER color)
    Q_PROPERTY(QString bootstrap MEMBER bootstrap)

public:
    explicit ClusterConfig();

    QString name;
    QString color;
    QString bootstrap;

    std::shared_ptr<kafka::Properties> properties;
};

Q_DECLARE_METATYPE(ClusterConfig)
