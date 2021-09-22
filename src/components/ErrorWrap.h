#pragma once

#include <QtCore/QObject>

/**!
 *
 */
struct ErrorWrap
{
    Q_GADGET
    Q_PROPERTY(bool isError MEMBER isError)
    Q_PROPERTY(QString where MEMBER where)
    Q_PROPERTY(QString what MEMBER what)
public:
    explicit ErrorWrap();
    explicit ErrorWrap(QString wh, QString wt);

    bool isError;
    QString where;
    QString what;
};

Q_DECLARE_METATYPE(ErrorWrap)
