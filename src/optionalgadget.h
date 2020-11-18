// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef OPTIONALGADGET_H
#define OPTIONALGADGET_H
#include "optional.h"

#define OPTIONAL_GADGET(T, name)                                                                                                 \
    /**                                                                                                                          \
     @brief An optional T.                                                                                                       \
     The parameterless constructor creates a name without value. \n                                                              \
     Use name(const T &val) to create a name with a value. \n                                                                    \
     Can be implicitly converted to T (undefined behavior when no value) and to bool (same has hasValue())                       \
     */                                                                                                                          \
    class KCGROUPS_EXPORT name : public optional<T>                                                                              \
    {                                                                                                                            \
        Q_GADGET                                                                                                                 \
                                                                                                                                 \
        /**                                                                                                                      \
         @brief The underlying T value. Returns a default T if hasValue() is false. \n                                           \
         Has a reset() method to remove the value, which can be reached in QML via x.value = undefined                           \
         @accessors getValue(), setValue(), reset()                                                                              \
         */                                                                                                                      \
        Q_PROPERTY(T value READ getValue WRITE setValue RESET reset)                                                             \
                                                                                                                                 \
        /**                                                                                                                      \
         @brief Whether there is currently a value defined                                                                       \
         @accessors hasValue()                                                                                                   \
         */                                                                                                                      \
        Q_PROPERTY(bool hasValue READ hasValue)                                                                                  \
                                                                                                                                 \
    public:                                                                                                                      \
        /**                                                                                                                      \
         The underlying value type. Equal to T                                                                                   \
         */                                                                                                                      \
        using value_type = T;                                                                                                    \
                                                                                                                                 \
        using optional<T>::optional;                                                                                             \
                                                                                                                                 \
        /**                                                                                                                      \
         @brief get value or default                                                                                             \
         @return underlying value if hasValue() is true, else default T value                                                    \
         */                                                                                                                      \
        T getValue()                                                                                                             \
        {                                                                                                                        \
            return *this ? **this : T();                                                                                         \
        }                                                                                                                        \
                                                                                                                                 \
        /**                                                                                                                      \
         @brief set a T value                                                                                                    \
         @param val: the value to set                                                                                            \
         */                                                                                                                      \
        void setValue(const T &val)                                                                                              \
        {                                                                                                                        \
            *reinterpret_cast<optional<T> *>(this) = val;                                                                        \
        }                                                                                                                        \
                                                                                                                                 \
        /**                                                                                                                      \
         @brief whether a value is currently set                                                                                 \
         @return true if a value is present, false otherwise                                                                     \
         */                                                                                                                      \
        bool hasValue()                                                                                                          \
        {                                                                                                                        \
            return static_cast<bool>(*this);                                                                                     \
        }                                                                                                                        \
    };                                                                                                                           \
                                                                                                                                 \
    inline bool operator==(const name &lhs, const name &rhs)                                                                     \
    {                                                                                                                            \
        return static_cast<optional<typename name::value_type>>(lhs) == static_cast<optional<typename name::value_type>>(rhs);   \
    }                                                                                                                            \
                                                                                                                                 \
    inline bool operator!=(const name &lhs, const name &rhs)                                                                     \
    {                                                                                                                            \
        return static_cast<optional<typename name::value_type>>(lhs) != static_cast<optional<typename name::value_type>>(rhs);   \
    }                                                                                                                            \
                                                                                                                                 \
    template<typename U>                                                                                                         \
    bool operator==(const name &lhs, const optional<U> &rhs)                                                                     \
    {                                                                                                                            \
        return static_cast<optional<typename name::value_type>>(lhs) == rhs;                                                     \
    }                                                                                                                            \
                                                                                                                                 \
    template<typename U>                                                                                                         \
    bool operator!=(const name &lhs, const optional<U> &rhs)                                                                     \
    {                                                                                                                            \
        return static_cast<optional<typename name::value_type>>(lhs) != rhs;                                                     \
    }                                                                                                                            \
                                                                                                                                 \
    template<typename T>                                                                                                         \
    bool operator==(const optional<T> &lhs, const name &rhs)                                                                     \
    {                                                                                                                            \
        return lhs == static_cast<optional<typename name::value_type>>(rhs);                                                     \
    }                                                                                                                            \
                                                                                                                                 \
    template<typename T>                                                                                                         \
    bool operator!=(const optional<T> &lhs, const name &rhs)                                                                     \
    {                                                                                                                            \
        return lhs != static_cast<optional<typename name::value_type>>(rhs);                                                     \
    }                                                                                                                            \
                                                                                                                                 \
    Q_DECLARE_METATYPE(name)                                                                                                     \
    static_assert(true, "")

#endif // OPTIONALGADGET_H
