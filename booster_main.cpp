#include "foregroundbooster.h"
#include "kcgroups_debug.h"
#include <QGuiApplication>
#include <QEventLoop>
#include <QLoggingCategory>

int main (int argc, char **argv) {
        QLoggingCategory::setFilterRules(QStringLiteral("kf5.kcgroups.debug=true"));

//    KCGROUPS_LOG().setEnabled(QtDebugMsg, true);
//    qWarning() << KCGROUPS_LOG.categoryName() << KCGROUPS_LOG.filt;
//    QLoggingCategory::setFilterRules(QStringLiteral("%1=true").format(KCGROUPS_LOG.categoryName()));
    QGuiApplication app(argc, argv);
    ForegroundBooster booster(&app);
    QEventLoop loop;
    loop.exec();
    return app.exec();
}
