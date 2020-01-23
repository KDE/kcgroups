// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <memory>
#include <type_traits>

template<typename T>
class optional
{
    static_assert(!std::is_reference<T>::value, "optional doesn't support references");

public:
    optional() = default;

    optional(const T &v)
        : m_value(v)
        , m_hasValue(true)
    {
    }

    optional(T &&v)
        : m_value(std::move(v))
        , m_hasValue(true)
    {
    }

    explicit operator bool() const noexcept
    {
        return m_hasValue;
    }

    T const &operator*() const noexcept
    {
        return m_value;
    }

    T operator*()
    {
        return m_value;
    }

    optional &operator=(const T &v) noexcept
    {
        m_hasValue = true;
        m_value = v;
        return *this;
    }

    optional &operator=(T &&v) noexcept
    {
        m_hasValue = true;
        m_value = std::move(v);
        return *this;
    }

    void reset()
    {
        if (m_hasValue) {
            m_value.~T();
            m_hasValue = false;
        }
    }

private:
    T m_value = {};
    bool m_hasValue = false;
}; // class optional

template<typename T, typename U>
bool operator==(const optional<T> &lhs, const optional<U> &rhs) noexcept
{
    const auto l = static_cast<bool>(lhs), r = static_cast<bool>(rhs);
    return l && r ? *lhs == *rhs : !l && !r;
}

template<typename T, typename U>
bool operator!=(const optional<T> &lhs, const optional<U> &rhs) noexcept
{
    return !(lhs == rhs);
}

template<typename T, typename U>
bool operator==(const optional<T> &lhs, const U &rhs) noexcept
{
    return static_cast<bool>(lhs) && *lhs == rhs;
}

template<typename T, typename U>
bool operator!=(const optional<T> &lhs, const U &rhs) noexcept
{
    return !(lhs == rhs);
}

template<typename T, typename U>
bool operator==(const T &lhs, const optional<T> &rhs) noexcept
{
    return rhs == lhs;
}

template<typename T, typename U>
bool operator!=(const T &lhs, const optional<T> &rhs) noexcept
{
    return !(lhs == rhs);
}

#endif // OPTIONAL_H
