#include "AbstractConverter.h"

namespace core {
QByteArray AbstractConverter::toJSON(QByteArray &&binary)
{
    return std::move(binary);
}

QByteArray AbstractConverter::fromJSON(QByteArray &&json)
{
    return std::move(json);
}
} // namespace core
