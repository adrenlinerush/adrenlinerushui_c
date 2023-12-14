#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QKeySequence>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QShortcut>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QDebug>

class WebEnginePage : public QWebEnginePage
{
//    Q_OBJECT

public:
    WebEnginePage(QObject* parent = nullptr) : QWebEnginePage(parent) {}

    bool certificateError(const QWebEngineCertificateError& error) override
    {
        try
        {
            qDebug() << error.url();
            QMessageBox ignoreCertificate;
            ignoreCertificate.setText("Invalid Certificate");
            ignoreCertificate.setInformativeText(error.url().toString() +
                                                 " has presented an invalid certificate. Continue to site anyway?");
            ignoreCertificate.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            ignoreCertificate.setDefaultButton(QMessageBox::No);

            int cont = ignoreCertificate.exec();
            if (cont == QMessageBox::Yes)
            {
                qDebug() << "Attempting to ignore certificate error.";
                error.ignoreCertificateError();
                return true;
            }

            return QWebEnginePage::certificateError(error);
        }
        catch (const std::exception& e)
        {
            qDebug() << "WebEnginePage::certificateError";
            qDebug() << e.what();
            return false;
        }
    }

    QWebEnginePage* createWindow(WebWindowType type) override
    {
        try
        {
            qDebug() << type;
            qDebug() << parent();
            // Assuming that parent() is a pointer to the parent class that has the add_tab() function.
            auto newTab = static_cast<YourParentClass*>(parent())->addTab();
            return newTab;
        }
        catch (const std::exception& e)
        {
            qDebug() << "WebEnginePage::createWindow";
            qDebug() << e.what();
            return nullptr;
        }
    }
};

class Browser : public QWidget {
    //Q_OBJECT

public:
    Browser(QWidget* parent = nullptr) : QWidget(parent) {
        try {
            tabs = new QTabWidget(this);
            tabs->setDocumentMode(true);
            connect(tabs, &QTabWidget::tabBarDoubleClicked, this, &Browser::addTab);
            connect(tabs, &QTabWidget::currentChanged, this, &Browser::updateUrlBar);
            tabs->setTabsClosable(true);
            connect(tabs, &QTabWidget::tabCloseRequested, this, &Browser::closeTab);

            layout = new QVBoxLayout(this);

            navbar = new QToolBar("Navigation", this);

            backbtn = new QPushButton(this);
            backbtn->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
            connect(backbtn, &QPushButton::clicked, [this]() { tabs->currentWidget()->back(); });

            fwdbtn = new QPushButton(this);
            fwdbtn->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
            connect(fwdbtn, &QPushButton::clicked, [this]() { tabs->currentWidget()->forward(); });

            reloadbtn = new QPushButton(this);
            reloadbtn->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
            connect(reloadbtn, &QPushButton::clicked, [this]() { tabs->currentWidget()->reload(); });

            shortcut_reload = new QShortcut(QKeySequence("F5"), this);
            connect(shortcut_reload, &QShortcut::activated, [this]() { tabs->currentWidget()->reload(); });

            shortcut_new_tab = new QShortcut(QKeySequence("Ctrl+T"), this);
            connect(shortcut_new_tab, &QShortcut::activated, this, &Browser::addTab);

            shortcut_find_text = new QShortcut(QKeySequence("Ctrl+F"), this);
            connect(shortcut_find_text, &QShortcut::activated, this, &Browser::findText);

            homebtn = new QPushButton(this);
            homebtn->setIcon(QIcon(":/fontawesome-free-5.15.1-desktop/svgs/solid/home.svg"));
            connect(homebtn, &QPushButton::clicked, this, &Browser::goHome);

            stopbtn = new QPushButton(this);
            stopbtn->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
            connect(stopbtn, &QPushButton::clicked, [this]() { tabs->currentWidget()->stop(); });

            backbtn->clicked.connect(lambda: self.tabs.currentWidget().back())
            self.fwdbtn.clicked.connect(lambda: self.tabs.currentWidget().forward())
            self.reloadbtn.clicked.connect(lambda: self.tabs.currentWidget().reload())
            self.shortcut_reload.activated.connect(lambda: self.tabs.currentWidget().reload())
            self.shortcut_new_tab.activated.connect(lambda: self.add_tab())
            self.stopbtn.clicked.connect(lambda: self.tabs.currentWidget().stop())
            self.homebtn.clicked.connect(self.go_home)

            urlbar = new QLineEdit(this);

            shortcut_url = new QShortcut(QKeySequence("Ctrl+G"), this);
            connect(shortcut_url, &QShortcut::activated, this, &Browser::focusUrlBar);

            connect(urlbar, &QLineEdit::returnPressed, this, &Browser::gotoUrl);

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
            favbtn->setIcon(QIcon(":/fontawesome-free-5.15.1-desktop/svgs/solid/star.svg"));
            favbtn->setPopupMode(QToolButton::InstantPopup);

            favmenu = new QMenu(this);
            connect(favmenu, &QMenu::triggered, this, &Browser::gotoFavorite);

            favbtn->setMenu(favmenu);
            navbar->addWidget(favbtn);

            loadFavorites();

            setLayout(layout);

            addTab(QUrl("https://www.google.com"));

            search_text = "";

            shortcut_find_next = new QShortcut(QKeySequence("N"), this);
            connect(shortcut_find_next, &QShortcut::activated, this, &Browser::findNext);

            shortcut_find_prev = new QShortcut(QKeySequence("P"), this);
            connect(shortcut_find_prev, &QShortcut::activated, this, &Browser::findPrev);

            shortcut_find_exit = new QShortcut(QKeySequence("ESC"), this);
            connect(shortcut_find_exit, &QShortcut::activated, this, &Browser::findExit);
        } catch (const std::exception& e) {
            qDebug() << "Browser::Browser Error: " << e.what();
        }
    }

   void Browser::addFavorite() {
    try {
        int tabIndex = tabs->currentIndex();
        QString name = tabs->tabText(tabIndex);
        QUrl url = tabs->currentWidget()->url();
        QString favorite = name + "=" + url.toString() + "\n";

        QFile favFile(QDir::homePath() + "/.favorites");
        if (favFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&favFile);
            stream << favorite;
            favFile.close();
        }

        loadFavorites();
    } catch (const std::exception& e) {
        qDebug() << "Browser::addFavorite Error: " << e.what();
    }
}

void Browser::loadFavorites() {
    try {
        favmenu->clear();
        favmenu->addAction("Add Favorite");

        QFile favFile(QDir::homePath() + "/.favorites");
        if (favFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&favFile);
            while (!stream.atEnd()) {
                QString fav = stream.readLine();
                if (!fav.isEmpty()) {
                    QStringList aryFav = fav.split('=');
                    QAction* favAct = new QAction(aryFav[0], this);
                    favAct->setData(aryFav[1].trimmed());
                    connect(favAct, &QAction::triggered, this, &Browser::gotoFavorite);
                    favmenu->addAction(favAct);
                }
            }
            favFile.close();
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser::loadFavorites Error: " << e.what();
    }
}

void Browser::gotoFavorite(QAction* favorite) {
    qDebug() << "Browser::gotoFavorite ";
    if (favorite->text() == "Add Favorite") {
        addFavorite();
    } else {
        qDebug() << favorite->data();
        tabs->currentWidget()->setUrl(QUrl(favorite->data().toString()));
    }
}

void Browser::handleAuth(const QUrl& url, QAuthenticator* auth) {
    qDebug() << "Browser::handleAuth";
    qDebug() << url;
    qDebug() << auth;
    try {
        QString user;
        bool okUser = QInputDialog::getText(this, url.toString() + " has requested authentication", "User name:", QLineEdit::Normal, "", &okUser, Qt::Dialog);

        if (okUser && !user.isEmpty()) {
            auth->setUser(user);
            qDebug() << "Browser::handleAuth set user";
        }

        QString passwd;
        bool okPasswd = QInputDialog::getText(this, url.toString() + " has requested authentication", "Password:", QLineEdit::Password, "", &okPasswd, Qt::Dialog);

        if (okPasswd && !passwd.isEmpty()) {
            auth->setPassword(passwd);
            qDebug() << "Browser::handleAuth set password";
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser::handleAuth Error: " << e.what();
    }
}

void Browser::findText() {
    try {
        bool ok;
        QString text = QInputDialog::getText(this, "Search", "Find Text:", QLineEdit::Normal, "", &ok, Qt::Dialog);

        if (ok && !text.isEmpty()) {
            qDebug() << "Browser::findText";
            qDebug() << text;
            search_text = text;
            tabs->currentWidget()->page()->findText(search_text, QWebEnginePage::FindFlags(0), this->findTextCallback);
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser::findText Error: " << e.what();
    }
}

void Browser::findTextCallback(bool bFound) {
    qDebug() << bFound;
    if (!bFound && !search_text.isEmpty()) {
        findText();
    }
}

void Browser::findNext() {
    try {
        tabs->currentWidget()->page()->findText(search_text, QWebEnginePage::FindFlags(0), this->findTextCallback);
    } catch (const std::exception& e) {
        qDebug() << "Browser::findNext Error: " << e.what();
    }
}

void Browser::findPrev() {
    try {
        tabs->currentWidget()->page()->findText(search_text, QWebEnginePage::FindFlags(QWebEnginePage::FindBackward), this->findTextCallback);
    } catch (const std::exception& e) {
        qDebug() << "Browser::findPrev Error: " << e.what();
    }
}

void Browser::findExit() {
    try {
        search_text.clear();
        tabs->currentWidget()->page()->findText(search_text, QWebEnginePage::FindFlags(0), this->findTextCallback);
    } catch (const std::exception& e) {
        qDebug() << "Browser::findExit Error: " << e.what();
    }
}

void Browser::closeTab(int i) {
    try {
        if (tabs->count() > 1) {
            tabs->removeTab(i);
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser::closeTab Error: " << e.what();
    }
}

WebEnginePage* Browser::addTab(const QUrl& qurl) {
    try {
        QWebEngineView* browser = new QWebEngineView();
        WebEnginePage* page = new WebEnginePage(this);
        connect(page->profile(), &QWebEngineProfile::downloadRequested, this, &Browser::save);
        connect(page, &QWebEnginePage::authenticationRequired, this, &Browser::handleAuth);
        browser->setPage(page);
        browser->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        browser->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);

        qDebug() << "Browser::addTab: url: " << qurl.toString();
        browser->setUrl(qurl);
        connect(browser, &QWebEngineView::urlChanged, this, &Browser::updateUrl);
        connect(browser, &QWebEngineView::titleChanged, this, &Browser::updateTabLabel);
        int i = tabs->addTab(browser, browser->title());
        tabs->setCurrentIndex(i);
        return page;
    } catch (const std::exception& e) {
        qDebug() << "Browser::addTab Error: " << e.what();
        return nullptr;
    }
}

void Browser::printToPdf() {
    try {
        int tabIndex = tabs->currentIndex();
        QString tabText = tabs->tabText(tabIndex);
        qDebug() << "Browser::printToPdf.tabText: " << tabText;
        QWebEnginePage* page = tabs->currentWidget()->page();
        QString path = QFileDialog::getSaveFileName(this, "Print to PDF", "", "*.pdf");
        qDebug() << "Browser::printToPdf.path: " << path;
        page->printToPdf(path);
    } catch (const std::exception& e) {
        qDebug() << "Browser::printToPdf Error: " << e.what();
    }
}

void Browser::save(QWebEngineDownloadItem* request) {
    try {
        qDebug() << "save requested:";
        qDebug() << request->state();
        qDebug() << request->path();

        if (request->state() == QWebEngineDownloadItem::DownloadRequested) {
            QUrl url = request->path();
            QString suffix = QFileInfo(url.path()).suffix();
            QString path = QFileDialog::getSaveFileName(tabs->currentWidget(), "Save File", url.toString(), "*." + suffix);

            if (!path.isEmpty()) {
                request->setPath(path);
                request->accept();
            } else {
                request->cancel();
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Browser::save Error: " << e.what();
    }
}

void Browser::focusUrlbar() {
    try {
        urlbar->setText("");
        urlbar->setCursorPosition(0);
        urlbar->setFocus();
    } catch (const std::exception& e) {
        qDebug() << "Browser::focusUrlbar Error: " << e.what();
    }
}

void Browser::goHome() {
    tabs->currentWidget()->setUrl(QUrl("http://retro.adrenlinerush.net"));
}

void Browser::gotoUrl() {
    QUrl url(urlbar->text());

    if (url.toString().contains(" ") || (!url.toString().contains(".") && url.scheme().isEmpty())) {
        url = QUrl("https://www.google.com/search?q=" + url.toString().split(" ").join("+"));
        qDebug() << "Browser::gotoUrl search: " << url.toString();
    } else if (url.scheme().isEmpty()) {
        url.setScheme("http");
    }

    tabs->currentWidget()->setUrl(url);
}

void Browser::updateUrl(const QUrl& url) {
    qDebug() << "Browser::updateUrl: url: " << url;
    if (!url.isEmpty()) {
        updateUrlBar(url.toString());
    }
}

void Browser::updateUrlBar(const QString& url) {
    try {
        qDebug() << "Browser::updateUrlBar: url: " << url;
        urlbar->setText(url);
        urlbar->setCursorPosition(0);
    } catch (const std::exception& e) {
        qDebug() << "Browser::updateUrlBar Error: " << e.what();
    }
}

void Browser::updateTabLabel() {
    try {
        qDebug() << "Browser::updateTabLabel: title: " << tabs->currentWidget()->page()->title();
        int i = tabs->currentIndex();
        QString title = tabs->currentWidget()->page()->title();

        if (title.length() > 15) {
            title = title.left(15);
        }

        tabs->setTabText(i, title);
    } catch (const std::exception& e) {
        qDebug() << "Browser::updateTabLabel Error: " << e.what();
    }
}

};
