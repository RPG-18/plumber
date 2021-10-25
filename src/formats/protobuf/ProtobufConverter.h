#pragma once

#include "core/AbstractConverter.h"
#include <memory>

namespace google {
namespace protobuf {
class Message;
class DescriptorPool;
class DynamicMessageFactory;

} // namespace protobuf
} // namespace google

namespace formats {
namespace protobuf {

/**!
 * ProtobufConverter proto2json
 */
class ProtobufConverter final : public core::AbstractConverter
{
public:
    virtual ~ProtobufConverter() = default;

    QByteArray toJSON(QByteArray &&binary) override;

    /**!
     * from json
     * @param json message
     * @return binary data
     */
    QByteArray fromJSON(QByteArray &&json) override;

private:
    std::unique_ptr<google::protobuf::DescriptorPool> m_pool;
    std::unique_ptr<google::protobuf::DynamicMessageFactory> m_factory;
    std::unique_ptr<google::protobuf::Message> m_message;
};

} // namespace protobuf
} // namespace formats
