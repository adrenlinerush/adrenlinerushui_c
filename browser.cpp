#include "browser.h"
#include "lineedit.h"
#include "webenginepage.h"
#include "webengineview.h"
#include <QInputDialog>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QStyle>

Browser::Browser(QWidget* parent) : QWidget(parent) {
    try {
        tabs = new QTabWidget(this);
        tabs->setDocumentMode(true);
        connect(tabs, &QTabWidget::tabBarDoubleClicked, this, static_cast<void (Browser::*)()>(&Browser::add_tab));
        connect(tabs, &QTabWidget::currentChanged, this, &Browser::tab_change);
        tabs->setTabsClosable(true);
        connect(tabs, &QTabWidget::tabCloseRequested, this, &Browser::close_tab);
        layout = new QVBoxLayout(this);
        navbar = new QToolBar("Navigation", this);
        backbtn = new QPushButton(this);
        backbtn->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
        fwdbtn = new QPushButton(this);
        fwdbtn->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
        reloadbtn = new QPushButton(this);
        reloadbtn->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
        shortcut_reload = new QShortcut(QKeySequence("F5"), this);
        shortcut_new_tab = new QShortcut(QKeySequence("Ctrl+T"), this);
        shortcut_find_text = new QShortcut(QKeySequence("Ctrl+F"), this);
        connect(shortcut_find_text, &QShortcut::activated, this, &Browser::find_text);
        homebtn = new QPushButton(this);
        homebtn->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
        stopbtn = new QPushButton(this);
        stopbtn->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
        connect(backbtn, &QPushButton::clicked, this, [this]() {
	    QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
            cw->back();
        });
        connect(fwdbtn, &QPushButton::clicked, this, [this]() {
	    QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
	    cw->forward();
        });
       connect(reloadbtn, &QPushButton::clicked, this, [this]() {
	    QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
	    cw->reload();
        });
        connect(shortcut_reload, &QShortcut::activated, this, [this]() {
	    QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
	    cw->reload();
        });
        connect(shortcut_new_tab, &QShortcut::activated, this, static_cast<void (Browser::*)()>(&Browser::add_tab));
        connect(stopbtn, &QPushButton::clicked, this, [this]() {
	    QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
	    cw->stop();
        });
        connect(homebtn, &QPushButton::clicked, this, &Browser::go_home);
        urlbar = new LineEdit(this);
        shortcut_url = new QShortcut(QKeySequence("Ctrl+G"), this);
        connect(shortcut_url, &QShortcut::activated, this, &Browser::focus_urlbar);
        connect(urlbar, &QLineEdit::returnPressed, this, &Browser::goto_url);
        shortcut_print_to_pdf = new QShortcut(QKeySequence("Ctrl+P"), this);
        connect(shortcut_print_to_pdf, &QShortcut::activated, this, &Browser::printToPdf);
        navbar->addWidget(backbtn);
        navbar->addWidget(fwdbtn);
        navbar->addWidget(homebtn);
        navbar->addWidget(urlbar);
        navbar->addWidget(reloadbtn);
        navbar->addWidget(stopbtn);
        layout->addWidget(navbar);
        layout->addWidget(tabs);
        favbtn = new QToolButton(this);
        favbtn->setIcon(style()->standardIcon(QStyle::SP_DialogYesButton));
        favbtn->setPopupMode(QToolButton::InstantPopup);
        favmenu = new QMenu(this);
        connect(favmenu, &QMenu::triggered, this, &Browser::goto_favorite);
        favbtn->setMenu(favmenu);
        navbar->addWidget(favbtn);
        load_favorites();
        setLayout(layout);
        add_tab();
        search_text = "";
        shortcut_find_next = new QShortcut(QKeySequence("N"), this);
        shortcut_find_prev = new QShortcut(QKeySequence("P"), this);
        shortcut_find_exit = new QShortcut(QKeySequence("ESC"), this);
        connect(shortcut_find_next, &QShortcut::activated, this, &Browser::find_next);
        connect(shortcut_find_prev, &QShortcut::activated, this, &Browser::find_prev);
        connect(shortcut_find_exit, &QShortcut::activated, this, &Browser::find_exit);
    } catch (const std::exception& e) {
        qDebug() << "Browser.__init__: " << e.what();
    }
}

void Browser::add_favorite() {
    try {
        int tab_index = tabs->currentIndex();
        QString name = tabs->tabText(tab_index);
	QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        QUrl url = cw->url();
        QString favorite = name + "=" + url.toString() + "\n";
        QFile fav_file(QDir::homePath() + "/.favorites");
        if (fav_file.open(QIODevice::Append)) {
            QTextStream out(&fav_file);
            out << favorite;
            fav_file.close();
            load_favorites();
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser.add_favorite: " << e.what();
    }
}

void Browser::load_favorites() {
    try {
        favmenu->clear();
        QAction* add_favorite_action = favmenu->addAction("Add Favorite");
        connect(add_favorite_action, &QAction::triggered, this, &Browser::add_favorite);

        QFile fav_file(QDir::homePath() + "/.favorites");
        if (fav_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&fav_file);
            while (!in.atEnd()) {
                QString fav = in.readLine();
                if (!fav.isEmpty()) {
                    QStringList aryfav = fav.split('=');
                    QAction* fav_act = new QAction(aryfav[0], this);
                    fav_act->setData(aryfav[1].trimmed());
                    favmenu->addAction(fav_act);
                }
            }
            fav_file.close();
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser.load_favorites: " << e.what();
    }
}

void Browser::goto_favorite(QAction* favorite) {
    qDebug() << "Browser.goto_favorite: ";
    if (favorite->text() == "Add Favorite") {
        add_favorite();
    } else {
        qDebug() << favorite->data();
	QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        cw->setUrl(QUrl(favorite->data().toString()));
    }
}

void Browser::handle_auth(const QUrl& url, QAuthenticator* auth) {
    qDebug() << "Browser.handle_auth";
    qDebug() << url;
    qDebug() << auth;

    try {
        QString user = QInputDialog::getText(this, url.toString() + " has requested authentication", "User name:");
        if (!user.isEmpty()) {
            auth->setUser(user);
            qDebug() << "Browser.handle_auth set user";
        }

        QString passwd = QInputDialog::getText(this, url.toString() + " has requested authentication", "Password:", QLineEdit::Password);
        if (!passwd.isEmpty()) {
            auth->setPassword(passwd);
            qDebug() << "Browser.handle_auth set password";
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser.handle_auth: " << e.what();
    }
}

void Browser::find_text() {
    try {
        QString text = QInputDialog::getText(this, "Search", "Find Text:");
        if (!text.isEmpty()) {
            qDebug() << "Browser.find_text";
            qDebug() << text;
            search_text = text;
	    QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
	    cw->page()->findText(search_text, QWebEnginePage::FindFlags(0), [this](bool found) { find_text_callback(found); });
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser.find_text: " << e.what();
    }
}

void Browser::find_text_callback(bool b_found) {
    qDebug() << b_found;
    if (!b_found && !search_text.isEmpty()) {
        find_text();
    }
}

void Browser::find_next() {
    try {
	QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
	cw->page()->findText(search_text, QWebEnginePage::FindFlags(0), [this](bool found) { find_text_callback(found); });
    } catch (const std::exception& e) {
        qDebug() << "Browser.find_next: " << e.what();
    }
}

void Browser::find_prev() {
    try {
	QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
	cw->page()->findText(search_text, QWebEnginePage::FindFlags(QWebEnginePage::FindBackward), [this](bool found) { find_text_callback(found); });
    } catch (const std::exception& e) {
        qDebug() << "Browser.find_prev: " << e.what();
    }
}

void Browser::find_exit() {
    try {
        search_text = "";
	QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
	cw->page()->findText(search_text, QWebEnginePage::FindFlags(0), [this](bool found) { find_text_callback(found); });
    } catch (const std::exception& e) {
        qDebug() << "Browser.find_exit: " << e.what();
    }
}

void Browser::close_tab(int i) {
    try {
        if (tabs->count() > 1) {
	    delete tabs->widget(i);
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser.close_tab: " << e.what();
    }
}

void Browser::add_tab() {
  WebEnginePage* page = add_tab(QUrl("https://google.com"));
}

WebEnginePage* Browser::add_tab(const QUrl& qurl) {
    try {
        WebEngineView* webView = new WebEngineView(tabs);
        WebEnginePage* page = new WebEnginePage(webView);
	page->setBrowser(this);
	connect(page->profile(), &QWebEngineProfile::downloadRequested, this, &Browser::save);
        connect(page, &WebEnginePage::authenticationRequired, this, &Browser::handle_auth);
        webView->setPage(page);
        webView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        webView->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);
        qDebug() << "Browser.add_tab: url: " << qurl;
        webView->setUrl(qurl);
        connect(webView, &QWebEngineView::urlChanged, this, &Browser::update_url);
        connect(webView, &QWebEngineView::titleChanged, this, &Browser::update_tab_label);
        int i = tabs->addTab(webView, webView->title());
        tabs->setCurrentIndex(i);
	return page;
    } catch (const std::exception& e) {
        qDebug() << "Browser.add_tab: " << e.what();
    }
}

void Browser::printToPdf() {
    try {
        int tab_index = tabs->currentIndex();
        QString tab_text = tabs->tabText(tab_index);
        qDebug() << "Browser.printToPdf.tab_text: " << tab_text;
	QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        QWebEnginePage* page = cw->page();
        QString path = QFileDialog::getSaveFileName(this, "Print to PDF", "", "*.pdf");
        qDebug() << "Browser.printToPdf.path: " << path;
        page->printToPdf(path);
    } catch (const std::exception& e) {
        qDebug() << "Browser.printToPdf: " << e.what();
    }
}
void Browser::save(QWebEngineDownloadItem *request) {
    try {
        qDebug() << "save requested:";
        qDebug() << request->state();
        qDebug() << request->path();
        
        if (request->state() == QWebEngineDownloadItem::DownloadRequested) {
            QString fileName = request->downloadFileName();
            QString suffix = QFileInfo(fileName).completeSuffix();
            QString path = QFileDialog::getSaveFileName(this->tabs->currentWidget(), "Save File", fileName, "*." + suffix);
            
            if (!path.isEmpty()) {
                request->setPath(path);
                request->accept();
            } else {
                request->cancel();
            }
        }
    } catch (const std::exception &e) {
        qDebug() << "Browser.save: " << e.what();
    }
}

void Browser::focus_urlbar() {
    try {
        urlbar->setText("");
        urlbar->setCursorPosition(0);
        urlbar->setFocus();
    } catch (const std::exception& e) {
        qDebug() << "Browser.focus_urlbar: " << e.what();
    }
}

void Browser::go_home() {
    QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
    cw->setUrl(QUrl("http://retro.adrenlinerush.net"));
}

void Browser::goto_url() {
    QUrl url(urlbar->text());

    if (url.toString().contains(" ") || (!url.toString().contains(".") && url.scheme().isEmpty())) {
        url = QUrl("https://www.google.com/search?q=" + url.toString().split(" ").join("+"));
        qDebug() << "Browser::gotoUrl search: " << url.toString();
    } else if (url.scheme().isEmpty()) {
        url.setScheme("http");
    }

    QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
    cw->setUrl(url);
}

void Browser::update_url(const QUrl& url) {
    qDebug() << "Browser::updateUrl: url: " << url;
    if (!url.isEmpty()) {
        update_url_bar(url.toString());
    }
}

void Browser::update_url_bar(const QString& url) {
    try {
        qDebug() << "Browser::updateUrlBar: url: " << url;
        urlbar->setText(url);
        urlbar->setCursorPosition(0);
    } catch (const std::exception& e) {
        qDebug() << "Browser::updateUrlBar Error: " << e.what();
    }
}

void Browser::tab_change(int i) {
    try { 
        QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
	update_url_bar(cw->page()->url().toString());
    } catch (const std::exception& e) {
	qDebug() << "Browser::tab_change Error: " << e.what();
    }
}

void Browser::update_tab_label() {
    try {
        QWebEngineView* cw = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        qDebug() << "Browser::updateTabLabel: title: " << cw->page()->title();
        int i = tabs->currentIndex();
        QString title = cw->page()->title();

        if (title.length() > 15) {
            title = title.left(15);
        }

        tabs->setTabText(i, title);
    } catch (const std::exception& e) {
        qDebug() << "Browser::updateTabLabel Error: " << e.what();
    }
}
