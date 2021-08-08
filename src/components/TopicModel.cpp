#include "TopicModel.h"
#include "AdminClient.hpp"
#include "spdlog/spdlog.h"

#include <QtCore/QDebug>

#include <thread>

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
    case Selected:
        return m_selected[row];
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TopicModel::roleNames() const
{
    static QHash<int, QByteArray> roles{{Topic, "topic"}, {Selected, "selected"}};
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
    m_selected.resize(m_topics.size(), false);
    endResetModel();
    emit topicsChanged();
}

void TopicModel::checked(const QModelIndex &index, bool state)
{
    const auto row = index.row();
    m_selected[row] = state;
    changePersistentIndex(index, index);
    if (state) {
        m_selectedTopics.insert(m_topics[row]);
    } else {
        m_selectedTopics.remove(m_topics[row]);
    }
    emit selectedChanged();
}

int TopicModel::selected() const
{
    return m_selectedTopics.size();
}

QStringList TopicModel::selectedTopics() const
{
    QStringList out(m_selectedTopics.begin(), m_selectedTopics.end());
    return out;
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

void TopicFilterModel::checked(int row, bool state)
{
    const auto idx = index(row, 0);
    const auto source = mapToSource(idx);
    model()->checked(source, state);
}
