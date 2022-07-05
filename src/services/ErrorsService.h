#pragma once

#include <QtCore/QObject>

#include "ErrorWrap.h"
#include "core/Error.h"

/**!
 * class ErrorsService. Show errors on main window
 */
class ErrorsService : public QObject
{
    Q_OBJECT

public:
    ErrorsService() = default;
    ErrorsService &operator<<(core::Error &&err);

signals:

    void error(ErrorWrap err);
};