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
    KApplicationScope *m_currentApp;
    KWindowInfo *m_info;
    QHash<WId, KWindowInfo *> m_infoByWid;
    QHash<uint, KApplicationScope *> m_appsByPid;
};

#endif // FOREGROUNDBOOSTER_H
