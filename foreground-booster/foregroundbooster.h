#ifndef FOREGROUNDBOOSTER_H
#define FOREGROUNDBOOSTER_H
#include <QHash>
#include <QObject>
#include <QtGui/qwindowdefs.h>

class KWindowSystem;
class KWindowInfo;
class KApplicationScope;

class ForegroundBooster : public QObject
{
public:
    ForegroundBooster(QObject *parent = nullptr);
    ~ForegroundBooster();

public Q_SLOTS:
    void onActiveWindowChanged(WId id);
    void onWindowRemoved(WId id);

private:
    KWindowSystem *m_kws;
    uint m_currentPid;
    WId m_currentWid;
    QHash<WId, KWindowInfo *> m_infoByWid;
    QHash<uint, KApplicationScope *> m_appsByPid;
    QMultiHash<uint, WId> m_widsByPid;
};

#endif // FOREGROUNDBOOSTER_H
