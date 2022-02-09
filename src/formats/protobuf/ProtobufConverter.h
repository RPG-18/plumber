#pragma once

#include <QtCore/QUrl>

#include "core/AbstractConverter.h"
#include <memory>

namespace google {
namespace protobuf {
class Message;
class DescriptorPool;
class DynamicMessageFactory;
namespace compiler {
class SourceTreeDescriptorDatabase;
}

} // namespace protobuf
} // namespace google

namespace formats {
namespace protobuf {

class ProtobufErrorCollector;
class ProtobufSourceTree;

/**!
 * ProtobufConverter proto2json
 */
class ProtobufConverter final : public core::AbstractConverter
{
public:
    virtual ~ProtobufConverter() = default;

    /**!
     * to json
     * @param binary message
     * @return json data
     */
    QByteArray toJSON(QByteArray &&binary) override;

    /**!
     * from json
     * @param json message
     * @return binary data
     */
    std::tuple<QByteArray, core::Error> fromJSON(QByteArray &&json) override;

    /**!
     * fabric
     * @param file path to proto file
     * @param message proto message
     * @param out errors return errors
     * @return
     */
    static std::unique_ptr<core::AbstractConverter> fabric(const QUrl &file,
                                                           const QString &message,
                                                           QStringList &errors);

private:
    std::unique_ptr<ProtobufErrorCollector> m_collector;
    std::unique_ptr<ProtobufSourceTree> m_sources;
    std::unique_ptr<google::protobuf::compiler::SourceTreeDescriptorDatabase> m_database;
    std::unique_ptr<google::protobuf::DescriptorPool> m_pool;
    std::unique_ptr<google::protobuf::DynamicMessageFactory> m_factory;
    std::unique_ptr<google::protobuf::Message> m_message;
};

} // namespace protobuf
} // namespace formats
