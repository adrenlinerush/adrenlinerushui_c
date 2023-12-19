#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QObject>
#include <QStatusBar>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QProcess>
#include <QThread>
#include <QCoreApplication>

class StatusBar : public QObject
{

public:
    explicit StatusBar(QStatusBar* status_bar, QObject* parent = nullptr);
    ~StatusBar();

public slots:
    void updateStatusBar();

private:
    QStatusBar* statusBar;
    bool isAcpi;
    QTimer* timer;

    QString getBatteryInfo();
};

#endif // STATUSBAR_H

