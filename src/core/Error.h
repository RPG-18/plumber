#pragma once

#include <QtCore/QString>

namespace core {
struct Error
{
    explicit Error();
    explicit Error(QString wh, QString wt);

    bool isError;
    QString where;
    QString what;
};
} // namespace core
