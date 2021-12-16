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
    using namespace kafka::clients::admin;

    std::thread t([this, config = m_config]() {
        try {
            Config cfg(config.properties->map());
            cfg.put(Config::BOOTSTRAP_SERVERS, config.bootstrap.toStdString());

            core::AdminClient client(cfg);

            const auto response = client.listTopics();
            if (!response.error) {
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
                spdlog::error("list topics {}", response.error.message());
            }
        } catch (const kafka::KafkaException &e) {
            spdlog::error("Unexpected exception caught: {}", e.what());
        }
    });
    t.detach();
}
void TopicModel::refresh()
{
    beginResetModel();
    loadTopics();
    endResetModel();
}

void TopicModel::received(QVector<QString> topics)
{
    beginResetModel();
    m_selectedTopics.clear();
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

ErrorWrap TopicModel::removeSelectedTopics()
{
    using namespace kafka::clients::admin;

    const QString when("topic remove");

    try {
        Config cfg(m_config.properties->map());
        cfg.put(Config::BOOTSTRAP_SERVERS, m_config.bootstrap.toStdString());

        kafka::clients::AdminClient client(cfg);
        Topics topics;
        for (const auto &topic : m_selectedTopics) {
            topics.insert(topic.toStdString());
        }

        auto result = client.deleteTopics(topics);
        if (result.error) {
            spdlog::error("topic remove error {}", result.error.message());
            return ErrorWrap{when, QString::fromStdString(result.error.message())};
        }
        refresh();
    } catch (const kafka::KafkaException &e) {
        spdlog::error("topic create exception {}", e.what());
        return ErrorWrap{when, QString::fromStdString(e.what())};
    }

    return ErrorWrap{};
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

HidePrivateTopicModel::HidePrivateTopicModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_hide(false)
{}

bool HidePrivateTopicModel::hide() const noexcept
{
    return m_hide;
}

void HidePrivateTopicModel::setHide(bool hide)
{
    beginResetModel();
    m_hide = hide;
    endResetModel();
}

bool HidePrivateTopicModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!m_hide) {
        return true;
    }
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QString topicName = index.data(TopicModel::Topic).toString();
    return !topicName.startsWith("__");
}
