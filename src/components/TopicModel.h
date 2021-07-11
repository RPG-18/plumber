#pragma once

#include <QtCore/QAbstractListModel>
#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QVector>

#include "ClusterConfig.h"

/**!
 * Show topics
 */
class TopicModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int topics READ topics NOTIFY topicsChanged)

public:
    enum Roles { Topic = Qt::UserRole + 1 };
    explicit TopicModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int topics() const noexcept;
    void setConfig(const ClusterConfig &broker);

signals:

    void topicsChanged();

private slots:

    void received(QVector<QString> topics);

private:
    void loadTopics();

private:
    ClusterConfig m_config;
    QVector<QString> m_topics;
};

/**!
 * filter topics by name
 */
class TopicFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(TopicModel *model READ model WRITE setModel)
    Q_PROPERTY(QString filter READ filter WRITE setFilter)

public:
    TopicFilterModel(QObject *parent = nullptr);

    void setModel(TopicModel *model);
    TopicModel *model() const;

    QString filter() const;
    void setFilter(const QString &topic);

private:
    QString m_filter;
};