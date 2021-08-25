#include "ErrorWrap.h"

ErrorWrap::ErrorWrap()
    : isError(false)
{}

ErrorWrap::ErrorWrap(QString wn, QString wt)
    : isError(true)
    , when(std::move(wn))
    , what(std::move(wt))
{}