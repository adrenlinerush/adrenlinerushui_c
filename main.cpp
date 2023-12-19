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

class MDIArea : public QMdiArea {
public:
    MDIArea(const QPixmap& backgroundPixmap, QWidget* parent = nullptr)
        : QMdiArea(parent), backgroundPixmap_(backgroundPixmap), centered_(false) {}

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(viewport());
        if (!centered_) {
            painter.drawPixmap(0, 0, width(), height(), backgroundPixmap_);
        } else {
            painter.fillRect(event->rect(), palette().color(QPalette::Window));
            qreal x = (width() - displayPixmap_.width()) / 2.0;
            qreal y = (height() - displayPixmap_.height()) / 2.0;
            painter.drawPixmap(x, y, displayPixmap_);
        }
    }

    void resizeEvent(QResizeEvent* event) override {
        displayPixmap_ = backgroundPixmap_.scaled(event->size(), Qt::KeepAspectRatio);
    }

private:
    QPixmap backgroundPixmap_;
    QPixmap displayPixmap_;
    bool centered_;
};

/*class FileBrowser {};
class VncClient {};
*/
class MainWindow : public QMainWindow {

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        try {
	    //setupFileLogging("~/ui.log");
            setWindowIcon(QIcon("adrenaline.png"));

            shortcut_quit = new QShortcut(QKeySequence("Ctrl+Q"), this);
            connect(shortcut_quit, &QShortcut::activated, qApp, &QApplication::quit);

            bg_img = QPixmap("adrenaline.jpg");
            mdi = new MDIArea(bg_img);
            connect(mdi, &MDIArea::subWindowActivated, this, &MainWindow::update_window_list);

            shortcut_next_window = new QShortcut(QKeySequence("Alt+Tab"), this);
            connect(shortcut_next_window, &QShortcut::activated, mdi, &MDIArea::activateNextSubWindow);

            shortcut_tile_windows = new QShortcut(QKeySequence("Alt+T"), this);
            connect(shortcut_tile_windows, &QShortcut::activated, mdi, &MDIArea::tileSubWindows);

            shortcut_restore_window = new QShortcut(QKeySequence("Alt+R"), this);
            connect(shortcut_restore_window, &QShortcut::activated, [this]() {
                mdi->activeSubWindow()->setWindowState(Qt::WindowNoState);
            });

            shortcut_maximize_window = new QShortcut(QKeySequence("Alt+M"), this);
            connect(shortcut_maximize_window, &QShortcut::activated, [this]() {
                mdi->activeSubWindow()->setWindowState(Qt::WindowMaximized);
            });

            setCentralWidget(mdi);
            add_terminal();
            add_tabbed_browser();

            QMenuBar* bar = menuBar();

            QMenu* start = bar->addMenu("Start");
            start->addAction("Quit");
            start->addAction("Terminal");
            start->addAction("Web Browser");
	    start->addAction("Media Player");
            start->addAction("Calculator");
            start->addAction("File Browser");
            // Add other actions...
	    connect(start, &QMenu::triggered, this, &MainWindow::start);

            QMenu* view = bar->addMenu("View");
            view->addAction("Tile");
            view->addAction("Cascade");
            connect(view, &QMenu::triggered, this, &MainWindow::view);

            windows = bar->addMenu("Window");
            connect(windows, &QMenu::triggered, this, &MainWindow::window_activate);

            start_status_bar();

            mdi->tileSubWindows();
            show();
        } catch (const std::exception& e) {
            qDebug() << "MainWindow::__init__";
            qDebug() << e.what();
        }
    }

private:
    void start_status_bar() {
        try {
            qDebug() << "App::start_status_bar";
            status_thread = new QThread;
            status_worker = new StatusBar(statusBar());
            status_worker->moveToThread(status_thread);
            connect(status_thread, &QThread::started, status_worker, &StatusBar::updateStatusBar);
            qDebug() << "About to start status thread";
            status_thread->start();
        } catch (const std::exception& e) {
            qDebug() << "App::start_status_bar";
            qDebug() << e.what();
        }
    }

    void view(QAction* action) {
        if (action->text() == "Tile") {
            mdi->tileSubWindows();
        } else if (action->text() == "Cascade") {
            mdi->cascadeSubWindows();
        }
    }

    void start(QAction* action) {
        if (action->text() == "Terminal") {
            add_terminal();
        } else if (action->text() == "Web Browser") {
            add_tabbed_browser();
        } else if (action->text() == "Media Player") {
	    addMediaPlayer();
        } else if (action->text() == "Calculator") {
	    addCalculator();
        } else if (action->text() == "File Browser") {
            addFileBrowser();
        } else if (action->text() == "Quit") {
            qApp->quit();
        }
    }

    void window_activate(QAction* action) {
        qDebug() << "App::window_activate";
        qDebug() << action->text();
        action->data().value<QWidget*>()->setFocus();
    }

    void update_window_list(QMdiSubWindow* active_window) {
        qDebug() << "App::update_window_list";
        windows->clear();

        for (auto window : mdi->subWindowList()) {
            qDebug() << "App::update_window_list: " << window->windowTitle();

            QAction* activate_action;
            if (window == active_window) {
                qDebug() << "Active Window: " << window->windowTitle();
                activate_action = new QAction("* " + window->windowTitle(), this);
                // Hack because mdiSubWindow retains focus, prevents keystroke events from getting to VNC
                // Solution: create a new subwindow class and implement keystroke event to emit to widget
                if (window->windowTitle() == "VNC") {
                    window->widget()->setFocus();
                }
            } else {
                qDebug() << "Inactive Window: " << window->windowTitle();
                activate_action = new QAction(window->windowTitle(), this);
            }

            activate_action->setData(QVariant::fromValue(window));
            windows->addAction(activate_action);
        }
    }

    void add_sub_window(QWidget* widget, const QString& title) {
        try {
            auto sub = new QMdiSubWindow;
            sub->setWindowIcon(QIcon("adrenaline.png"));
            sub->setWidget(widget);
            sub->setWindowTitle(title);
            sub->setAttribute(Qt::WA_DeleteOnClose);
            mdi->addSubWindow(sub);
            sub->show();
        } catch (const std::exception& e) {
            qDebug() << "App::add_sub_window";
            qDebug() << e.what();
        }
    }

    void add_terminal() {
        try {
            QTermWidget *console = new QTermWidget();
            QFont font = QApplication::font();
            font.setFamily("Terminus");
            font.setPointSize(6);
            console->setTerminalFont(font);
            console->setColorScheme("Linux");
            connect(console, &QTermWidget::termKeyPressed, console,
                   [=](const QKeyEvent *key) -> void {
                     if (key->matches(QKeySequence::Copy)) {
                       console->copyClipboard();
                     } else if (key->matches(QKeySequence::Paste)) {
                       console->pasteClipboard();
                     }
                   });
            console->setTerminalBackgroundImage("/home/austin/adrenaline.jpg");
            console->setTerminalBackgroundMode(1);	    
            add_sub_window(console, "Terminal");
        } catch (const std::exception& e) {
            qDebug() << "App::add_terminal";
            qDebug() << e.what();
        }
    }

    void add_tabbed_browser() {
        try {
            Browser* webBrowser = new Browser();
            add_sub_window(webBrowser, "Web Browser");
        } catch (const std::exception& e) {
            qDebug() << "App::tabbed_browser";
            qDebug() << e.what();
        }
    }
       void addMediaPlayer() {
        try {
            VideoPlayer* widget = new VideoPlayer();
            add_sub_window(widget, "Video Player");
        } catch (const std::exception& e) {
            qDebug() << "App::addMediaPlayer";
            qDebug() << e.what();
        }
    }
/*
    void addVncClient() {
        try {
            auto widget = new VncClient();
            addSubWindow(widget, "VNC");
        } catch (const std::exception& e) {
            qDebug() << "App::addVncClient";
            qDebug() << e.what();
        }
    }
*/
    void addCalculator() {
        try {
            Calculator* widget = new Calculator();
            add_sub_window(widget, "Calculator");
        } catch (const std::exception& e) {
            qDebug() << "App::addCalculator";
            qDebug() << e.what();
        }
    }

    void addFileBrowser() {
        try {
            FileBrowser* widget = new FileBrowser();
            add_sub_window(widget, "File Browser");
        } catch (const std::exception& e) {
            qDebug() << "App::addFileBrowser";
            qDebug() << e.what();
        }
    }

    QShortcut* shortcut_quit;
    QShortcut* shortcut_next_window;
    QShortcut* shortcut_tile_windows;
    QShortcut* shortcut_restore_window;
    QShortcut* shortcut_maximize_window;

    QPixmap bg_img;
    MDIArea* mdi;
    QThread* status_thread;
    StatusBar* status_worker;

    QMenu* windows;
};

int main(int argc, char** argv) {
    setenv("TERM", "konsole-256color", 1); // 1 means overwrite if already exists
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    QFont font = QApplication::font();
    font.setFamily("Terminus");
    font.setPointSize(6);
    app.setFont(font);
    return app.exec();
}

