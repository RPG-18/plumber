#pragma once

#include <QtCore/QAbstractListModel>

#include "Configuration.h"

class ConfigurationService;

class ConfigModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles { Name = Qt::UserRole + 1, Color, Bootstrap };

    explicit ConfigModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void createBroker(QString name,
                                  QString bootstrap,
                                  QString color,
                                  QString properties);
    Q_INVOKABLE void removeBroker(int index);
    Q_INVOKABLE QVariant broker(int index);

public slots:

    void configurationIsChanged();

private:
    ConfigurationService *m_service;
    Configuration::ClusterList m_brokers;
};
