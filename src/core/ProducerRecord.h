#pragma once

#include "Types.h"

namespace core {

struct ProducerRecord
{
    QString topic;
    QByteArray key;
    QByteArray value;
};

struct ProducerMetadata
{
    Partition partition;
    Offset offset;
    QString topic;
    kafka::Timestamp timestamp;
};
} // namespace core