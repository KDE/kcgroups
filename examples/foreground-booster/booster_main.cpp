// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "foregroundbooster.h"
#include "kcgroups_debug.h"
#include <KDBusService>
#include <QEventLoop>
#include <QGuiApplication>
#include <QLoggingCategory>

int main(int argc, char **argv)
{
    QLoggingCategory::setFilterRules(QStringLiteral("kf5.kcgroups.debug=true"));
    QGuiApplication::setDesktopSettingsAware(false);
    QGuiApplication app(argc, argv);

    app.setApplicationName(QStringLiteral("foreground-booster"));
    app.setOrganizationDomain(QStringLiteral("kde.org"));
    KDBusService service(KDBusService::Unique);

    ForegroundBooster booster(&app);
    return app.exec();
}
