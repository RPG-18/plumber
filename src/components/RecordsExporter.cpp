#include "rapidjson/writer.h"

#include "RecordsExporter.h"

namespace {
class QTextStreamWrapper
{
public:
    typedef char Ch;

    QTextStreamWrapper(QTextStream &stream)
        : m_stream(stream)
    {}

    void Put(Ch c) { m_stream << c; }

    void Flush() { m_stream.flush(); }

    Ch Peek() const
    {
        assert(false);
        return '\0';
    }
    Ch Take()
    {
        assert(false);
        return '\0';
    }
    size_t Tell() const { return 0; }

    Ch *PutBegin()
    {
        assert(false);
        return 0;
    }
    size_t PutEnd(Ch *)
    {
        assert(false);
        return 0;
    }

private:
    QTextStream &m_stream;
};

using JSONWriter = rapidjson::Writer<QTextStreamWrapper>;

void writeType(Types tp, JSONWriter &w, const QByteArray &data)
{
    switch (tp) {
    case JSON:
        w.RawValue(data.data(), data.size(), rapidjson::kObjectType);
        break;
    case String:
        w.String(data.data(), data.size());
        break;
    case Base64:
        w.String(data.data(), data.size());
        break;
    case Float:
    case Double: {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);

        switch (data.size()) {
        case 4: {
            float val = 0;
            stream >> val;
            w.Double(val);
        }
        case 8: {
            double val = 0;
            stream >> val;
            w.Double(val);
        }
        };
    } break;
    case Long: {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);

        switch (data.size()) {
        case 4: {
            qint32 val = 0;
            stream >> val;
            w.Int(val);
        } break;

        case 8: {
            qint64 val = 0;
            w.Int64(val);
        } break;
        }
    } break;
    case NoneType:
        w.Null();
        break;
    case Protobuf:
        w.RawValue(data.data(), data.size(), rapidjson::kObjectType);
        break;
    case Avro:
        w.RawValue(data.data(), data.size(), rapidjson::kObjectType);
        break;
    };
}

} // namespace

CSVConsumerRecordsExporter::CSVConsumerRecordsExporter(Types key, Types value, const QString &file)
    : core::AbstractConsumerExporter(file)
    , m_key(key)
    , m_value(value)
{}

void CSVConsumerRecordsExporter::writeHeader(QTextStream &stream)
{
    bool delimiter = false;
#define PRINT_DELIMITER \
    if (delimiter) { \
        stream << ";"; \
    }
    if (useTimestamp()) {
        stream << "timestamp";
        delimiter = true;
    }
    if (usePartition()) {
        PRINT_DELIMITER
        stream << "partition";
        delimiter = true;
    }

    if (useOffset()) {
        PRINT_DELIMITER
        stream << "offset";
        delimiter = true;
    }

    if (useKey()) {
        PRINT_DELIMITER
        stream << "key";
        delimiter = true;
    }

    if (useValue()) {
        PRINT_DELIMITER
        stream << "value";
    }
    stream << Qt::endl << Qt::flush;
#undef PRINT_DELIMITER
}

void CSVConsumerRecordsExporter::write(QTextStream &stream, core::ConsumerRecord *record)
{
    bool delimiter = false;
#define PRINT_DELIMITER \
    if (delimiter) { \
        stream << ";"; \
    }
    if (useTimestamp()) {
        stream << record->timestamp.msSinceEpoch;
        delimiter = true;
    }

    if (usePartition()) {
        PRINT_DELIMITER
        stream << record->partition;
        delimiter = true;
    }

    if (useOffset()) {
        PRINT_DELIMITER
        stream << record->offset;
        delimiter = true;
    }

    if (useKey()) {
        PRINT_DELIMITER
        if (m_key != NoneType) {
            stream << format(m_key, record->key);
        }
        delimiter = true;
    }

    if (useValue()) {
        PRINT_DELIMITER
        if (m_value != NoneType) {
            stream << format(m_value, record->value);
        }
    }

#undef PRINT_DELIMITER
    stream << Qt::endl << Qt::flush;
}

JSONConsumerRecordsExporter::JSONConsumerRecordsExporter(Types key, Types value, const QString &file)
    : core::AbstractConsumerExporter(file)
    , m_key(key)
    , m_value(value)
{}

void JSONConsumerRecordsExporter::writeHeader(QTextStream &stream)
{
    Q_UNUSED(stream)
}

void JSONConsumerRecordsExporter::write(QTextStream &stream, core::ConsumerRecord *record)
{
    QTextStreamWrapper s(stream);
    JSONWriter writer(s);

    writer.StartObject();
    if (useTimestamp()) {
        writer.Key("timestamp");
        writer.Int64(record->timestamp.msSinceEpoch);
    }

    if (usePartition()) {
        writer.Key("partition");
        writer.Int(record->partition);
    }

    if (useOffset()) {
        writer.Key("offset");
        writer.Int64(record->offset);
    }

    if (useKey()) {
        writer.Key("key");
        writeType(m_key, writer, record->key);
    }

    if (useValue()) {
        writer.Key("value");
        writeType(m_value, writer, record->value);
    }
    writer.EndObject();
    stream << Qt::endl << Qt::flush;
}