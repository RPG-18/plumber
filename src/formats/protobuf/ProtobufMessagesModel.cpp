#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/descriptor.h>

#include <QtCore/QFileInfo>

#include "ProtobufErrorCollector.h"
#include "ProtobufMessagesModel.h"
#include "ProtobufSourceTree.h"

namespace formats::protobuf {

ProtobufMessagesModel::ProtobufMessagesModel(QObject *parent)
    : QAbstractListModel(parent)
{}

void ProtobufMessagesModel::setFile(const QUrl &file)
{
    m_file = file;
    emit fileChanged();
    if (m_file.isValid()) {
        loadMessages();
    }
}

QUrl ProtobufMessagesModel::file() const
{
    return m_file;
}

void ProtobufMessagesModel::loadMessages()
{
    using namespace google::protobuf;
    using namespace google::protobuf::compiler;

    QFileInfo info(m_file.path());

    ProtobufErrorCollector errors;
    ProtobufSourceTree sources;
    sources.MapPath("", info.path().toStdString());

    SourceTreeDescriptorDatabase database(&sources, nullptr);
    database.RecordErrorsTo(&errors);

    DescriptorPool pool(&database, database.GetValidationErrorCollector());
    pool.EnforceWeakDependencies(true);

    const auto *const fileDescriptor = pool.FindFileByName(info.fileName().toStdString());
    if (errors.hasErrors()) {
        emit parseErrors(errors.errors().join('\n'));
        return;
    }

    if (fileDescriptor == nullptr) {
        return;
    }

    beginResetModel();
    m_messages.clear();
    for (int i = 0; i < fileDescriptor->message_type_count(); i++) {
        m_messages << QString::fromStdString(fileDescriptor->message_type(i)->name());
    }
    endResetModel();
    if (!m_messages.isEmpty()) {
        emit loaded();
    }
}

int ProtobufMessagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_messages.size();
}

QVariant ProtobufMessagesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const auto row = index.row();
    if (role == Message) {
        return m_messages[row];
    }
    return {};
}

QHash<int, QByteArray> ProtobufMessagesModel::roleNames() const
{
    static QHash<int, QByteArray> roles{{Message, "message"}};
    return roles;
}

} // namespace formats::protobuf
