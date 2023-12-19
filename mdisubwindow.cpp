#include "mdisubwindow.h"
#include <QKeyEvent>
#include <QDebug>
#include <QCoreApplication>

MdiSubWindow::MdiSubWindow(QWidget *parent) : QMdiSubWindow(parent) {
}
void MdiSubWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::ActivationChange) {
        if (isActiveWindow()) {
            // Subwindow is activated, set focus to the internal widget
            widget()->setFocus();
        }
    }

    QMdiSubWindow::changeEvent(event);
}
