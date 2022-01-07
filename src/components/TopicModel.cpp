#include "TopicModel.h"
#include "AdminClient.hpp"
#include "KafkaAdmin.h"
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

bool TopicModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::CheckStateRole) {
        return false;
    }
    checked(index, value.toBool());
    emit dataChanged(index, index, {Qt::CheckStateRole});
    return true;
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
        core::KafkaAdmin admin(config);
        core::KafkaAdmin::Topics topics;
        core::KafkaAdmin::Error err;
        std::tie(topics, err) = admin.listTopics();

        if (!topics.isEmpty()) {
            QMetaObject::invokeMethod(this,
                                      "received",
                                      Qt::QueuedConnection,
                                      Q_ARG(QVector<QString>, topics));
        }
    });
    t.detach();
}
void TopicModel::refresh()
{
    beginResetModel();
    m_topics.clear();
    m_selected.clear();
    m_selectedTopics.clear();
    loadTopics();
    endResetModel();
    emit selectedChanged();
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
    if (state) {
        m_selectedTopics.insert(m_topics[row]);
    } else {
        m_selectedTopics.remove(m_topics[row]);
    }

    changePersistentIndex(index, index);
    emit selectedChanged();
    emit dataChanged(index, index, {TopicModel::Selected});
}

int TopicModel::selected() const
{
    return m_selectedTopics.size();
}

ErrorWrap TopicModel::removeSelectedTopics()
{
    using namespace kafka::clients::admin;

    core::KafkaAdmin admin(m_config);
    core::KafkaAdmin::Topics topics(m_selectedTopics.cbegin(), m_selectedTopics.cend());
    if (auto err = admin.deleteTopics(topics)) {
        return ErrorWrap{err->where, err->what};
    }

    refresh();
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
    emit hideChanged();
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

void HidePrivateTopicModel::checked(int row, bool state)
{
    const auto itemIndex = index(row, 0, QModelIndex());
    auto sourceIndex = mapToSource(itemIndex);
    if (!sourceIndex.isValid()) {
        spdlog::error("HidePrivateTopicModel::checked invalid source index");
        return;
    }
    sourceModel()->setData(sourceIndex, state, Qt::CheckStateRole);
}
