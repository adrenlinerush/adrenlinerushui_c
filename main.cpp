#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QFont>
#include <QWidget>
#include <QDesktopWidget>
//#include <vncclient.h>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    setenv("TERM", "konsole-256color", 1); 
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    MainWindow w1;
    MainWindow w2;

    QFont font = QApplication::font();
    font.setFamily("Terminus");
    font.setPointSize(6);
    a.setFont(font);

    QDesktopWidget *widget = QApplication::desktop();
    qDebug() << "Number of screens:" << widget->screenCount();
    for (int i = 0; i < widget->screenCount(); i++) {
        qDebug() << "Geometry:" << widget->screenGeometry(i);
    }
    QRect rect = widget->screenGeometry(0);
    w1.setFixedSize(rect.width(), rect.height());
    w1.addMediaPlayer();
    w1.add_terminal();
    w1.add_tabbed_browser();
    w1.tileSubWindows();
    w1.show();
    if (widget->screenCount() > 1) {
        QRect rect = widget->screenGeometry(1);
	w1.setOtherScreen(&w2);
	w2.setOtherScreen(&w1);
        w2.move(rect.width(), rect.y());
        w2.setFixedSize(rect.width(), rect.height());
        w2.addFileBrowser();
        w2.tileSubWindows();
        w2.show();
    }
    int r = a.exec();
    return r;

}

