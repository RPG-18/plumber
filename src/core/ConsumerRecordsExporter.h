#pragma once

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "ConsumerRecord.h"

namespace core {

/**!
 * cass AbstractConsumerRecordsExporter abstract exporter
 */
class AbstractConsumerRecordsExporter
{
public:
    AbstractConsumerRecordsExporter(const QString &name);
    virtual ~AbstractConsumerRecordsExporter();

    void writeRecord(core::ConsumerRecord *record);

    bool open();
    QString error();

    void setUseTimestamp(bool use);
    void setUsePartition(bool use);
    void setUseOffset(bool use);
    void setUseKey(bool use);
    void setUseValue(bool use);

    virtual void appendHeader();

protected:
    virtual void writeHeader(QTextStream &stream);
    virtual void write(QTextStream &stream, core::ConsumerRecord *record) = 0;

    inline bool useTimestamp() const { return m_useTimestamp; }

    inline bool usePartition() const { return m_usePartition; }

    inline bool useOffset() const { return m_useOffset; }

    inline bool useKey() const { return m_useKey; }

    inline bool useValue() const { return m_useValue; }

private:
    void closeFile();

private:
    QFile m_file;
    QTextStream m_stream;
    bool m_useTimestamp;
    bool m_usePartition;
    bool m_useOffset;
    bool m_useKey;
    bool m_useValue;
};
} // namespace core
