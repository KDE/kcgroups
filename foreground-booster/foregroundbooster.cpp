#include "kapplicationscope.h"
#include <QtCore>
#include <algorithm>
#include <foregroundbooster.h>
#include <kwindowsystem.h>

ForegroundBooster::ForegroundBooster(QObject *parent)
    : QObject(parent)
    , m_kws(KWindowSystem::self())
{
    onActiveWindowChanged(KWindowSystem::activeWindow());
    connect(m_kws, &KWindowSystem::activeWindowChanged, this, &ForegroundBooster::onActiveWindowChanged);
    connect(m_kws, &KWindowSystem::windowRemoved, this, &ForegroundBooster::onWindowRemoved);
}

ForegroundBooster::~ForegroundBooster()
{
    qDeleteAll(m_infoByWid);
}

void ForegroundBooster::onWindowRemoved(WId wid)
{
    const auto info = m_infoByWid.take(wid);
    if (info) {
        const auto pid = info->pid();
        const auto app = m_appsByPid.value(pid);

        if (app) {
            if (std::any_of(
                    m_infoByWid.constBegin(), m_infoByWid.constEnd(), [info, pid](const KWindowInfo *otherInfo) {
                        return otherInfo != info && otherInfo->pid() == pid;
                    })) {
                qDebug() << app->id() << "still has at least one window, not deleting";
            } else {
                qDebug() << "Removing" << app->id() << "from cache";
                m_appsByPid.remove(pid);
                delete app;
            }
        }
        delete info;
    }
}

void ForegroundBooster::onActiveWindowChanged(WId wid)
{
    const auto prevInfo = m_infoByWid.value(currentWid);
    const auto prevApp = m_appsByPid.value(m_currentPid);

    auto info = m_infoByWid[wid];
    if (!info) {
        info = new KWindowInfo(wid, NET::WMPid | NET::WMName | NET::WMWindowType, NET::WM2DesktopFileName);
        m_infoByWid[wid] = info;
    }

    if (!info->valid()) {
        qWarning() << "INVALID";
        return;
    }

    qDebug() << "TYPE" << info->windowType(NET::AllTypesMask);

    m_currentPid = info->pid();

    KApplicationScope *currentApp;

    if (m_appsByPid.contains(m_currentPid)) {
        currentApp = m_appsByPid[m_currentPid];
        if (currentApp == nullptr) {
            qDebug() << "Previous non-systemd app focused" << info->name() << info->desktopFileName() << m_currentPid;
        } else {
            qDebug() << "Previous app focused:" << currentApp->id();
        }
    } else {
        qDebug() << "New window focused:" << info->name() << info->desktopFileName() << m_currentPid;
        currentApp = KApplicationScope::fromPid(m_currentPid, this);
        m_appsByPid[m_currentPid] = currentApp;
        if (currentApp == nullptr) {
            qDebug() << "New Window not managed by systemd";
        }
    }

    if (prevApp != currentApp) {
        if (prevApp != nullptr) {
            qInfo() << "resetting" << prevInfo->name() << prevApp->id() << "weight to default";
            prevApp->setCpuWeight(OptionalQULongLong());
        }
        if (currentApp != nullptr) {
            qInfo() << "setting" << info->name() << "weight to max";
            currentApp->setCpuWeight(10000);
        }
    } else {
        if (wid == currentWid) {
            qDebug() << "Same app, same window, weird";
        } else {
            qDebug() << "Same app but different window" << (prevInfo ? prevInfo->name() : QStringLiteral("[none]"))
                     << info->name();
        }
    }
    currentWid = wid;
}
