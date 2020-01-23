// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef KAPPLICATIONSCOPE_P_H
#define KAPPLICATIONSCOPE_P_H
#include "kapplicationscope.h"
#include "propertiesinterface.h"
#include "unitinterface.h"
#include <QVariant>
#include <QtDBus/QDBusPendingCall>

template<typename T>
struct Property {
    using value_type = typename T::value_type;
    T KApplicationScopePrivate::*privateMember;
    void (KApplicationScope::*changedSignal)(const T &);
    QString systemdName;
    bool hasDefault = false;
    value_type defaultValue = {};
};

class KApplicationScopePrivate
{
public:
    KApplicationScopePrivate(const QString &path, const QString &id, KApplicationScope *parent);
    KApplicationScope::ErrorCode m_lastError;
    OptionalQULongLong m_cpuQuota;
    OptionalQULongLong m_cpuQuotaPeriod;
    OptionalQULongLong m_memoryLow;
    OptionalQULongLong m_memoryHigh;
    OptionalQULongLong m_memoryMin;
    OptionalQULongLong m_memoryMax;
    OptionalQULongLong m_memorySwapMax;
    QString m_path;
    QString m_id;
    QString m_cgroup;
    template<typename T>
    T getProperty(const Property<T> &prop);
    template<typename T>
    void trySetProperty(const Property<T> &prop, T variant);
    void stop();

private:
    KApplicationScope *q_ptr;
    org::freedesktop::systemd1::Unit *m_unit;
    org::freedesktop::DBus::Properties *m_properties;
    void handleGetAllCallFinished(QDBusPendingCallWatcher *call);
    void handleGetIdFinished(QDBusPendingCallWatcher *call);
    void handleVoidCallFinished(QDBusPendingCallWatcher *call, KApplicationScope::ErrorCode code);
    template<typename T>
    T nullIfDefault(const Property<T> &prop, const QVariant &variant);
    template<typename T>
    QVariant defaultIfNull(const Property<T> &prop, const T &opt);
    template<typename T>
    void saveProperty(const Property<T> &prop, const T &opt);
    template<typename T>
    void saveIfNull(const Property<T> &prop, const QVariant &variant);
    void setError(KApplicationScope::ErrorCode code, const QString &message = QString(), const bool warning = false);
};

#endif // KAPPLICATIONSCOPE_P_H
