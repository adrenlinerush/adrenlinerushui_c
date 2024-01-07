#include "mdisubwindow.h"
#include <QKeyEvent>
#include <QDebug>
#include <QMdiArea>
#include <QApplication>

MdiSubWindow::MdiSubWindow(QWidget *parent) : QMdiSubWindow(parent) {
}
bool MdiSubWindow::event(QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        mdiArea()->activateWindow();
        widget()->setFocus();
    }
    //qApp->processEvents();
    return QMdiSubWindow::event(event);
}
