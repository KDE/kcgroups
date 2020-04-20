#ifndef FOREGROUNDBOOSTER_H
#define FOREGROUNDBOOSTER_H
#include <QtGui/qwindowdefs.h>
#include <QObject>
#include <QHash>

class KWindowSystem;
class KApplicationScope;

class ForegroundBooster : public QObject
{
public:
    ForegroundBooster(QObject *parent = nullptr);
public Q_SLOTS:
    void onActiveWindowChanged(WId id);

private:
    KWindowSystem *m_kws;
    KApplicationScope *m_currentApp;
    QHash<WId, KApplicationScope*> m_appCache;
};

#endif // FOREGROUNDBOOSTER_H
