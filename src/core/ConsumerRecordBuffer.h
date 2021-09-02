#pragma once

#include "ConsumerRecord.h"
#include <boost/circular_buffer.hpp>
#include <cstdint>
#include <mutex>

namespace core {
class ConsumerRecordBuffer
{
public:
    explicit ConsumerRecordBuffer(size_t size);

    void push(std::unique_ptr<ConsumerRecord> &&record);
    ConsumerRecords records();

private:
    size_t m_size;
    size_t m_write;
    std::mutex m_mu;
    boost::circular_buffer<std::unique_ptr<ConsumerRecord>> m_buffer;
};
} // namespace core
