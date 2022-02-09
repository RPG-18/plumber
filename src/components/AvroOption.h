#pragma once

#include <memory>
#include <tuple>

#include <QtCore/QObject>
#include <QtCore/QUrl>

#include "ErrorWrap.h"

namespace core {
class AbstractConverter;
}

/**!
 * AvroOption avro options and fabric
 */
class AvroOption : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QUrl file READ file WRITE setFile)

    explicit AvroOption(QObject *parent = nullptr);

    void setFile(const QUrl &file);
    const QUrl &file() const;

    std::tuple<std::unique_ptr<core::AbstractConverter>, ErrorWrap> converter();

signals:

    void changed();

private:
    QUrl m_file;
};
