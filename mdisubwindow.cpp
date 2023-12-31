#include "mdisubwindow.h"
#include "mainwindow.h"
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
        try {
             auto mw = dynamic_cast<MainWindow*>(mdiArea()->parent());
             mw->paintOtherScreen();
        } catch (const std::exception& e) {
            qDebug() << "Exception caught: " << e.what();
        }	
    }
    return QMdiSubWindow::event(event);
}
void MdiSubWindow::changeEvent(QEvent *event) {
    qDebug() << event->type();
    if (event->type() == QEvent::WindowStateChange) {
	qDebug() << "WindowStateChange.";
	if (widget()->hasFocus()) {
	try {
             auto mw = dynamic_cast<MainWindow*>(mdiArea()->parent());
	     mw->paintOtherScreen();
        } catch (const std::exception& e) {
            qDebug() << "Exception caught: " << e.what();
        }} 
    }
    QMdiSubWindow::changeEvent(event);

}
