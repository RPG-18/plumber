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
 * ProtoOption protobuf options and fabric
 */
class ProtoOption : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QUrl file READ file WRITE setFile)
    Q_PROPERTY(QString message READ message WRITE setMessage)

    explicit ProtoOption(QObject *parent = nullptr);

    void setFile(const QUrl &file);
    const QUrl &file() const;

    void setMessage(const QString &message);
    const QString &message() const;

    std::tuple<std::unique_ptr<core::AbstractConverter>, ErrorWrap> converter();

signals:

    void changed();

private:
    QUrl m_file;
    QString m_message;
};