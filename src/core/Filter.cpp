#include "Filter.h"
#include <QtCore/QDebug>

namespace {

template<typename T>
bool contain(const T &left, const T &right) noexcept
{
    if (!left.isEmpty() && !right.isEmpty()) {
        return right.contains(left);
    }
    return false;
}

template<typename T>
bool noContain(const T &left, const T &right) noexcept
{
    if (!left.isEmpty() && !right.isEmpty()) {
        return !right.contains(left);
    }

    return false;
}

template<typename T>
bool equal(const T &left, const T &right) noexcept
{
    if (!left.isEmpty() && !right.isEmpty()) {
        return left == right;
    }
    return false;
}

} // namespace

namespace core {
bool AbstractFilter::IsAcceptable(const std::unique_ptr<ConsumerRecord> & /* record */) noexcept
{
    return true;
}

void AbstractFilter::setKey(const QByteArray &key)
{
    m_key = key;
}

void AbstractFilter::setValue(const QByteArray &value)
{
    m_value = value;
}

void AbstractFilter::setHeader(const Header &header)
{
    m_header = header;
}

bool AbstractFilter::isEmpty()
{
    return m_key.isEmpty() && m_value.isEmpty() && m_header.key.isEmpty()
           && m_header.value.isEmpty();
}

bool ContainFilter::IsAcceptable(const std::unique_ptr<ConsumerRecord> &record) noexcept
{
    if (contain(m_value, record->value)) {
        return true;
    }

    if (contain(m_key, record->key)) {
        return true;
    }

    for (const auto &header : record->headers) {
        if (contain(m_header.value, header.value)) {
            return true;
        }
        if (contain(m_header.key, header.key)) {
            return true;
        }
    }

    return false;
}

bool NotContainFilter::IsAcceptable(const std::unique_ptr<ConsumerRecord> &record) noexcept
{
    if (noContain(m_value, record->value)) {
        return true;
    }

    if (noContain(m_key, record->key)) {
        return true;
    }

    for (const auto &header : record->headers) {
        if (noContain(m_header.value, header.value)) {
            return true;
        }
        if (noContain(m_header.key, header.key)) {
            return true;
        }
    }

    return false;
}

bool EqualFilter::IsAcceptable(const std::unique_ptr<ConsumerRecord> &record) noexcept
{
    if (equal(m_value, record->value)) {
        return true;
    }

    if (equal(m_key, record->key)) {
        return true;
    }

    for (const auto &header : record->headers) {
        if (equal(m_header.value, header.value)) {
            return true;
        }
        if (equal(m_header.key, header.key)) {
            return true;
        }
    }

    return false;
}

} // namespace core
