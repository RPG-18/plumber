#pragma once

#include "Consumer.h"

/**!
 * Simple key/value types list model
  */
class ConsumerTypesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles { TypeId = Qt::UserRole + 1, TypeName };

    ConsumerTypesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Types> m_typeIds;
    QVector<QString> m_typeNames;
};

/**!
 * Time Base Model
 */
class StartFromTimeBasedModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles { StartTimeId = Qt::UserRole + 1, StartTimeLabel };

    StartFromTimeBasedModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Consumer::StartFromTimeBased> m_times;
    QVector<QString> m_labels;
};

/**!
 * Filters Model
 */
class FiltersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles { FilterId = Qt::UserRole + 1, FilterLabel };

    FiltersModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Consumer::Filters> m_filters;
    QVector<QString> m_labels;
};

/**!
 * Limit Model
 */
class LimitModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles { LimitId = Qt::UserRole + 1, LimitLabel };

    LimitModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Consumer::Limit> m_limits;
    QVector<QString> m_labels;
};