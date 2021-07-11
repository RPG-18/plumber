#pragma once

#include "ConsumerRecord.h"
#include <QtCore/QDateTime>

namespace core {
/**!
 * class AbstractLimiter abstract limiter.
 *
 * Override ExcessOfLimit for implement a limit strategy
 */
class AbstractLimiter
{
public:
    virtual ~AbstractLimiter();

    /**!
     * ExcessOfLimit implement a limit strategy
     * @param record record
     * @return false if needed stop consuming
     *
     * Default implementation do nothing
     */
    virtual bool ExcessOfLimit(const ConsumerRecord *record);
};

/**!
 * Limit on message count
 */
class NumberOfRecordsLimiter final : public AbstractLimiter
{
public:
    /**!
     * Constructor
     * @param count max messages
     */
    NumberOfRecordsLimiter(int count);
    bool ExcessOfLimit(const ConsumerRecord *record) override;

private:
    int m_limit;
    int m_count;
};

/**!
 * Limit on size
 */
class MaxSizeLimiter final : public AbstractLimiter
{
public:
    MaxSizeLimiter(unsigned int bytes);
    bool ExcessOfLimit(const ConsumerRecord *record) override;

private:
    unsigned int m_limit;
    unsigned int m_bytes;
};

/**!
 * Limit on time
 */
class DateLimiter final : public AbstractLimiter
{
public:
    DateLimiter(const QDateTime &tm);
    bool ExcessOfLimit(const ConsumerRecord *record) override;

private:
    QDateTime m_tm;
    int64_t m_msSinceEpoch;
};

} // namespace core