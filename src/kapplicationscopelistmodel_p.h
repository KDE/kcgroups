// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef KAPPLICATIONSCOPELISTMODEL_P_H
#define KAPPLICATIONSCOPELISTMODEL_P_H

#include "kapplicationscope.h"
#include "kapplicationscopelister.h"
#include "kapplicationscopelistmodel.h"

class KApplicationScopeListModelPrivate
{
public:
    explicit KApplicationScopeListModelPrivate(KApplicationScopeListModel *parent);
    explicit KApplicationScopeListModelPrivate(KApplicationScopeLister *lister, KApplicationScopeListModel *parent);
    QList<KApplicationScope *> m_apps;

private:
    KApplicationScopeListModel *q_ptr;
    KApplicationScopeLister *m_lister;
    void handleNewApp(const QString &path, const QString &id);
    void handleRemovedApp(const QString &path);
};

#endif // KAPPLICATIONSCOPELISTMODEL_P_H
