// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef KAPPLICATIONSCOPE_H
#define KAPPLICATIONSCOPE_H
#include "kcgroups_export.h"
#include "optional.h"
#include <QObject>

class KApplicationScopePrivate;

#define OPTIONAL_GADGET(T, name)                                                                                       \
    /**                                                                                                                \
     @brief An optional T.                                                                                             \
     The parameterless constructor creates a name without value. \n                                                    \
     Use name(const T &val) to create a name with a value. \n                                                          \
     Can be implicitly converted to T (undefined behavior when no value) and to bool (same has hasValue())             \
     */                                                                                                                \
    class KCGROUPS_EXPORT name : public optional<T>                                                                    \
    {                                                                                                                  \
        Q_GADGET                                                                                                       \
                                                                                                                       \
        /**                                                                                                            \
         @brief The underlying T value. Returns a default T if hasValue() is false. \n                                 \
         Has a reset() method to remove the value, which can be reached in QML via x.value = undefined                 \
         @accessors getValue(), setValue(), reset()                                                                    \
         */                                                                                                            \
        Q_PROPERTY(T value READ getValue WRITE setValue RESET reset)                                                   \
                                                                                                                       \
        /**                                                                                                            \
         @brief Whether there is currently a value defined                                                             \
         @accessors hasValue()                                                                                         \
         */                                                                                                            \
        Q_PROPERTY(bool hasValue READ hasValue)                                                                        \
                                                                                                                       \
    public:                                                                                                            \
        /**                                                                                                            \
         The underlying value type. Equal to T                                                                         \
         */                                                                                                            \
        using value_type = T;                                                                                          \
                                                                                                                       \
        using optional<T>::optional;                                                                                   \
                                                                                                                       \
        /**                                                                                                            \
         @brief get value or default                                                                                   \
         @return underlying value if hasValue() is true, else default T value                                          \
         */                                                                                                            \
        T getValue()                                                                                                   \
        {                                                                                                              \
            return *this ? **this : T();                                                                               \
        }                                                                                                              \
                                                                                                                       \
        /**                                                                                                            \
         @brief set a T value                                                                                          \
         @param val: the value to set                                                                                  \
         */                                                                                                            \
        void setValue(const T &val)                                                                                    \
        {                                                                                                              \
            *reinterpret_cast<optional<T> *>(this) = val;                                                              \
        }                                                                                                              \
                                                                                                                       \
        /**                                                                                                            \
         @brief whether a value is currently set                                                                       \
         @return true if a value is present, false otherwise                                                           \
         */                                                                                                            \
        bool hasValue()                                                                                                \
        {                                                                                                              \
            return static_cast<bool>(*this);                                                                           \
        }                                                                                                              \
    };                                                                                                                 \
                                                                                                                       \
    inline bool operator==(const name &lhs, const name &rhs)                                                           \
    {                                                                                                                  \
        return static_cast<optional<typename name::value_type>>(lhs) ==                                                \
            static_cast<optional<typename name::value_type>>(rhs);                                                     \
    }                                                                                                                  \
                                                                                                                       \
    inline bool operator!=(const name &lhs, const name &rhs)                                                           \
    {                                                                                                                  \
        return static_cast<optional<typename name::value_type>>(lhs) !=                                                \
            static_cast<optional<typename name::value_type>>(rhs);                                                     \
    }                                                                                                                  \
                                                                                                                       \
    template<typename U>                                                                                               \
    bool operator==(const name &lhs, const optional<U> &rhs)                                                           \
    {                                                                                                                  \
        return static_cast<optional<typename name::value_type>>(lhs) == rhs;                                           \
    }                                                                                                                  \
                                                                                                                       \
    template<typename U>                                                                                               \
    bool operator!=(const name &lhs, const optional<U> &rhs)                                                           \
    {                                                                                                                  \
        return static_cast<optional<typename name::value_type>>(lhs) != rhs;                                           \
    }                                                                                                                  \
                                                                                                                       \
    template<typename T>                                                                                               \
    bool operator==(const optional<T> &lhs, const name &rhs)                                                           \
    {                                                                                                                  \
        return lhs == static_cast<optional<typename name::value_type>>(rhs);                                           \
    }                                                                                                                  \
                                                                                                                       \
    template<typename T>                                                                                               \
    bool operator!=(const optional<T> &lhs, const name &rhs)                                                           \
    {                                                                                                                  \
        return lhs != static_cast<optional<typename name::value_type>>(rhs);                                           \
    }                                                                                                                  \
                                                                                                                       \
    Q_DECLARE_METATYPE(name)                                                                                           \
    static_assert(true, "")

OPTIONAL_GADGET(qulonglong, OptionalQULongLong);

/**
 * @brief A desktop application in a systemd transient scope
 */
class KCGROUPS_EXPORT KApplicationScope : public QObject
{
    Q_OBJECT

    /**
     * @brief the dbus path of the application. Same as constructor parameter. Read-only, constant
     * @accessors path()
     */
    Q_PROPERTY(QString path READ path)

    /**
     * @brief the systemd unit id. Read-only. Will be set asynchronously if not specified in constructor.
     * @accessors id()
     * @notifySignal idChanged()
     */
    Q_PROPERTY(QString id READ id NOTIFY idChanged)

    /**
     * @brief file path of the control group in /sys/fs/cgroup
     * @accessors cgroup()
     * @notifySignal cgroupChanged()
     */
    Q_PROPERTY(QString cgroup READ cgroup NOTIFY cgroupChanged)

    /**
     * @brief code of the last error that occurred (NoError if none)
     * @accessors lastError()
     * @notifySignal errorOccurred()
     */
    Q_PROPERTY(ErrorCode lastError READ lastError NOTIFY errorOccurred)

    /**
     * @brief cpu quota for cpu controller, in microseconds per second (1000000 means 100%). Can be unset
     * @accessors cpuQuota(), setCpuQuota()
     * @notifySignal cpuQuotaChanged()
     */
    Q_PROPERTY(OptionalQULongLong cpuQuota READ cpuQuota WRITE setCpuQuota NOTIFY cpuQuotaChanged)

    /**
     * @brief duration in micoseconds over which the CPU time quota is measured. (default when unset is 100ms)
     * @accessors cpuQuotaPeriod(), setCpuQuotaPeriod()
     * @notifySignal cpuQuotaPeriodChanged()
     */
    Q_PROPERTY(OptionalQULongLong cpuQuotaPeriod READ cpuQuotaPeriod WRITE setCpuQuotaPeriod NOTIFY cpuQuotaPeriodChanged)

    /**
     * @brief cpu time weight. Between 1 and 10000. Defaults to 100.
     * @accessors cpuQuotaPeriod(), setCpuQuotaPeriod()
     * @notifySignal cpuQuotaPeriodChanged()
     */
    Q_PROPERTY(OptionalQULongLong cpuWeight READ cpuWeight WRITE setCpuWeight NOTIFY cpuWeightChanged)

    /**
     * @brief Overall block I/O weight. Between 1 and 10000. Defaults to 100.
     * @accessors ioWeight(), setIoWeight()
     * @notifySignal ioWeightChanged()
     */
    Q_PROPERTY(OptionalQULongLong ioWeight READ ioWeight WRITE setIoWeight NOTIFY ioWeightChanged)

    /**
     * @brief best-effort memory usage protection (in bytes) of all executed processes within the application.
     * @accessors memoryLow(), setMemoryLow()
     * @notifySignal memoryLowChanged()
     */
    Q_PROPERTY(OptionalQULongLong memoryLow READ memoryLow WRITE setMemoryLow NOTIFY memoryLowChanged)

    /**
     * @brief throttling limit on memory usage (in bytes) of all executed processes within the application.
     * @accessors memoryHigh(), setMemoryHigh()
     * @notifySignal memoryHighChanged()
     */
    Q_PROPERTY(OptionalQULongLong memoryHigh READ memoryHigh WRITE setMemoryHigh NOTIFY memoryHighChanged)

    /**
     * @brief memory usage protection (in bytes) of all executed processes within the application.
     * @accessors memoryMin(), setMemoryMin()
     * @notifySignal memoryMinChanged()
     */
    Q_PROPERTY(OptionalQULongLong memoryMin READ memoryMin WRITE setMemoryMin NOTIFY memoryMinChanged)

    /**
     * @brief absolute limit on memory usage (in bytes) of all executed processes within the application.
     * @accessors memoryMax(), setMemoryMax()
     * @notifySignal memoryMaxChanged()
     */
    Q_PROPERTY(OptionalQULongLong memoryMax READ memoryMax WRITE setMemoryMax NOTIFY memoryMaxChanged)

    /**
     * @brief absolute limit on swap usage (in bytes) of all executed processes within the application.
     * @accessors memorySwapMax(), setMemorySwapMax()
     * @notifySignal memorySwapMaxChanged()
     */
    Q_PROPERTY(OptionalQULongLong memorySwapMax READ memorySwapMax WRITE setMemorySwapMax NOTIFY memorySwapMaxChanged)

public:
    /**
     * @brief The types of errors that can occur
     */
    enum ErrorCode {
        /**
         * Default value. No error has occurred
         */
        NoError,

        /**
         * A property set operation failed
         */
        SetFailedError,

        /**
         * Initial loading of property values failed
         */
        CacheFillError,

        /**
         * Error during stop() operation
         */
        StopFailedError
    };
    Q_ENUM(ErrorCode)

    /**
     * @brief Use when only path is known. Incurs an extra DBus call to get unit id.
     * @param path: dbus path of the application
     * @param parent: parent QObject
     */
    explicit KApplicationScope(const QString &path, QObject *parent = nullptr);

    /**
     * @brief Use when unit id is known in advance (such as when using KApplicationScopeLister)
     * @param path: dbus path of the application
     * @param id: systemd unit id
     * @param parent: parent QObject
     */
    explicit KApplicationScope(const QString &path, const QString &id, QObject *parent = nullptr);

    /**
     * @brief Use when only PID is known
     * @param pid: process ID
     * @param parent: parent QObject
     * @return a new KApplicationScope, or null in case of failure (such as if process is not managed by systemd)
     */
    static KApplicationScope *fromPid(uint pid, QObject *parent = nullptr);

    ~KApplicationScope() override;

    QString path() const;
    QString id() const;
    QString cgroup() const;
    ErrorCode lastError() const;

    OptionalQULongLong cpuQuota() const;
    OptionalQULongLong cpuQuotaPeriod() const;
    OptionalQULongLong cpuWeight() const;
    OptionalQULongLong ioWeight() const;
    OptionalQULongLong memoryLow() const;
    OptionalQULongLong memoryHigh() const;
    OptionalQULongLong memoryMin() const;
    OptionalQULongLong memoryMax() const;
    OptionalQULongLong memorySwapMax() const;

Q_SIGNALS:
    /**
     * @brief emitted after loading when id is not known at constructor time
     * @param id: the systemd unit id
     */
    void idChanged(const QString &id);

    /**
     * @brief emitted when cgroup has been loaded asynchronously
     * @param cgroup: the filesystem cgroup path
     */
    void cgroupChanged(const QString &cgroup);

    /**
     * @brief emitted when there is an error setting or getting a value
     * @param lastError: the error code
     */
    void errorOccurred(ErrorCode lastError);

    /**
     * @brief emitted when any cgroup resource property has changed
     * @param propertyName: the systemd name of the property
     */
    void propertyChanged(const QString &propertyName);

    /**
     * @brief emitted when the cpu quota has changed
     * @param quota: the new quota value
     */
    void cpuQuotaChanged(const OptionalQULongLong &quota);

    /**
     * @brief emitted when the cpu quota period has changed
     * @param period: the new period value
     */
    void cpuQuotaPeriodChanged(const OptionalQULongLong &period);

    /**
     * @brief emitted when the cpu weight has changed
     * @param weight: the new weight value
     */
    void cpuWeightChanged(const OptionalQULongLong &weight);

    /**
     * @brief emitted when the io weight has changed
     * @param weight: the new weight value
     */
    void ioWeightChanged(const OptionalQULongLong &weight);

    /**
     * @brief emitted when memoryLow has changed
     * @param memoryLow: the new memoryLow value
     */
    void memoryLowChanged(const OptionalQULongLong &memoryLow);

    /**
     * @brief emitted when memoryHigh has changed
     * @param memoryHigh: the new memoryHigh value
     */
    void memoryHighChanged(const OptionalQULongLong &memoryHigh);

    /**
     * @brief emitted when memoryMin has changed
     * @param memoryMin: the new memoryMin value
     */
    void memoryMinChanged(const OptionalQULongLong &memoryMin);

    /**
     * @brief emitted when memoryMax has changed
     * @param memoryMax: the new memoryMax value
     */
    void memoryMaxChanged(const OptionalQULongLong &memoryMax);

    /**
     * @brief emitted when memorySwapMax has changed
     * @param memorySwapMax: the new memorySwapMax value
     */
    void memorySwapMaxChanged(const OptionalQULongLong &memorySwapMax);

public Q_SLOTS:
    /**
     * @brief Stops the application
     */
    void stop();

    /**
     * @brief set cpuQuota
     * @param quota: value to set
     */
    void setCpuQuota(const OptionalQULongLong &quota);

    /**
     * @brief set cpuQuotaPeriod
     * @param period: value to set
     */
    void setCpuQuotaPeriod(const OptionalQULongLong &period);

    /**
     * @brief set cpuWeight
     * @param weight: value to set
     */
    void setCpuWeight(const OptionalQULongLong &weight);

    /**
     * @brief set ioWeight
     * @param weight: value to set
     */
    void setIoWeight(const OptionalQULongLong &weight);

    /**
     * @brief set memoryLow
     * @param memoryLow: value to set
     */
    void setMemoryLow(const OptionalQULongLong &memoryLow);

    /**
     * @brief set memoryHigh
     * @param memoryHigh: value to set
     */
    void setMemoryHigh(const OptionalQULongLong &memoryHigh);

    /**
     * @brief set memoryMin
     * @param memoryMin: value to set
     */
    void setMemoryMin(const OptionalQULongLong &memoryMin);

    /**
     * @brief set memoryMax
     * @param memoryMax: value to set
     */
    void setMemoryMax(const OptionalQULongLong &memoryMax);

    /**
     * @brief set memorySwapMax
     * @param memorySwapMax: value to set
     */
    void setMemorySwapMax(const OptionalQULongLong &memorySwapMax);

private:
    KApplicationScopePrivate *const d_ptr;
};

#endif // KAPPLICATIONSCOPE_H
