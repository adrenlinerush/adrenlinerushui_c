#include "mainwindow.h"
#include "mdisubwindow.h"
#include <unistd.h>
#include <QApplication>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), oScreen(nullptr) {
    try {
        //setupFileLogging("~/ui.log");
        setWindowIcon(QIcon("/usr/share/adrenlinerushui/adrenaline.png"));

        shortcut_quit = new QShortcut(QKeySequence("Ctrl+Q"), this);
	connect(shortcut_quit, &QShortcut::activated, qApp, &QApplication::quit);

        bg_img = QPixmap("/usr/share/adrenlinerushui/adrenaline.jpg");
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

        bar = menuBar();

        startMenu = bar->addMenu("Start");
        startMenu->addAction("Quit");
        startMenu->addAction("Terminal");
        startMenu->addAction("Web Browser");
        startMenu->addAction("Media Player");
        startMenu->addAction("Calculator");
        startMenu->addAction("File Browser");
        connect(startMenu, &QMenu::triggered, this, &MainWindow::start);

	shortcut_start_menu = new QShortcut(QKeySequence("Alt+S"), this);
        connect(shortcut_start_menu, &QShortcut::activated, this, &MainWindow::openStartMenu);

        QMenu* view = bar->addMenu("View");
        view->addAction("Tile");
        view->addAction("Cascade");
        connect(view, &QMenu::triggered, this, &MainWindow::view);

        windows = bar->addMenu("Window");
        connect(windows, &QMenu::triggered, this, &MainWindow::window_activate);

        start_status_bar();
    } catch (const std::exception& e) {
        qDebug() << "MainWindow::__init__";
        qDebug() << e.what();
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
        status_thread->requestInterruption();
	status_thread->wait();
        QMainWindow::closeEvent(event);	
}

void MainWindow::openStartMenu() {
    startMenu->exec(bar->parentWidget()->mapToGlobal(bar->rect().bottomLeft()));
}

void MainWindow::tileSubWindows() {
    mdi->tileSubWindows();
}

void MainWindow::setOtherScreen(MainWindow *otherScreen) {
    oScreen = otherScreen;
    shortcut_next_screen = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(shortcut_next_screen, &QShortcut::activated, this, &MainWindow::activateOtherScreen);
}

void MainWindow::addFileBrowser() {
    try {
        FileBrowser* widget = new FileBrowser();
        add_sub_window(widget, "File Browser");
    } catch (const std::exception& e) {
        qDebug() << "MainWindow::addFileBrowser";
        qDebug() << e.what();
    }
}

void MainWindow::add_terminal() {
    try {
            Terminal *console = new Terminal();
            console->setBackground("/usr/share/adrenlinerushui/adrenaline.jpg");
            add_sub_window(console, "Terminal");
	    connect(console, &Terminal::finished, [console](){if(console->parent()) {
                QMetaObject::invokeMethod(console->parent(), "close", Qt::QueuedConnection);}});
    } catch (const std::exception& e) {
        qDebug() << "MainWindow::add_terminal";
        qDebug() << e.what();
    }
}

void MainWindow::add_tabbed_browser() {
    try {
        Browser* webBrowser = new Browser();
        add_sub_window(webBrowser, "Web Browser");
    } catch (const std::exception& e) {
        qDebug() << "MainWindow::add_tabbed_browser";
        qDebug() << e.what();
    }
}

void MainWindow::addMediaPlayer() {
    try {
        VideoPlayer* widget = new VideoPlayer();
        add_sub_window(widget, "Video Player");
    } catch (const std::exception& e) {
        qDebug() << "MainWindow::addMediaPlayer";
        qDebug() << e.what();
    }
}

void MainWindow::activateOtherScreen() {
    oScreen->activateWindow();
}

void MainWindow::start(QAction* action) {
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

void MainWindow::window_activate(QAction* action) {
    qDebug() << "MainWindow::window_activate";
    qDebug() << action->text();
    action->data().value<QWidget*>()->setFocus();
}

void MainWindow::update_window_list(QMdiSubWindow* active_window) {
    qDebug() << "MainWindow::update_window_list";
    windows->clear();

    for (auto window : mdi->subWindowList()) {
        qDebug() << "MainWindow::update_window_list: " << window->windowTitle();

        QAction* activate_action;
        if (window == active_window) {
            qDebug() << "Active Window: " << window->windowTitle();
            activate_action = new QAction("* " + window->windowTitle(), this);
        } else {
            qDebug() << "Inactive Window: " << window->windowTitle();
            activate_action = new QAction(window->windowTitle(), this);
        }

        activate_action->setData(QVariant::fromValue(window));
        windows->addAction(activate_action);
    }
}

void MainWindow::start_status_bar() {
    try {
        qDebug() << "MainWindow::start_status_bar";
        status_thread = new QThread;
        status_worker = new StatusBar(statusBar());
        status_worker->moveToThread(status_thread);
        connect(status_thread, &QThread::started, status_worker, &StatusBar::updateStatusBar);
        qDebug() << "About to start status thread";
        status_thread->start();
    } catch (const std::exception& e) {
        qDebug() << "MainWindow::start_status_bar";
        qDebug() << e.what();
    }
}

void MainWindow::view(QAction* action) {
    if (action->text() == "Tile") {
        mdi->tileSubWindows();
    } else if (action->text() == "Cascade") {
        mdi->cascadeSubWindows();
    }
}

void MainWindow::add_sub_window(QWidget* widget, const QString& title) {
    try {
        auto sub = new MdiSubWindow;
        sub->setWindowIcon(QIcon("adrenaline.png"));
        sub->setWidget(widget);
        sub->setWindowTitle(title);
        sub->setAttribute(Qt::WA_DeleteOnClose);
	widget->setParent(sub);
        mdi->addSubWindow(sub);
        sub->show();
    } catch (const std::exception& e) {
        qDebug() << "MainWindow::add_sub_window";
        qDebug() << e.what();
    }
}

void MainWindow::addCalculator() {
    try {
        Calculator* widget = new Calculator();
        add_sub_window(widget, "Calculator");
    } catch (const std::exception& e) {
        qDebug() << "MainWindow::addCalculator";
        qDebug() << e.what();
    }
}

