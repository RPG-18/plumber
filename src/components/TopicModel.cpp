#include <thread>
#include <QtCore/QDebug>

#include "AdminClient.hpp"
#include "spdlog/spdlog.h"

#include "TopicModel.h"

using namespace kafka;

TopicModel::TopicModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int TopicModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_topics.size();
}

QVariant TopicModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    const auto row = index.row();
    switch (role) {
    case Topic:
        return m_topics[row];
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TopicModel::roleNames() const
{
    static QHash<int, QByteArray> roles{{Topic, "topic"}};
    return roles;
}

void TopicModel::setConfig(const ClusterConfig &broker)
{
    m_config = broker;
    loadTopics();
}

int TopicModel::topics() const noexcept
{
    return m_topics.size();
}

void TopicModel::loadTopics()
{
    std::thread t([this, config = m_config]() {
        try {
            AdminClientConfig cfg(config.properties->map());
            cfg.put(AdminClientConfig::BOOTSTRAP_SERVERS, config.bootstrap.toStdString());

            core::AdminClient client(cfg);

            const auto response = client.listTopics();
            if (!response.errorCode()) {
                QVector<QString> topics;
                topics.reserve(response.topics.size());
                for (const auto &topic : response.topics) {
                    topics.emplaceBack(QString::fromStdString(topic));
                }
                QMetaObject::invokeMethod(this,
                                          "received",
                                          Qt::QueuedConnection,
                                          Q_ARG(QVector<QString>, topics));
            } else {
                spdlog::error("list topics {}", response.message());
            }
        } catch (const kafka::KafkaException &e) {
            spdlog::error("Unexpected exception caught: {}", e.what());
        }
    });
    t.detach();
}

void TopicModel::received(QVector<QString> topics)
{
    beginResetModel();
    m_topics.swap(topics);
    endResetModel();
    emit topicsChanged();
}

TopicFilterModel::TopicFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterRole(TopicModel::Topic);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void TopicFilterModel::setModel(TopicModel *model)
{
    setSourceModel(model);
}
TopicModel *TopicFilterModel::model() const
{
    return dynamic_cast<TopicModel *>(sourceModel());
}

QString TopicFilterModel::filter() const
{
    return m_filter;
}

void TopicFilterModel::setFilter(const QString &topic)
{
    m_filter = topic;
    setFilterFixedString(m_filter);
}