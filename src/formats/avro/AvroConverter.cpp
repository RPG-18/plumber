#include <QtCore/QDebug>
#include <QtCore/QFile>

#include <avro/Compiler.hh>
#include <avro/Decoder.hh>
#include <avro/Generic.hh>
#include <avro/Specific.hh>
#include <avro/ValidSchema.hh>

#include "AvroConverter.h"
#include "spdlog/spdlog.h"

namespace {
const QByteArray errParse("\"failed parse message\"");
}

namespace formats {
namespace avro {

QByteArray AvroConverter::toJSON(QByteArray &&binary)
{
    try {
        qDebug() << QString::fromStdString(m_schema->toJson());
        qDebug() << binary;
        auto in = ::avro::memoryInputStream(reinterpret_cast<const uint8_t *>(binary.data()),
                                            binary.size());
        m_binaryDecoder->init(*in);
        ::avro::GenericDatum datum(*m_schema);
        ::avro::decode(*m_binaryDecoder, datum);

        auto out = ::avro::memoryOutputStream();
        m_jsonEncoder->init(*out);
        ::avro::encode(*m_jsonEncoder, datum);
        m_jsonEncoder->flush();
        out->flush();

        auto copyStream = ::avro::memoryInputStream(*out);
        const uint8_t *p = nullptr;
        size_t n = 0;
        QByteArray outData;
        outData.reserve(out->byteCount());
        while (copyStream->next(&p, &n)) {
            outData.append(reinterpret_cast<const char *>(p), n);
        }
        return outData;
    } catch (const ::avro::Exception &exc) {
        spdlog::error("avro2json exception {}", exc.what());
        return errParse;
    }

    return {};
}

QByteArray AvroConverter::fromJSON(QByteArray &&json)
{
    try {
        auto in = ::avro::memoryInputStream(reinterpret_cast<uint8_t *>(json.data()), json.size());
        m_jsonDecoder->init(*in);

        ::avro::GenericDatum datum(*m_schema);
        ::avro::decode(*m_jsonDecoder, datum);

        auto out = ::avro::memoryOutputStream();
        m_binaryEncode->init(*out);
        ::avro::encode(*m_binaryEncode, datum);
        m_binaryEncode->flush();

        auto copyStream = ::avro::memoryInputStream(*out);
        const uint8_t *p = nullptr;
        size_t n = 0;
        QByteArray outData;
        outData.reserve(out->byteCount());
        while (copyStream->next(&p, &n)) {
            outData.append(reinterpret_cast<const char *>(p), int(n));
        }
        return outData;
    } catch (const ::avro::Exception &exc) {
        spdlog::error("json2avro exception {}", exc.what());
        return errParse;
    }
    return {};
}

std::unique_ptr<core::AbstractConverter> AvroConverter::fabric(const QUrl &filePath,
                                                               QStringList &errors)
{
    qDebug() << filePath;
    QFile schemaFile(filePath.path());
    if (!schemaFile.open(QIODevice::ReadOnly)) {
        errors << QString("open schema error: %1").arg(schemaFile.errorString());
        return nullptr;
    }

    auto schemaData = schemaFile.readAll();
    qDebug() << schemaData;
    try {
        auto pt = filePath.path();
        ::avro::ValidSchema schema = ::avro::compileJsonSchemaFromMemory(reinterpret_cast<uint8_t *>(
                                                                             schemaData.data()),
                                                                         schemaData.size());
        auto converter = std::make_unique<AvroConverter>();
        converter->m_schema = std::make_unique<::avro::ValidSchema>(std::move(schema));
        converter->m_binaryEncode = ::avro::binaryEncoder();
        converter->m_binaryDecoder = ::avro::binaryDecoder();

        converter->m_jsonEncoder = ::avro::jsonEncoder(*converter->m_schema);
        converter->m_jsonDecoder = ::avro::jsonDecoder(*converter->m_schema);
        return converter;
    } catch (const ::avro::Exception &exc) {
        errors << QString("parse schema error: %1").arg(exc.what());
        return nullptr;
    }

    return nullptr;
}
} // namespace avro
} // namespace formats