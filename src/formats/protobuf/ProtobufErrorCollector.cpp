#include "spdlog/spdlog.h"

#include "ProtobufErrorCollector.h"

namespace formats::protobuf {

void ProtobufErrorCollector::AddError(const std::string &filename,
                                      int line,
                                      int column,
                                      const std::string &message)
{
    spdlog::error("proto file: {}, line: {}, column: {} {}", filename, line, column, message);
    m_messages << QString("error file: %1, line: %2, column: %3 %4")
                      .arg(QString::fromStdString(filename))
                      .arg(line)
                      .arg(column)
                      .arg(QString::fromStdString(message));
}

void ProtobufErrorCollector::AddWarning(const std::string &filename,
                                        int line,
                                        int column,
                                        const std::string &message)
{
    spdlog::warn("proto file: {}, line: {}, column: {} {}", filename, line, column, message);
    m_messages << QString("warning file: %1, line: %2, column: %3 %4")
                      .arg(QString::fromStdString(filename))
                      .arg(line)
                      .arg(column)
                      .arg(QString::fromStdString(message));
}

QStringList ProtobufErrorCollector::errors() const
{
    return m_messages;
}

bool ProtobufErrorCollector::hasErrors() const
{
    return !m_messages.isEmpty();
}

} // namespace formats::protobuf