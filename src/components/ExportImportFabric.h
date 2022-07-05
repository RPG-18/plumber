#pragma once

#include <QtCore/QObject>
#include <QtCore/QUrl>

#include "ConsumerRecordsExporter.h"
#include "ErrorWrap.h"
#include "Types.h"

/**!
 * ExportImportFabric export/import format fabric
 */
class ExportImportFabric : public QObject
{
    Q_OBJECT

public:
    using ExporterPtr = std::unique_ptr<core::AbstractConsumerExporter>;

    enum Format { CSVFormat, JSONFormat };
    Q_ENUM(Format);

    Q_PROPERTY(QUrl file READ file WRITE setFile)
    Q_PROPERTY(Format format READ format WRITE setFormat)
    Q_PROPERTY(bool useTimestamp READ useTimestamp WRITE setUseTimestamp)
    Q_PROPERTY(bool usePartition READ usePartition WRITE setUsePartition)
    Q_PROPERTY(bool useOffset READ useOffset WRITE setUseOffset)
    Q_PROPERTY(bool useKey READ useKey WRITE setUseKey)
    Q_PROPERTY(bool useValue READ useValue WRITE setUseValue)

    explicit ExportImportFabric(QObject *parent = nullptr);

    const QUrl &file() const;
    void setFile(const QUrl &file);

    Format format() const;
    void setFormat(Format f);

    bool useTimestamp() const;
    void setUseTimestamp(bool use);

    bool usePartition() const;
    void setUsePartition(bool use);

    bool useOffset() const;
    void setUseOffset(bool use);

    bool useKey() const;
    void setUseKey(bool use);

    bool useValue() const;
    void setUseValue(bool use);

    std::tuple<ExporterPtr, ErrorWrap> makeExporter(Types key, Types value);

private:
    QUrl m_file;
    Format m_format;
    bool m_useTimestamp;
    bool m_usePartition;
    bool m_useOffset;
    bool m_useKey;
    bool m_useValue;
};
Q_DECLARE_METATYPE(ExportImportFabric::Format)
