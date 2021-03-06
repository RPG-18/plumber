#include "ConfigModel.h"
#include "Registry.h"
#include "spdlog/spdlog.h"

#include <kafka/Properties.h>

#include <QtCore/QSignalBlocker>
#include <QtCore/QTextStream>

ConfigModel::ConfigModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_service(Services->configuration())
{
    QObject::connect(m_service,
                     &ConfigurationService::configurationChanged,
                     this,
                     &ConfigModel::configurationIsChanged);
    m_brokers = m_service->brokers();
}

int ConfigModel::rowCount(const QModelIndex & /*parent*/) const
{
    return int(m_brokers.size());
}

QVariant ConfigModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_brokers.size()) {
        return {};
    }

    switch (role) {
    case Name:
        return m_brokers[index.row()].name;
    case Color:
        return m_brokers[index.row()].color;
    case Bootstrap:
        return m_brokers[index.row()].bootstrap;
    default:
        return {};
    }
}

QHash<int, QByteArray> ConfigModel::roleNames() const
{
    static const QHash<int, QByteArray> data{{Name, "name"},
                                             {Color, "bgcolor"},
                                             {Bootstrap, "bootstrap"}};
    return data;
}

void ConfigModel::configurationIsChanged()
{
    beginResetModel();
    m_brokers = m_service->brokers();
    endResetModel();
}

void ConfigModel::createBroker(const QString &name,
                               const QString &bootstrap,
                               const QString &color,
                               const QString &properties)
{
    qDebug() << "PROPS" << properties;
    QSignalBlocker lock(m_service);

    ClusterConfig broker;
    broker.name = name;
    broker.bootstrap = bootstrap;
    broker.color = color;

    QString props = properties;
    QTextStream stream(&props, QIODeviceBase::ReadOnly);
    stream.skipWhiteSpace();

    const QChar sep('=');
    while (!stream.atEnd()) {
        const auto line = stream.readLine();
        const auto kv = line.split(sep);
        if (kv.length() != 2) {
            spdlog::warn("invalid properties string {}", line.toStdString());
            continue;
        }
        const auto key = kv[0].simplified().toStdString();
        const auto value = kv[1].simplified().toStdString();
        broker.properties->put(key, value);
    }

    m_service->append(broker);

    beginInsertRows(QModelIndex(), int(m_brokers.size()), int(m_brokers.size()));
    m_brokers.push_back(broker);
    endInsertRows();
}

void ConfigModel::removeBroker(int index)
{
    QSignalBlocker lock(m_service);

    m_service->remove(index);

    beginRemoveRows(QModelIndex(), index, index);
    m_brokers.remove(index, 1);
    endRemoveRows();
}

QVariant ConfigModel::broker(int index)
{
    if (m_brokers.size() < index) {
        return QVariant{};
    }
    return QVariant::fromValue(m_brokers[index]);
}
