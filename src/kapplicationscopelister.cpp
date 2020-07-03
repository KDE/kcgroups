// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "kapplicationscopelister.h"
#include "kapplicationscopelister_p.h"
#include "kcgroups_debug.h"
#include "managerinterface.h"

KApplicationScopeLister::KApplicationScopeLister(QObject *parent)
    : QObject(parent)
    , d(new KApplicationScopeListerPrivate(this))
{
}

KApplicationScopeLister::~KApplicationScopeLister()
{
    delete d;
}

KApplicationScopeLister::ErrorCode KApplicationScopeLister::lastError() const
{
    return d->m_lastError;
}

QStringList KApplicationScopeLister::paths() const
{
    return d->m_appPaths;
}

static const auto glob = QStringLiteral("app-*");

KApplicationScopeListerPrivate::KApplicationScopeListerPrivate(KApplicationScopeLister *parent)
    : m_lastError(KApplicationScopeLister::NoError)
    , m_appPaths(QStringList())
    , q(parent)
{
    static const auto service = QStringLiteral("org.freedesktop.systemd1");
    static const auto path = QStringLiteral("/org/freedesktop/systemd1");

    qDBusRegisterMetaType<ManagerDBusUnit>();
    qDBusRegisterMetaType<ManagerDBusUnitList>();

    m_manager = new org::freedesktop::systemd1::Manager(service, path, QDBusConnection::sessionBus(), q);

    const auto *watcher = new QDBusPendingCallWatcher(m_manager->ListUnitsByPatterns({}, {glob}), q);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, q, [this](QDBusPendingCallWatcher *watcher) {
        handleGetListCallFinished(watcher);
    });

    const auto *subscribeWatcher = new QDBusPendingCallWatcher(m_manager->Subscribe(), q);
    QObject::connect(subscribeWatcher, &QDBusPendingCallWatcher::finished, q, [this](QDBusPendingCallWatcher *watcher) {
        handleSubscribeFinished(watcher);
    });

    QObject::connect(m_manager,
                     &org::freedesktop::systemd1::Manager::UnitNew,
                     q,
                     [this](const QString &name, const QDBusObjectPath &path) { handleUnitNew(name, path); });
    QObject::connect(m_manager,
                     &org::freedesktop::systemd1::Manager::UnitRemoved,
                     q,
                     [this](const QString &name, const QDBusObjectPath &path) { handleUnitRemoved(name, path); });
}

void KApplicationScopeListerPrivate::handleGetListCallFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<ManagerDBusUnitList> reply = *call;
    if (reply.isError()) {
        setError(KApplicationScopeLister::CannotListError, reply.error().message());
    } else {
        const auto unitList = reply.argumentAt<0>();
        int changes = 0;
        for (const auto &unit : unitList) {
            qCDebug(KCGROUPS_LOG) << unit.activeState;
            const auto path = unit.path.path();
            if (!m_appPaths.contains(path)) {
                m_appPaths.append(path);
                emit q->pathAdded(path, unit.id);
                changes++;
            }
        }
        if (changes > 0) {
            emit q->pathsChanged(m_appPaths);
        }
    }
    call->deleteLater();
}

void KApplicationScopeListerPrivate::handleSubscribeFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<> reply = *call;
    if (reply.isError()) {
        setError(KApplicationScopeLister::CannotSubscribeError, reply.error().message());
    }
    call->deleteLater();
}

void KApplicationScopeListerPrivate::handleUnitNew(const QString &id, const QDBusObjectPath &path)
{
    const auto pathStr = path.path();
    qCDebug(KCGROUPS_LOG) << "handleUnitNew" << id << path.path();
    static const QRegularExpression regex(QRegularExpression::wildcardToRegularExpression(glob));
    if (regex.globalMatch(id).hasNext() && !m_appPaths.contains(pathStr)) {
        m_appPaths.append(pathStr);
        emit q->pathAdded(pathStr, id);
        emit q->pathsChanged(m_appPaths);
    }
}

void KApplicationScopeListerPrivate::handleUnitRemoved(const QString &name, const QDBusObjectPath &path)
{
    const auto pathStr = path.path();
    qCDebug(KCGROUPS_LOG) << "handleUnitRemoved" << name << path.path();
    if (m_appPaths.contains(pathStr)) {
        m_appPaths.removeOne(pathStr);
        emit q->pathRemoved(pathStr);
        emit q->pathsChanged(m_appPaths);
    }
}

void KApplicationScopeListerPrivate::setError(KApplicationScopeLister::ErrorCode code, const QString &message)
{
    m_lastError = code;
    qCDebug(KCGROUPS_LOG) << "ERROR: " << message;
    emit q->errorOccurred(m_lastError);
}
