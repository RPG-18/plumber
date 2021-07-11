#include <QtCore/QDebug>

#include <cassert>

#include "Limiter.h"

namespace core {
AbstractLimiter::~AbstractLimiter() {}

bool AbstractLimiter::ExcessOfLimit(const ConsumerRecord *)
{
    return false;
}

NumberOfRecordsLimiter::NumberOfRecordsLimiter(int count)
    : m_limit(count)
    , m_count(0)
{
    assert(count > 0);
}

bool NumberOfRecordsLimiter::ExcessOfLimit(const ConsumerRecord *record)
{
    if (m_count > m_limit) {
        return true;
    }

    m_count++;
    return false;
}

MaxSizeLimiter::MaxSizeLimiter(unsigned int bytes)
    : m_limit(bytes)
{
    assert(bytes > 0);
}

bool MaxSizeLimiter::ExcessOfLimit(const ConsumerRecord *record)
{
    const auto size = record->key.size() + record->value.size();
    if (m_bytes > m_limit) {
        return true;
    }
    qDebug() << m_limit << m_bytes;
    m_bytes += size;
    return false;
}

DateLimiter::DateLimiter(const QDateTime &tm)
    : m_tm(tm)
{
    assert(tm.isValid());
    m_msSinceEpoch = m_tm.toMSecsSinceEpoch();
}

bool DateLimiter::ExcessOfLimit(const ConsumerRecord *record)
{
    return record->timestamp.msSinceEpoch > m_msSinceEpoch;
}

} // namespace core
