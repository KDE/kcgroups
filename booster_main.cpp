#include "foregroundbooster.h"
#include "kcgroups_debug.h"
#include <QGuiApplication>
#include <QEventLoop>
#include <QLoggingCategory>

int main (int argc, char **argv) {
    QLoggingCategory::setFilterRules(QStringLiteral("kf5.kcgroups.debug=true"));
    QGuiApplication app(argc, argv);
    ForegroundBooster booster(&app);
    return app.exec();
}
