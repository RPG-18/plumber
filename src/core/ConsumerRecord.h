#pragma once

#include <kafka/Timestamp.h>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QVector>

#include "Types.h"

namespace core {
struct Header
{
    QString key;
    QString value;
};

using Headers = QVector<Header>;

struct ConsumerRecord
{
    Partition partition;
    Offset offset;
    QString topic;
    QByteArray key;
    QByteArray value;
    kafka::Timestamp timestamp;
    Headers headers;
};

using ConsumerRecords = QVector<ConsumerRecord *>;

} // namespace core