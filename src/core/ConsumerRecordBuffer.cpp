#include <QtCore/QDebug>

#include "ConsumerRecordBuffer.h"
#ifdef Q_OS_WINDOWS
#undef min
#endif
#include <algorithm>

namespace core {

using LockGuard = std::lock_guard<std::mutex>;

ConsumerRecordBuffer::ConsumerRecordBuffer(int64_t size)
    : m_size(size)
    , m_write(0)
    , m_buffer(size)
{}

void ConsumerRecordBuffer::push(std::unique_ptr<ConsumerRecord> &&record)
{
    const LockGuard lock(m_mu);
    m_write++;
    m_buffer.push_back(std::move(record));
}

ConsumerRecords ConsumerRecordBuffer::records()
{
    const LockGuard lock(m_mu);

    ConsumerRecords data;
    const auto count = std::min(m_write, m_size);
    data.reserve(count);

    auto begin = m_buffer.end() - count;
    for (auto iter = begin; iter < m_buffer.end(); ++iter) {
        data.push_back(iter->release());
    }

    m_write = 0;
    return data;
}

} // namespace core
