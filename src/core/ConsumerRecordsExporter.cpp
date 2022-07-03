#include "ConsumerRecordsExporter.h"

namespace core {

AbstractConsumerRecordsExporter::AbstractConsumerRecordsExporter(const QString &name)
    : m_file(name)
    , m_useTimestamp(true)
    , m_usePartition(true)
    , m_useOffset(true)
    , m_useKey(true)
    , m_useValue(true)
{}

AbstractConsumerRecordsExporter::~AbstractConsumerRecordsExporter()
{
    closeFile();
}

void AbstractConsumerRecordsExporter::appendHeader()
{
    writeHeader(m_stream);
}

void AbstractConsumerRecordsExporter::writeHeader(QTextStream &stream)
{
    Q_UNUSED(stream);
}

void AbstractConsumerRecordsExporter::writeRecord(core::ConsumerRecord *record)
{
    if (!m_file.isOpen()) {
        return;
    }

    write(m_stream, record);
}

void AbstractConsumerRecordsExporter::closeFile()
{
    if (!m_file.isOpen()) {
        return;
    }
    m_stream.flush();
    m_file.close();
}

bool AbstractConsumerRecordsExporter::open()
{
    if (!m_file.open(QIODevice::Text | QIODeviceBase::Truncate | QIODeviceBase::WriteOnly)) {
        return false;
    }
    m_stream.setDevice(&m_file);

    writeHeader(m_stream);
    return true;
}

QString AbstractConsumerRecordsExporter::error()
{
    return m_file.errorString();
}

void AbstractConsumerRecordsExporter::setUseTimestamp(bool use)
{
    m_useTimestamp = use;
}

void AbstractConsumerRecordsExporter::setUsePartition(bool use)
{
    m_usePartition = use;
}

void AbstractConsumerRecordsExporter::setUseOffset(bool use)
{
    m_useOffset = use;
}

void AbstractConsumerRecordsExporter::setUseKey(bool use)
{
    m_useKey = use;
}

void AbstractConsumerRecordsExporter::setUseValue(bool use)
{
    m_useValue = use;
}
} // namespace core