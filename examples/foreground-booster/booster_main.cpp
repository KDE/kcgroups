// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "foregroundbooster.h"
#include "kcgroups_debug.h"
#include <KConfigCore/KConfig>
#include <KConfigCore/KConfigGroup>
#include <QEventLoop>
#include <QGuiApplication>
#include <QLoggingCategory>

int main(int argc, char **argv)
{
    QLoggingCategory::setFilterRules(QStringLiteral("kf5.kcgroups.debug=true"));
    QGuiApplication::setDesktopSettingsAware(false);
    QGuiApplication app(argc, argv);

    const KConfig cfg(QStringLiteral("kcgroupsrc"));
    const auto group = cfg.group("Foreground Booster");
    ForegroundBooster booster(group.readEntry<qulonglong>("boostedCpuWeight", 10000), &app);
    return app.exec();
}
