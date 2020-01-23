// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef KAPPLICATIONSCOPELISTER_P_H
#define KAPPLICATIONSCOPELISTER_P_H
#include "kapplicationscopelister.h"
#include "managerinterface.h"
#include <QtDBus/QDBusPendingCall>

class KApplicationScopeListerPrivate
{
public:
    KApplicationScopeListerPrivate(KApplicationScopeLister *parent);
    KApplicationScopeLister::ErrorCode m_lastError;
    QStringList m_appPaths;

private:
    KApplicationScopeLister *q;
    org::freedesktop::systemd1::Manager *m_manager;
    void setError(KApplicationScopeLister::ErrorCode code, const QString &message = QString());
    void handleGetListCallFinished(QDBusPendingCallWatcher *call);
    void handleSubscribeFinished(QDBusPendingCallWatcher *call);
    void handleUnitNew(const QString &name, const QDBusObjectPath &path);
    void handleUnitRemoved(const QString &name, const QDBusObjectPath &path);
};

#endif // KAPPLICATIONSCOPELISTER_P_H
