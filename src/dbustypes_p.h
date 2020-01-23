// SPDX-FileCopyrightText: 2013 Andrea Scarpino <me@andreascarpino.it>
// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef GENERIC_TYPES_H
#define GENERIC_TYPES_H

#include <QDBusArgument>

/**
 * DbusUnit
 */
using DBusUnit = struct DBusUnit {
    QString name;
    QDBusObjectPath path;
};
Q_DECLARE_METATYPE(DBusUnit)

inline QDBusArgument &operator<<(QDBusArgument &argument, const DBusUnit &unit)
{
    argument.beginStructure();
    argument << unit.name << unit.path;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, DBusUnit &unit)
{
    argument.beginStructure();
    argument >> unit.name >> unit.path;
    argument.endStructure();
    return argument;
}

/**
 * CGroupDBusBlockIODeviceWeight
 */
using CGroupDBusBlockIODeviceWeight = struct CGroupDBusBlockIODeviceWeight {
    QString path;
    qulonglong weight;
};
Q_DECLARE_METATYPE(CGroupDBusBlockIODeviceWeight)
using CGroupDBusBlockIODeviceWeightList = QList<CGroupDBusBlockIODeviceWeight>;
Q_DECLARE_METATYPE(CGroupDBusBlockIODeviceWeightList)

inline QDBusArgument &operator<<(QDBusArgument &argument,
                                 const CGroupDBusBlockIODeviceWeight &cGroupBlockIODeviceWeight)
{
    argument.beginStructure();
    argument << cGroupBlockIODeviceWeight.path << cGroupBlockIODeviceWeight.weight;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument,
                                       CGroupDBusBlockIODeviceWeight &cGroupBlockIODeviceWeight)
{
    argument.beginStructure();
    argument >> cGroupBlockIODeviceWeight.path >> cGroupBlockIODeviceWeight.weight;
    argument.endStructure();
    return argument;
}

/**
 * CGroupDBusDeviceAllow
 */
using CGroupDBusDeviceAllow = struct CGroupDBusDeviceAllow {
    QString path;
    QString rwm;
};
Q_DECLARE_METATYPE(CGroupDBusDeviceAllow)
using CGroupDBusDeviceAllowList = QList<CGroupDBusDeviceAllow>;
Q_DECLARE_METATYPE(CGroupDBusDeviceAllowList)

inline QDBusArgument &operator<<(QDBusArgument &argument, const CGroupDBusDeviceAllow &cGroupDeviceAllow)
{
    argument.beginStructure();
    argument << cGroupDeviceAllow.path << cGroupDeviceAllow.rwm;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, CGroupDBusDeviceAllow &cGroupDeviceAllow)
{
    argument.beginStructure();
    argument >> cGroupDeviceAllow.path >> cGroupDeviceAllow.rwm;
    argument.endStructure();
    return argument;
}

/**
 * ManagerDBusUnit
 */
using ManagerDBusUnit = struct ManagerDBusUnit {
    QString id;
    QString description;
    QString loadState;
    QString activeState;
    QString subState;
    QString following;
    QDBusObjectPath path;
    quint32 jobId;
    QString jobType;
    QDBusObjectPath jobPath;
};
Q_DECLARE_METATYPE(ManagerDBusUnit)
using ManagerDBusUnitList = QList<ManagerDBusUnit>;
Q_DECLARE_METATYPE(ManagerDBusUnitList)

inline QDBusArgument &operator<<(QDBusArgument &argument, const ManagerDBusUnit &unit)
{
    argument.beginStructure();
    argument << unit.id << unit.description << unit.loadState << unit.activeState;
    argument << unit.subState << unit.following << unit.path << unit.jobId;
    argument << unit.jobType << unit.jobPath;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, ManagerDBusUnit &unit)
{
    argument.beginStructure();
    argument >> unit.id >> unit.description >> unit.loadState >> unit.activeState;
    argument >> unit.subState >> unit.following >> unit.path >> unit.jobId;
    argument >> unit.jobType >> unit.jobPath;
    argument.endStructure();
    return argument;
}

/**
 * ManagerDBusAux
 */
using ManagerDBusAux = struct ManagerDBusAux {
    QString name;
    QVariantMap properties;
};
Q_DECLARE_METATYPE(ManagerDBusAux)
using ManagerDBusAuxList = QList<ManagerDBusAux>;
Q_DECLARE_METATYPE(ManagerDBusAuxList)

inline QDBusArgument &operator<<(QDBusArgument &argument, const ManagerDBusAux &aux)
{
    argument.beginStructure();
    argument << aux.name << aux.properties;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, ManagerDBusAux &aux)
{
    argument.beginStructure();
    argument >> aux.name >> aux.properties;
    argument.endStructure();
    return argument;
}

/**
 * Process
 */
using Process = struct Process {
    QString path;
    quint32 pid;
    QString name;
};
Q_DECLARE_METATYPE(Process)
using ProcessList = QList<Process>;
Q_DECLARE_METATYPE(ProcessList)

inline QDBusArgument &operator<<(QDBusArgument &argument, const Process &process)
{
    argument.beginStructure();
    argument << process.path << process.pid << process.name;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, Process &process)
{
    argument.beginStructure();
    argument >> process.path >> process.pid >> process.name;
    argument.endStructure();
    return argument;
}

/**
 * QVariantMultiItem
 */
using QVariantMultiItem = struct QVariantMultiItem {
    QString key;
    QVariant value;
};
Q_DECLARE_METATYPE(QVariantMultiItem)
using QVariantMultiMap = QList<QVariantMultiItem>;
Q_DECLARE_METATYPE(QVariantMultiMap)

inline QDBusArgument &operator<<(QDBusArgument &argument, const QVariantMultiItem &item)
{
    argument.beginStructure();
    argument << item.key << QDBusVariant(item.value);
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, QVariantMultiItem &item)
{
    argument.beginStructure();
    argument >> item.key >> item.value;
    argument.endStructure();
    return argument;
}

#endif // GENERIC_TYPES_H
