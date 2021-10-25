#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QSet>

#include <google/protobuf/io/zero_copy_stream.h>

#include "ProtobufSourceTree.h"
#include "spdlog/spdlog.h"

namespace {
class ByteArrayInputStream final : public google::protobuf::io::ArrayInputStream
{
public:
    explicit ByteArrayInputStream(QByteArray &&data)
        : ArrayInputStream(data.data(), data.size())
        , m_data(std::move(data))
    {}

private:
    QByteArray m_data;
};

} // namespace

namespace formats::protobuf {

google::protobuf::io::ZeroCopyInputStream *ProtobufSourceTree::Open(const std::string &filename)
{
    static QSet<std::string> inResources = {"google/protobuf/any.proto",
                                            "google/protobuf/api.proto",
                                            "google/protobuf/descriptor.proto",
                                            "google/protobuf/duration.proto",
                                            "google/protobuf/empty.proto",
                                            "google/protobuf/field_mask.proto",
                                            "google/protobuf/source_context.proto",
                                            "google/protobuf/struct.proto",
                                            "google/protobuf/timestamp.proto",
                                            "google/protobuf/type.proto",
                                            "google/protobuf/wrappers.proto"};

    if (inResources.contains(filename)) {
        return openFromResources(filename);
    }

    return DiskSourceTree::Open(filename);
}

google::protobuf::io::ZeroCopyInputStream *ProtobufSourceTree::openFromResources(
    const std::string &filename)
{
    using namespace google::protobuf::io;

    QString path(QString(":/%1").arg(QString::fromStdString(filename)));
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        spdlog::error("failed open file {} from resources error {}",
                      path.toStdString(),
                      file.errorString().toStdString());
        return nullptr;
    }
    auto data = file.readAll();
    file.close();

    return new ByteArrayInputStream(std::move(data));
}

} // namespace formats::protobuf
