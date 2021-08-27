#pragma once

#include <QtCore/QAbstractListModel>
#include <QtCore/QSet>
#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QVector>

#include "ClusterConfig.h"
#include "ErrorWrap.h"

/**!
 * Show topics
 */
class TopicModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int topics READ topics NOTIFY topicsChanged)
    Q_PROPERTY(int selected READ selected NOTIFY selectedChanged)

public:
    enum Roles { Topic = Qt::UserRole + 1, Selected };
    explicit TopicModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int topics() const noexcept;
    void setConfig(const ClusterConfig &broker);

    void checked(const QModelIndex &index, bool state);

    int selected() const;

    Q_INVOKABLE QStringList selectedTopics() const;
    Q_INVOKABLE void refresh();
    Q_INVOKABLE ErrorWrap removeSelectedTopics();

signals:

    void topicsChanged();
    void selectedChanged();

private slots:

    void received(QVector<QString> topics);

private:
    void loadTopics();

private:
    ClusterConfig m_config;
    QVector<QString> m_topics;
    QVector<bool> m_selected;
    QSet<QString> m_selectedTopics;
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

    Q_INVOKABLE void checked(int row, bool state);

private:
    QString m_filter;
};