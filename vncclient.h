#ifndef VNCCLIENT_H
#define VNCCLIENT_H

#include <QWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QKeyEvent>
#include <qvncclientwidget.h>
#include <QDebug>
#include <QSizePolicy>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logVncClient)

class VncClient : public QWidget
{

public:
    VncClient(QWidget *parent = nullptr);
    virtual ~VncClient() {}

private:
    QVNCClientWidget *vnc;
};

#endif // VNCCLIENT_H

