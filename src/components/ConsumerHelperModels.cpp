#include "ConsumerHelperModels.h"

ConsumerTypesModel::ConsumerTypesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_typeIds << Types::JSON << Types::String << Types::Base64 << Types::Float << Types::Double
              << Types::Long << Types::NoneType;

    m_typeNames << "JSON"
                << "string"
                << "bytes (base64)"
                << "float"
                << "double"
                << "long"
                << "none (ignore)";
}

QHash<int, QByteArray> ConsumerTypesModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{TypeId, "typeId"}, {TypeName, "typeName"}};
    return roles;
}

int ConsumerTypesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return int(m_typeIds.size());
}

QVariant ConsumerTypesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    const auto row = index.row();
    switch (role) {
    case TypeId:
        return m_typeIds[row];
    case TypeName:
        return m_typeNames[row];
    default:
        return {};
    }
}

StartFromTimeBasedModel::StartFromTimeBasedModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_times << Consumer::StartFromTimeBased::Now << Consumer::StartFromTimeBased::LastHour
            << Consumer::StartFromTimeBased::Today << Consumer::StartFromTimeBased::Yesterday
            << Consumer::StartFromTimeBased::SpecificDate << Consumer::StartFromTimeBased::Earliest;

    m_labels << "Now (latest)"
             << "Last Hour"
             << "Today"
             << "Yesterday"
             << "A specific date"
             << "The beginning (earliest)";
}

int StartFromTimeBasedModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return int(m_times.size());
}

QVariant StartFromTimeBasedModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    const auto row = index.row();
    switch (role) {
    case StartTimeId:
        return m_times[row];
    case StartTimeLabel:
        return m_labels[row];
    default:
        return {};
    }
}
QHash<int, QByteArray> StartFromTimeBasedModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{StartTimeId, "startTimeId"},
                                              {StartTimeLabel, "startTimeLabel"}};
    return roles;
}

FiltersModel::FiltersModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_filters << Consumer::Filters::NoneFilter << Consumer::Filters::Contains
              << Consumer::Filters::NotContains << Consumer::Filters::Equals;

    m_labels << "(none)"
             << "Contains"
             << "Does NOT contain"
             << "Equals";
}

int FiltersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return int(m_filters.size());
}

QVariant FiltersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    const auto row = index.row();
    switch (role) {
    case FilterId:
        return m_filters[row];
    case FilterLabel:
        return m_labels[row];
    default:
        return {};
    }
}

QHash<int, QByteArray> FiltersModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{FilterId, "filterId"}, {FilterLabel, "filterLabel"}};
    return roles;
}

LimitModel::LimitModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_limits << Consumer::Limit::NoneLimit << Consumer::Limit::NumbersOfRecords
             << Consumer::Limit::SpecificDateLimit << Consumer::Limit::MaxSizeBytes;

    m_labels << "None (forever)"
             << "Number of records"
             << "A specific date"
             << "Max Size (bytes)";
}

int LimitModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return int(m_limits.size());
}

QVariant LimitModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    const auto row = index.row();
    switch (role) {
    case LimitId:
        return m_limits[row];
    case LimitLabel:
        return m_labels[row];
    default:
        return {};
    }
}

QHash<int, QByteArray> LimitModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{LimitId, "limitId"}, {LimitLabel, "limitLabel"}};
    return roles;
}

CustomTypesModel::CustomTypesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_typeIds << Types::Protobuf << Types::Avro;
    m_typeNames << "Protobuf"
                << "Avro";
}

int CustomTypesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return int(m_typeIds.size());
}

QVariant CustomTypesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    const auto row = index.row();
    switch (role) {
    case TypeId:
        return m_typeIds[row];
    case TypeName:
        return m_typeNames[row];
    default:
        return {};
    }
}
QHash<int, QByteArray> CustomTypesModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{TypeId, "typeId"}, {TypeName, "typeName"}};
    return roles;
}
