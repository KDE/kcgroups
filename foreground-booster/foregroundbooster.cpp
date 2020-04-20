#include "kapplicationscope.h"
#include <QtCore>
#include <foregroundbooster.h>
#include <kwindowsystem.h>

ForegroundBooster::ForegroundBooster(QObject *parent)
    : QObject(parent)
    , m_kws(KWindowSystem::self())
    , m_currentApp(nullptr)
    , m_info(nullptr)
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
            bool unused = true;
            for (auto it = m_infoByWid.constBegin(); it != m_infoByWid.constEnd(); it++) {
                if (it.key() != wid && it.value()->pid() == pid) {
                    unused = false;
                    qDebug() << app->id() << "still has at least one window, not deleting";
                    break;
                }
            }
            if (unused) {
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
    const auto prevApp = m_currentApp;
    const auto prevInfo = m_info;

    m_info = m_infoByWid[wid];
    if (!m_info) {
        m_info = new KWindowInfo(wid, NET::WMPid | NET::WMName, NET::WM2DesktopFileName);
        m_infoByWid[wid] = m_info;
    }
    const auto pid = m_info->pid();

    if (m_appsByPid.contains(pid)) {
        m_currentApp = m_appsByPid[pid];
        if (m_currentApp == nullptr) {
            qDebug() << "Previous non-systemd app focused" << m_info->name() << m_info->desktopFileName() << pid;
        } else {
            qDebug() << "Previous app focused:" << m_currentApp->id();
        }
    } else {
        qDebug() << "New window focused:" << m_info->name() << m_info->desktopFileName() << pid;
        m_currentApp = KApplicationScope::fromPid(pid, this);
        m_appsByPid[pid] = m_currentApp;
        if (m_currentApp == nullptr) {
            qDebug() << "New Window not managed by systemd";
        }
    }

    if (prevApp != m_currentApp) {
        if (prevApp != nullptr) {
            qInfo() << "resetting" << prevInfo->name() << prevApp->id() << "weight to default";
            prevApp->setCpuWeight(OptionalQULongLong());
        }
        if (m_currentApp != nullptr) {
            qInfo() << "setting" << m_info->name() << "weight to max";
            m_currentApp->setCpuWeight(10000);
        }
    } else {
        if (m_info == prevInfo) {
            qDebug() << "Same app, same window, weird";
        } else {
            qDebug() << "Same app but different window" << (prevInfo ? prevInfo->name() : QStringLiteral("none")) << m_info->name() ;
        }
    }
}
