#include "mdisubwindow.h"
#include "mainwindow.h"
#include <QKeyEvent>
#include <QDebug>
#include <QMdiArea>
#include <QApplication>
#include <QShortcut>

MdiSubWindow::MdiSubWindow(QWidget *parent) : QMdiSubWindow(parent) {
   QShortcut* shortcut_close_window = new QShortcut(QKeySequence("Ctrl+X"), this);
   connect(shortcut_close_window, &QShortcut::activated, this, &MdiSubWindow::close);
}
bool MdiSubWindow::event(QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        mdiArea()->activateWindow();
        widget()->setFocus();
    }
    return QMdiSubWindow::event(event);
}
