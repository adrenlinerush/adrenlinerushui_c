#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QPushButton>
#include <QLineEdit>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QShortcut>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QUrl>

class WebEnginePage;

class Browser : public QWidget {

public:
    Browser(QWidget* parent = nullptr);

private slots:
    void add_favorite();
    void load_favorites();
    void goto_favorite(QAction* favorite);
    void handle_auth(const QUrl& url, QAuthenticator* auth);
    void find_text();
    void find_text_callback(bool b_found);
    void find_next();
    void find_prev();
    void find_exit();
    void close_tab(int i);
    void printToPdf();
    void save(QWebEngineDownloadItem*);
    void focus_urlbar();
    void go_home();
    void goto_url();
    void update_url(const QUrl& url);
    void update_url_bar(const QString& url);
    void update_tab_label();
    void add_tab();
    void tab_change(int i);

public slots:
    //WebEnginePage* add_tab();
    WebEnginePage* add_tab(const QUrl& qurl);

private:
    QTabWidget* tabs;
    QVBoxLayout* layout;
    QToolBar* navbar;
    QPushButton* backbtn;
    QPushButton* fwdbtn;
    QPushButton* reloadbtn;
    QShortcut* shortcut_reload;
    QShortcut* shortcut_new_tab;
    QShortcut* shortcut_find_text;
    QPushButton* homebtn;
    QPushButton* stopbtn;
    QLineEdit* urlbar;
    QShortcut* shortcut_url;
    QShortcut* shortcut_print_to_pdf;
    QToolButton* favbtn;
    QMenu* favmenu;
    QString search_text;
    QShortcut* shortcut_find_next;
    QShortcut* shortcut_find_prev;
    QShortcut* shortcut_find_exit;

};

#endif // BROWSER_H

