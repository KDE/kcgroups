// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QTimer>

class ForegroundTester : public QObject
{
public:
    ForegroundTester(QObject *parent)
        : QObject(parent)
        , m_booster(this)
        , m_managed1(this)
        , m_managed2(this)
    {
        m_booster.setProgram(QStringLiteral("./foreground_booster"));
        m_booster.setProcessChannelMode(QProcess::ForwardedChannels);

        m_unmanaged.setProgram(QStringLiteral("./spinner"));
        m_unmanaged.setArguments({QStringLiteral("unmanaged")});

        m_managed1.setProgram(QStringLiteral("systemd-run"));
        m_managed1.setArguments({QStringLiteral("--user"),
                                 QStringLiteral("--scope"),
                                 QStringLiteral("./spinner"),
                                 QStringLiteral("managed 1")});
        m_managed1.setProcessChannelMode(QProcess::ForwardedChannels);

        m_managed2.setProgram(QStringLiteral("systemd-run"));
        m_managed2.setArguments({QStringLiteral("--user"),
                                 QStringLiteral("--scope"),
                                 QStringLiteral("./spinner"),
                                 QStringLiteral("managed 2")});
        m_managed2.setProcessChannelMode(QProcess::ForwardedChannels);
        m_unmanaged.start();
        QTimer::singleShot(100, [this]() { m_managed1.start(); });
        QTimer::singleShot(200, [this]() { m_managed2.start(); });
        QTimer::singleShot(300, [this]() { m_booster.start(); });
    }
public Q_SLOTS:
    void onStopping()
    {
        m_managed1.kill();
        m_managed2.kill();
        m_booster.kill();
    }

private:
    QProcess m_booster;
    QProcess m_managed1;
    QProcess m_managed2;
    QProcess m_unmanaged;
};

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    ForegroundTester tester(&app);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &tester, &ForegroundTester::onStopping);
    return app.exec();
}
