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
        , m_left(this)
        , m_right(this)
    {
        m_booster.setProgram(QStringLiteral("./foreground_booster"));
        m_booster.setProcessChannelMode(QProcess::ForwardedChannels);
        m_booster.start();

        m_left.setProgram(QStringLiteral("systemd-run"));
        m_left.setArguments(
            {QStringLiteral("--user"), QStringLiteral("--scope"), QStringLiteral("./spinner"), QStringLiteral("left")});
        m_left.setProcessChannelMode(QProcess::ForwardedChannels);
        m_left.start();

        m_right.setProgram(QStringLiteral("systemd-run"));
        m_right.setArguments({QStringLiteral("--user"),
                              QStringLiteral("--scope"),
                              QStringLiteral("./spinner"),
                              QStringLiteral("right")});
        m_right.setProcessChannelMode(QProcess::ForwardedChannels);
        m_right.start();
    }
public Q_SLOTS:
    void onStopping()
    {
        m_left.kill();
        m_right.kill();
        m_booster.kill();
    }

private:
    QProcess m_booster;
    QProcess m_left;
    QProcess m_right;
};

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    ForegroundTester tester(&app);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &tester, &ForegroundTester::onStopping);
    return app.exec();
}
