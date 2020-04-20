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
    connect(m_kws, &KWindowSystem::windowRemoved, this, &ForegroundBooster::onWindowRemoved);
}

void ForegroundBooster::onWindowRemoved(WId id) {
    const auto app = m_appCache.take(id);
    if (app) {
        qDebug() << "Removing app from cache";
        delete app;
    }
}

void ForegroundBooster::onActiveWindowChanged(WId id)
{
    const auto prevApp = m_currentApp;
    if (m_appCache.contains(id)) {
        m_currentApp = m_appCache[id];
        if (m_currentApp == nullptr) {
            qDebug() << "Previous non-systemd window focused";
        } else {
            qDebug() << "Old window focused:" << m_currentApp->id();
        }
    } else {
        const auto info = KWindowInfo(id, NET::WMPid | NET::WMName, NET::WM2DesktopFileName);
        qDebug() << "New window focused:" << info.name() << info.desktopFileName() << info.pid();

        m_currentApp = KApplicationScope::fromPid(info.pid(), this);
        m_appCache[id] = m_currentApp;
    }
    if (prevApp != m_currentApp) {
        if (prevApp != nullptr) {
            prevApp->setCpuWeight(OptionalQULongLong());
        }
        if (m_currentApp != nullptr) {
            m_currentApp->setCpuWeight(10000);
        }
    }
}
