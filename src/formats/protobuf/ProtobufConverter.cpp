#include <QtCore/QFileInfo>

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor_database.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/util/json_util.h>

#include "ProtobufConverter.h"
#include "ProtobufErrorCollector.h"
#include "ProtobufSourceTree.h"

namespace {
QByteArray errParse("\"failed parse message\"");
}

namespace formats::protobuf {

QByteArray ProtobufConverter::toJSON(QByteArray &&binary)
{
    using namespace google::protobuf::util;

    m_message->Clear();
    if (!m_message->ParseFromArray(binary.data(), binary.size())) {
        return errParse;
    }

    static JsonPrintOptions opt;
    std::string json;
    MessageToJsonString(*m_message, &json, opt);
    return QByteArray(json.c_str(), json.size());
}

QByteArray ProtobufConverter::fromJSON(QByteArray &&json)
{
    return std::move(json);
}

std::unique_ptr<core::AbstractConverter> ProtobufConverter::fabric(const QUrl &file,
                                                                   const QString &message,
                                                                   QStringList &errors)
{
    QFileInfo info(file.path());

    using namespace google::protobuf;
    using namespace google::protobuf::compiler;

    auto collector = std::make_unique<ProtobufErrorCollector>();
    auto sources = std::make_unique<ProtobufSourceTree>();

    sources->MapPath("", info.path().toStdString());

    auto database
        = std::make_unique<google::protobuf::compiler::SourceTreeDescriptorDatabase>(sources.get(),
                                                                                     nullptr);
    database->RecordErrorsTo(collector.get());

    auto pool = std::make_unique<DescriptorPool>(database.get(),
                                                 database->GetValidationErrorCollector());
    pool->EnforceWeakDependencies(true);

    const auto *const fileDescriptor = pool->FindFileByName(info.fileName().toStdString());
    if (collector->hasErrors()) {
        errors << collector->errors();
        return nullptr;
    }
    if (fileDescriptor == nullptr) {
        errors << "failed parse file";
        return nullptr;
    }
    const auto package = fileDescriptor->package();
    const auto messageType = package + "." + message.toStdString();

    const auto *const typeDescriptor = pool->FindMessageTypeByName(messageType);
    if (collector->hasErrors()) {
        errors << collector->errors();
        return nullptr;
    }
    if (typeDescriptor == nullptr) {
        errors << QString("can't find type %1").arg(message);
        return nullptr;
    }

    auto factory = std::make_unique<DynamicMessageFactory>(pool.get());
    auto *dynamicMessage = factory->GetPrototype(typeDescriptor)->New();

    auto converter = std::make_unique<ProtobufConverter>();
    converter->m_collector.swap(collector);
    converter->m_sources.swap(sources);
    converter->m_database.swap(database);
    converter->m_pool.swap(pool);
    converter->m_factory.swap(factory);
    converter->m_message.reset(dynamicMessage);
    return converter;
};

} // namespace formats::protobuf