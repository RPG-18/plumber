#pragma once

#include <thread>

#include <QtCore/QAbstractListModel>
#include <QtCore/QVector>

#include "ClusterConfig.h"
#include "kafka/BrokerMetadata.h"

/**!
 * Show brokers
 */
class BrokerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int brokers READ brokers NOTIFY brokersChanged)

public:
    enum Roles { Node = Qt::UserRole + 1 };

    explicit BrokerModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int brokers() const noexcept;
    void setConfig(const ClusterConfig &broker);

signals:

    void brokersChanged();

private slots:

    void received();

private:
    void loadMetaData();

private:
    ClusterConfig m_config;
    QVector<QString> m_nodes;
    std::shared_ptr<kafka::BrokerMetadata> m_md;
};
