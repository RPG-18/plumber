#pragma once

#include <google/protobuf/compiler/importer.h>
#include <QtCore/QStringList>

namespace formats {
namespace protobuf {

/**!
 * Error collector implementation
 */
class ProtobufErrorCollector final : public google::protobuf::compiler::MultiFileErrorCollector
{
public:
    void AddError(const std::string &filename,
                  int line,
                  int column,
                  const std::string &message) override;

    void AddWarning(const std::string &filename,
                    int line,
                    int column,
                    const std::string &message) override;

    QStringList errors() const;

    bool hasErrors() const;

private:
    QStringList m_messages;
};

} // namespace protobuf
} // namespace formats