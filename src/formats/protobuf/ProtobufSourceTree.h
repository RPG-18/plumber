#pragma once

#include <google/protobuf/compiler/importer.h>

namespace formats {
namespace protobuf {

/**!
 * ProtobufSourceTree source tree
 */
class ProtobufSourceTree final : public google::protobuf::compiler::DiskSourceTree
{
public:
    google::protobuf::io::ZeroCopyInputStream *Open(const std::string &filename) override;

private:
    static google::protobuf::io::ZeroCopyInputStream *openFromResources(const std::string &filename);
};

} // namespace protobuf
} // namespace formats