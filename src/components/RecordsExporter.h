#pragma once

#include "ConsumerRecordsExporter.h"

#include "Types.h"

/**!
 * class CSVConsumerRecordsExporter export to CSV
 */
class CSVConsumerRecordsExporter final : public core::AbstractConsumerRecordsExporter
{
public:
    CSVConsumerRecordsExporter(Types key, Types value, const QString &file);

protected:
    void writeHeader(QTextStream &stream) override;
    void write(QTextStream &stream, core::ConsumerRecord *record) override;

private:
    Types m_key;
    Types m_value;
};

/**!
 * class JSONConsumerRecordsExporter export to JSON
 */
class JSONConsumerRecordsExporter final : public core::AbstractConsumerRecordsExporter
{
public:
    JSONConsumerRecordsExporter(Types key, Types value, const QString &file);

protected:
    void writeHeader(QTextStream &stream) override;
    void write(QTextStream &stream, core::ConsumerRecord *record) override;

private:
    Types m_key;
    Types m_value;
};