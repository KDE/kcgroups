// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include <QDebug>
#include <QElapsedTimer>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QTimer>

class CPUHog : public QObject
{
public:
    CPUHog(QObject *parent)
        : QObject(parent)
        , frame(0)
    {
    }
public Q_SLOTS:
    void onBeforeRendering()
    {
        frame++;
        for (qlonglong i = 0; i < 30000000; i++) {
            if (i % 10000000 == 0 && frame % 100 == 0) {
                qWarning() << frame << i;
            }
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
    CPUHog hog(&app);
    QObject::connect(&view, &QQuickView::beforeRendering, &hog, &CPUHog::onBeforeRendering, Qt::DirectConnection);
    return app.exec();
}
