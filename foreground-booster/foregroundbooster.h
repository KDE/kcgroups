#ifndef FOREGROUNDBOOSTER_H
#define FOREGROUNDBOOSTER_H
#include <QHash>
#include <QObject>
#include <QtGui/qwindowdefs.h>

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
    QHash<WId, KApplicationScope *> m_appCache;
};

#endif // FOREGROUNDBOOSTER_H
