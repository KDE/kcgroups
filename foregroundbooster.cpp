// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "foregroundbooster.h"
#include <KApplicationScope>
#include <QtCore>
#include <abstracttasksmodel.h>
#include <algorithm>

using namespace TaskManager;

ForegroundBooster::ForegroundBooster(QObject *parent)
    : QObject(parent)
    , m_tasksModel(new TasksModel(this))
    , m_settings(new BoosterSettings(this))

{
    connect(m_tasksModel, &TasksModel::activeTaskChanged, this, &ForegroundBooster::onActiveWindowChanged);
    connect(m_tasksModel, &TasksModel::rowsAboutToBeRemoved, this, &ForegroundBooster::onWindowRemoved);
}

ForegroundBooster::~ForegroundBooster()
{
}

void ForegroundBooster::onWindowRemoved(const QModelIndex &parent, int first, int last)
{
    for (int row = first; row <= last; row++) {
        const auto index = m_tasksModel->index(row, 0, parent);
        const auto pid = m_tasksModel->data(index, AbstractTasksModel::AppPid).toUInt();

        if (m_appsByPid.contains(pid)) {
            const auto app = m_appsByPid.value(pid);
            if (app) {
                qDebug() << "Removing" << app->id() << "from cache";
            }
            delete app;
            m_appsByPid.remove(pid);
        }
    }
}

void ForegroundBooster::onActiveWindowChanged()
{
    const auto activeTaskIndex = m_tasksModel->activeTask();
    const auto appid = m_tasksModel->data(activeTaskIndex, AbstractTasksModel::AppId).toString();
    const auto pid = m_tasksModel->data(activeTaskIndex, AbstractTasksModel::AppPid).toUInt();
    const auto isWindow = m_tasksModel->data(activeTaskIndex, AbstractTasksModel::IsWindow).toBool();

    qDebug() << "";
    if (!isWindow) {
        qWarning() << "NOT WINDOW" << pid;

        return;
    }

    if (pid == m_currentPid) {
        qWarning() << "SAME PID" << appid;
        return;
    }

    const auto prevApp = m_appsByPid.value(m_currentPid);
    qDebug() << "Switching from" << m_currentAppid << "to" << appid;

    KApplicationScope *currentApp;

    if (m_appsByPid.contains(pid)) {
        currentApp = m_appsByPid[pid];
        if (currentApp == nullptr) {
            qDebug() << "Previous unmanaged app focused: appid =" << appid << ", pid =" << pid;
        } else {
            qDebug() << "Previous  systemd  app focused:" << currentApp->id() << ", appid =" << appid << ", pid =" << pid;
        }
    } else {
        qDebug() << "New window focused: appid =" << appid << ", pid =" << pid;
        currentApp = KApplicationScope::fromPid(pid, this);
        m_appsByPid[pid] = currentApp;
        if (currentApp == nullptr) {
            qDebug() << "This new window is not managed by systemd";
        }
    }

    if (prevApp != currentApp) {
        if (prevApp != nullptr) {
            qInfo() << "resetting" << prevApp->id() << "weight to default";
            prevApp->setCpuWeight(OptionalQULongLong());
        }
        if (currentApp != nullptr) {
            qInfo() << "setting" << currentApp->id() << "weight to" << (float)m_settings->boostedCpuWeight() / 100.
                    << "times normal weight";
            currentApp->setCpuWeight(m_settings->boostedCpuWeight());
        }
    } else {
        qDebug() << "Changed to different window of same app";
    }
    m_currentPid = pid;
    m_currentAppid = appid;
}