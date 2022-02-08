#include "AbstractConverter.h"

namespace core {
QByteArray AbstractConverter::toJSON(QByteArray &&binary)
{
    return std::move(binary);
}

std::tuple<QByteArray, Error> AbstractConverter::fromJSON(QByteArray &&json)
{
    return std::make_tuple(std::move(json), Error{});
}
} // namespace core
