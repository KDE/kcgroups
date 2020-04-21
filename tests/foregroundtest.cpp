// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QTimer>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    //    QQuickView view;
    //    view.setSource(QUrl(QStringLiteral("qrc:spinner.qml")));
    //    view.show();
    QProcess booster(&app);
    booster.setProgram(QStringLiteral("./foreground_booster"));
    booster.setProcessChannelMode(QProcess::ForwardedChannels);
    booster.start();
    qDebug() << booster.error() << booster.errorString();

    QProcess left(&app);
    left.setProgram(QStringLiteral("./spinner"));
    left.setArguments({QStringLiteral("left")});
    left.setProcessChannelMode(QProcess::ForwardedChannels);

    //    left.arguments().append(QStringLiteral("./spinner"));
    left.start();
    qDebug() << left.error() << left.errorString();

    QProcess right(&app);
    right.setProgram(QStringLiteral("./spinner"));
    right.setArguments({QStringLiteral("right")});
    right.setProcessChannelMode(QProcess::ForwardedChannels);
    right.start();
    qDebug() << right.error() << right.errorString();

//    app.connect(&QCoreApplication::aboutToQuit, [](){});
    return app.exec();
}
