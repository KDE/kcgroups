#include "kapplicationscope.h"
#include "kcgroups_debug.h"
#include "managerinterface.h"
#include <QtCore>
#include <foregroundbooster.h>
#include <kwindowsystem.h>

using namespace org::freedesktop::systemd1;

ForegroundBooster::ForegroundBooster(QObject *parent)
    : QObject(parent)
    , m_kws(KWindowSystem::self())
    , m_manager(new Manager(QStringLiteral("org.freedesktop.systemd1"),
                            QStringLiteral("/org/freedesktop/systemd1"),
                            QDBusConnection::sessionBus(),
                            this))
{
    onActiveWindowChanged(KWindowSystem::activeWindow());
    connect(m_kws, &KWindowSystem::activeWindowChanged, this, &ForegroundBooster::onActiveWindowChanged);
}

KApplicationScope *ForegroundBooster::getAppFromPid(uint pid)
{
    auto reply = m_manager->GetUnitByPID(pid);
    reply.waitForFinished();

    if (reply.isError()) {
        qCWarning(KCGROUPS_LOG) << "Cannot get app" << reply.error().message();
        return nullptr;
    } else {
        return new KApplicationScope(reply.argumentAt<0>().path(), this);
    }
}

void ForegroundBooster::onActiveWindowChanged(WId id)
{
    const auto prevApp = m_currentApp;
    if (m_appCache.contains(id)) {
        m_currentApp = m_appCache[id];
    } else {
        const auto info = KWindowInfo(id, NET::WMPid | NET::WMName, NET::WM2DesktopFileName);
        qDebug() << "New window focused:" << info.name() << info.desktopFileName() << info.pid();

        m_currentApp = getAppFromPid(info.pid());
        m_appCache[id] = m_currentApp;
    }
    if (prevApp != m_currentApp) {
        if (m_currentApp != nullptr) {
            m_currentApp->setCpuQuota(10);
        }
    }
}
