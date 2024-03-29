#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <QMenuBar>
#include <QMenu>
#include <QKeyEvent>
#include "mdiarea.h"
#include "statusbar.h"
#include "filebrowser.h"
#include <qtermwidget5/qtermwidget.h>
#include "browser.h"
#include "videoplayer.h"
#include "calculator.h"
#include "terminal.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void tileSubWindows();
    void setOtherScreen(MainWindow *otherScreen);
    void addFileBrowser();
    void add_terminal();
    void add_tabbed_browser();
    void addMediaPlayer();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void activateOtherScreen();
    void start(QAction* action);
    void window_activate(QAction* action);
    void view(QAction* action);

private:
    void start_status_bar();
    void update_window_list(QMdiSubWindow* active_window);
    void add_sub_window(QWidget* widget, const QString& title);
    void addCalculator();
    void openStartMenu();

    QShortcut* shortcut_quit;
    QShortcut* shortcut_next_window;
    QShortcut* shortcut_next_screen;
    QShortcut* shortcut_tile_windows;
    QShortcut* shortcut_restore_window;
    QShortcut* shortcut_maximize_window;
    QShortcut* shortcut_start_menu;
    QMenuBar* bar;
    MainWindow* oScreen;

    QPixmap bg_img;
    MDIArea* mdi;
    QMenu* startMenu;
    QThread* status_thread;
    StatusBar* status_worker;

    QMenu* windows;
};

#endif // MAINWINDOW_H

