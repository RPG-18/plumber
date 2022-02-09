#include "Error.h"

namespace core {
Error::Error()
    : isError(false)
{}

Error::Error(QString wh, QString wt)
    : isError(true)
    , where(std::move(wh))
    , what(std::move(wt))
{}
} // namespace core