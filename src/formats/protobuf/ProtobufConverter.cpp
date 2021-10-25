#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/util/json_util.h>

#include "ProtobufConverter.h"

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
} // namespace formats::protobuf
