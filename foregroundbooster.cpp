#include "kapplicationscope.h"
#include <QtCore>
#include <foregroundbooster.h>
#include <kwindowsystem.h>

ForegroundBooster::ForegroundBooster(QObject *parent)
    : QObject(parent)
    , m_kws(KWindowSystem::self())
{
    onActiveWindowChanged(KWindowSystem::activeWindow());
    connect(m_kws, &KWindowSystem::activeWindowChanged, this, &ForegroundBooster::onActiveWindowChanged);
}



void ForegroundBooster::onActiveWindowChanged(WId id)
{
    const auto prevApp = m_currentApp;
    if (m_appCache.contains(id)) {
        m_currentApp = m_appCache[id];
    } else {
        const auto info = KWindowInfo(id, NET::WMPid | NET::WMName, NET::WM2DesktopFileName);
        qDebug() << "New window focused:" << info.name() << info.desktopFileName() << info.pid();

        m_currentApp = KApplicationScope::fromPid(info.pid(), this);
        m_appCache[id] = m_currentApp;
    }
    if (prevApp != m_currentApp) {
        if (m_currentApp != nullptr) {
            m_currentApp->setCpuQuota(10);
        }
    }
}
