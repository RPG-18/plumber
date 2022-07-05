#include "ConsumerRecordsExporter.h"

namespace core {

AbstractConsumerExporter::AbstractConsumerExporter(const QString &name)
    : m_file(name)
    , m_useTimestamp(true)
    , m_usePartition(true)
    , m_useOffset(true)
    , m_useKey(true)
    , m_useValue(true)
{}

AbstractConsumerExporter::~AbstractConsumerExporter()
{
    closeFile();
}

void AbstractConsumerExporter::appendHeader()
{
    writeHeader(m_stream);
}

void AbstractConsumerExporter::writeHeader(QTextStream &stream)
{
    Q_UNUSED(stream);
}

void AbstractConsumerExporter::writeRecord(core::ConsumerRecord *record)
{
    if (!m_file.isOpen()) {
        return;
    }

    write(m_stream, record);
}

void AbstractConsumerExporter::closeFile()
{
    if (!m_file.isOpen()) {
        return;
    }
    m_stream.flush();
    m_file.close();
}

bool AbstractConsumerExporter::open()
{
    if (!m_file.open(QIODevice::Text | QIODeviceBase::Truncate | QIODeviceBase::WriteOnly)) {
        return false;
    }
    m_stream.setDevice(&m_file);

    writeHeader(m_stream);
    return true;
}

QString AbstractConsumerExporter::error()
{
    return m_file.errorString();
}

void AbstractConsumerExporter::setUseTimestamp(bool use)
{
    m_useTimestamp = use;
}

void AbstractConsumerExporter::setUsePartition(bool use)
{
    m_usePartition = use;
}

void AbstractConsumerExporter::setUseOffset(bool use)
{
    m_useOffset = use;
}

void AbstractConsumerExporter::setUseKey(bool use)
{
    m_useKey = use;
}

void AbstractConsumerExporter::setUseValue(bool use)
{
    m_useValue = use;
}
} // namespace core