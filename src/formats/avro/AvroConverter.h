#pragma once

#include <QtCore/QUrl>

#include "core/AbstractConverter.h"
#include <memory>

namespace avro {
class Encoder;
class Decoder;
class ValidSchema;
class InputStream;
} // namespace avro

namespace formats {
namespace avro {

/**!
 * AvroConverter avro2json
 */
class AvroConverter final : public core::AbstractConverter
{
public:
    ~AvroConverter() = default;

    /**!
     * to json
     * @param binary
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
     * @param filePath path to Avro schema file
     * @param errors [out] errors
     * @return
     */
    static std::unique_ptr<core::AbstractConverter> fabric(const QUrl &filePath,
                                                           QStringList &errors);

private:
    std::unique_ptr<::avro::ValidSchema> m_schema;
    std::shared_ptr<::avro::Encoder> m_binaryEncode;
    std::shared_ptr<::avro::Decoder> m_binaryDecoder;
    std::shared_ptr<::avro::Encoder> m_jsonEncoder;
    std::shared_ptr<::avro::Decoder> m_jsonDecoder;
    std::unique_ptr<::avro::InputStream> m_tempInputStream;
};
} // namespace avro
} // namespace formats