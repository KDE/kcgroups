// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include <QDebug>
#include <QElapsedTimer>
#include <QGuiApplication>
#include <QQuickView>
#include <QTimer>
#include <QQmlContext>

class CPUHog : public QObject
{
public:
    CPUHog(QObject *parent)
        : QObject(parent), frame(0)
    {
    }
public Q_SLOTS:
    void onBeforeRendering()
    {
        frame++;
        for (qlonglong i = 0; i < 100000000; i++) {
            if (i % 10000000 == 0){
            qWarning() << frame << i;}
        }
    }
private:
    qlonglong frame;
};

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    qWarning() << "SPINNER" << app.arguments();
    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:spinner.qml")));
    auto *ctxt = view.rootContext();
    ctxt->setContextProperty(QStringLiteral("name"), app.arguments().value(1, QStringLiteral("none")));
    view.show();
    //    QTimer timer(&app);
    //    timer.setInterval(1);
    //    timer.setTimerType(Qt::PreciseTimer);
    //    QElapsedTimer elapsed();
    //    qlonglong j =0;
    //    timer.callOnTimeout([&j]() {
    //        qWarning() << "spinning" << j;
    //        j++;
    //        for (qlonglong i=0; i<10000000000; i++) {
    ////            qWarning() <<i;

    //        }
    ////        int i = 0;
    ////        while (true) {
    ////            i++;
    ////        }
    //    });
    //    timer.start();
    CPUHog hog(&app);
    QObject::connect(&view, &QQuickView::beforeRendering, &hog, &CPUHog::onBeforeRendering, Qt::DirectConnection);
    return app.exec();
}
