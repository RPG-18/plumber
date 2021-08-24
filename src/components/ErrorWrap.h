#pragma once

#include <QtCore/QObject>

/**!
 *
 */
struct ErrorWrap
{
    Q_GADGET
    Q_PROPERTY(bool isError MEMBER isError)
    Q_PROPERTY(QString when MEMBER when)
    Q_PROPERTY(QString what MEMBER what)
public:
    explicit ErrorWrap();
    explicit ErrorWrap(QString wn, QString wt);

    bool isError;
    QString when;
    QString what;
};

Q_DECLARE_METATYPE(ErrorWrap)
