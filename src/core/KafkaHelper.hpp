#pragma once

#include <kafka/RdKafkaHelper.h>

namespace core {
struct RtListGroupDeleter
{
    void operator()(const rd_kafka_group_list *p) { rd_kafka_group_list_destroy(p); }
};
using rd_kafka_group_list_unique_ptr = std::unique_ptr<const rd_kafka_group_list, RtListGroupDeleter>;
} // namespace core