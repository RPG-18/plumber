#include "spdlog/spdlog.h"

#include "ExportImportFabric.h"
#include "RecordsExporter.h"

ExportImportFabric::ExportImportFabric(QObject *parent)
    : QObject(parent)
    , m_format(CSVFormat)
    , m_useTimestamp(true)
    , m_usePartition(true)
    , m_useOffset(true)
    , m_useKey(true)
    , m_useValue(true)
{}

void ExportImportFabric::setFile(const QUrl &file)
{
    m_file = file;
}

const QUrl &ExportImportFabric::file() const
{
    return m_file;
}

ExportImportFabric::Format ExportImportFabric::format() const
{
    return m_format;
}

void ExportImportFabric::setFormat(ExportImportFabric::Format f)
{
    m_format = f;
}

bool ExportImportFabric::useTimestamp() const
{
    return m_useTimestamp;
}

void ExportImportFabric::setUseTimestamp(bool use)
{
    m_useTimestamp = use;
}

bool ExportImportFabric::usePartition() const
{
    return m_usePartition;
}

void ExportImportFabric::setUsePartition(bool use)
{
    m_usePartition = use;
}

bool ExportImportFabric::useOffset() const
{
    return m_useOffset;
}

void ExportImportFabric::setUseOffset(bool use)
{
    m_useOffset = use;
}

bool ExportImportFabric::useKey() const
{
    return m_useKey;
}

void ExportImportFabric::setUseKey(bool use)
{
    m_useKey = use;
}

bool ExportImportFabric::useValue() const
{
    return m_useValue;
}

void ExportImportFabric::setUseValue(bool use)
{
    m_useValue = use;
}

std::unique_ptr<core::AbstractConsumerRecordsExporter> ExportImportFabric::makeExporter(Types key,
                                                                                        Types value)
{
    if (!m_file.isValid()) {
        return nullptr;
    }

    auto file = m_file.toString(QUrl::RemoveScheme);

    if (m_format == CSVFormat) {
        if (!file.endsWith("csv")) {
            file.push_back(".csv");
        }
    } else {
        if (!file.endsWith("json")) {
            file.push_back(".json");
        }
    }

    std::unique_ptr<core::AbstractConsumerRecordsExporter> exporter;
    if (m_format == CSVFormat) {
        exporter = std::make_unique<CSVConsumerRecordsExporter>(key, value, file);
    } else {
        exporter = std::make_unique<JSONConsumerRecordsExporter>(key, value, file);
    }

    if (!exporter->open()) {
        spdlog::error("failed open file to write {}", m_file.path().toStdString());
        return nullptr;
    }

    exporter->appendHeader();
    exporter->setUseTimestamp(m_useTimestamp);
    exporter->setUsePartition(m_usePartition);
    exporter->setUseOffset(m_useOffset);
    exporter->setUseKey(m_useKey);
    exporter->setUseValue(m_useValue);
    return exporter;
}
