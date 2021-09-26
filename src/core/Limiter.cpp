#include <QtCore/QDebug>

#include <cassert>

#include "Limiter.h"

namespace core {
bool AbstractLimiter::ExcessOfLimit(const std::unique_ptr<ConsumerRecord> & /* record */)
{
    return false;
}

NumberOfRecordsLimiter::NumberOfRecordsLimiter(int count)
    : m_limit(count)
    , m_count(0)
{
    assert(count > 0);
}

bool NumberOfRecordsLimiter::ExcessOfLimit(const std::unique_ptr<ConsumerRecord> & /* record */)
{
    if (m_count > m_limit) {
        return true;
    }

    m_count++;
    return false;
}

MaxSizeLimiter::MaxSizeLimiter(unsigned int bytes)
    : m_limit(bytes)
    , m_bytes(0)
{
    assert(bytes > 0);
}

bool MaxSizeLimiter::ExcessOfLimit(const std::unique_ptr<ConsumerRecord> &record)
{
    const auto size = record->key.size() + record->value.size();
    if (m_bytes > m_limit) {
        return true;
    }

    m_bytes += size;
    return false;
}

DateLimiter::DateLimiter(QDateTime tm)
    : m_tm(std::move(tm))
{
    assert(m_tm.isValid());
    m_msSinceEpoch = m_tm.toMSecsSinceEpoch();
}

bool DateLimiter::ExcessOfLimit(const std::unique_ptr<ConsumerRecord> &record)
{
    return record->timestamp.msSinceEpoch > m_msSinceEpoch;
}

} // namespace core
