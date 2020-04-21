// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
