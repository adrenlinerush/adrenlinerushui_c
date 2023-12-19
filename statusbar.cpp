#include "statusbar.h"
#include <QCoreApplication>
#include <QThread>
#include <QProcess>
#include <QDebug>

StatusBar::StatusBar(QStatusBar* status_bar, QObject* parent)
    : QObject(parent), statusBar(status_bar), isAcpi(QProcess::execute("acpi") == 0), timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, this, &StatusBar::updateStatusBar);
    timer->start(1000); // Update every 1000 milliseconds (1 second)
}

StatusBar::~StatusBar()
{
    if (timer->isActive())
        timer->stop();
}

void StatusBar::updateStatusBar()
{
    try
    {
        QDateTime current = QDateTime::currentDateTime();
        QString statusText = current.toString("MM-dd-yyyy HH:mm");

        if (isAcpi)
        {
            QString battery = getBatteryInfo();
            statusText += "\t\t" + battery;
        }

        statusBar->showMessage(statusText);
    }
    catch (const std::exception& e)
    {
        qDebug() << "StatusBar.updateStatusBar";
        qDebug() << e.what();
        qDebug() << "message";
    }
}

QString StatusBar::getBatteryInfo()
{
    QProcess acpiProcess;
    acpiProcess.start("acpi");
    acpiProcess.waitForFinished();

    if (acpiProcess.exitCode() == 0)
    {
        QByteArray batteryOutput = acpiProcess.readAllStandardOutput();
        return batteryOutput.trimmed();
    }
    else
    {
        return "Error retrieving battery info";
    }
}

