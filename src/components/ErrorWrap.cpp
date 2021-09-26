#include "ErrorWrap.h"

ErrorWrap::ErrorWrap()
    : isError(false)
{}

ErrorWrap::ErrorWrap(QString wh, QString wt)
    : isError(true)
    , where(std::move(wh))
    , what(std::move(wt))
{}