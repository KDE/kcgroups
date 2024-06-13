// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef FOREGROUNDBOOSTER_H
#define FOREGROUNDBOOSTER_H
#include "boostersettings.h"
#include <QHash>
#include <QObject>
#include <tasksmodel.h>

class KApplicationScope;

class ForegroundBooster : public QObject
{
public:
    ForegroundBooster(QObject *parent = nullptr);
    ~ForegroundBooster();

public Q_SLOTS:
    void onActiveWindowChanged();
    void onWindowRemoved(const QModelIndex &parent, int first, int last);

private:
    TaskManager::TasksModel *m_tasksModel;

    BoosterSettings *m_settings;
    uint m_currentPid;
    QString m_currentAppid;
    QHash<uint, KApplicationScope *> m_appsByPid;
};

#endif // FOREGROUNDBOOSTER_H