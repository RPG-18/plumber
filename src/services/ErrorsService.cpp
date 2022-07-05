#include "ErrorsService.h"

ErrorsService &ErrorsService::operator<<(core::Error &&err)
{
    emit error(ErrorWrap(std::move(err.where), std::move(err.what)));
    return *this;
}