// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "kapplicationscopelistmodel.h"
#include <QDebug>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QTimer>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    KApplicationScopeListModel model(&app);
    QQuickView view;
    QQmlContext *ctxt = view.rootContext();
    ctxt->setContextProperty(QStringLiteral("appList"), &model);
    view.setSource(QUrl(QStringLiteral("qrc:main.qml")));
    view.show();
    return app.exec();
}
