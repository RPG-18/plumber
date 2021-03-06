#pragma once

#include "ConsumerRecord.h"
#include <memory>

namespace core {

/**!
 * class AbstractFilter abstract filter
 *
 * Override IsAcceptable for implement a filter rules
 */
class AbstractFilter
{
public:
    virtual ~AbstractFilter() = default;

    virtual bool IsAcceptable(const std::unique_ptr<ConsumerRecord> &record) noexcept;
    bool isEmpty();

    void setKey(const QByteArray &key);
    void setValue(const QByteArray &value);
    void setHeader(const Header &header);

protected:
    QByteArray m_key;
    QByteArray m_value;
    Header m_header;
};

/**!
 *  Contain key/value, header
 */
class ContainFilter final : public AbstractFilter
{
public:
    bool IsAcceptable(const std::unique_ptr<ConsumerRecord> &record) noexcept override;
};

/**!
 * Does not contain
 */
class NotContainFilter final : public AbstractFilter
{
public:
    bool IsAcceptable(const std::unique_ptr<ConsumerRecord> &record) noexcept override;
};

/**!
 * Equal filter
 */
class EqualFilter final : public AbstractFilter
{
public:
    bool IsAcceptable(const std::unique_ptr<ConsumerRecord> &record) noexcept override;
};
} // namespace core