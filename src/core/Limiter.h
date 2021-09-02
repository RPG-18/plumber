#pragma once

#include "ConsumerRecord.h"
#include <memory>
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
    virtual ~AbstractLimiter() = default;

    /**!
     * ExcessOfLimit implement a limit strategy
     * @param record record
     * @return false if needed stop consuming
     *
     * Default implementation do nothing
     */
    virtual bool ExcessOfLimit(const std::unique_ptr<ConsumerRecord> &record);
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
    bool ExcessOfLimit(const std::unique_ptr<ConsumerRecord> &record) override;

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
    bool ExcessOfLimit(const std::unique_ptr<ConsumerRecord> &record) override;

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
    DateLimiter(QDateTime tm);
    bool ExcessOfLimit(const std::unique_ptr<ConsumerRecord> &record) override;

private:
    QDateTime m_tm;
    int64_t m_msSinceEpoch;
};

} // namespace core