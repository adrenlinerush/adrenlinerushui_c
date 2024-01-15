#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFont>
#include <QIcon>
#include <QKeySequence>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QProcess>
#include <QResizeEvent>
#include <QShortcut>
#include <QStatusBar>
#include <qtermwidget5/qtermwidget.h>
#include <QThread>
#include <QTimer>
#include <QWidget>
#include <browser.h>
#include <videoplayer.h>
#include <calculator.h>
#include <filebrowser.h>
#include <statusbar.h>
#include <QDesktopWidget>
//#include <vncclient.h>
#include <mdisubwindow.h>
#include <mdiarea.h>
#include <mainwindow.h>

class AdrenlinerushUI : public QApplication {
public:
    AdrenlinerushUI(int& argc, char** argv) : QApplication(argc, argv) {}

    bool notify(QObject* receiver, QEvent* event) override {
        try {
            return QApplication::notify(receiver, event);
        } catch (const std::exception& e) {
            qDebug() << "Exception caught: " << e.what();
            return true;
        }
    }
};

int main(int argc, char *argv[])
{
    setenv("TERM", "konsole-256color", 1); // 1 means overwrite if already exists
    AdrenlinerushUI a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    MainWindow w1;
    //w1.setWindowFlags( Qt::Window );
    //w1.setAttribute( Qt::WA_DeleteOnClose, true );
    MainWindow w2;
    //w2.setWindowFlags( Qt::Window );
    //w2.setAttribute( Qt::WA_DeleteOnClose, true );

    QFont font = QApplication::font();
    font.setFamily("Terminus");
    font.setPointSize(6);
    a.setFont(font);

    QDesktopWidget *widget = QApplication::desktop();
    qDebug() << "Number of screens:" << widget->screenCount();
    for (int i = 0; i < widget->screenCount(); i++) {
        qDebug() << "Geometry:" << widget->screenGeometry(i);
    }
    // Get geometry of Secondary screen
    //qDebug() << "Move to:" << rect.width() << ":" << rect.y();
    //qDebug() << "Move to:" << rect.x() << ":" << rect.y();
    //w1.setStyleSheet("QMainWindow {background: 'yellow';}");
    QRect rect = widget->screenGeometry(0);
    w1.setFixedSize(rect.width(), rect.height());
    w1.addMediaPlayer();
    w1.add_terminal();
    w1.add_tabbed_browser();
    w1.tileSubWindows();
    w1.show();
    //Move w2 to Secondary Screen
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
    //delete &w2;
    //delete &w1;
    return r;

}

