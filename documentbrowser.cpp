// documentbrowser.cpp
#include "documentbrowser.h"

DocumentBrowser::DocumentBrowser(const QString& url, QWidget* parent)
    : QWidget(parent) {
    try {
        browser = new QWebEngineView(this);
        browser->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        browser->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);
        browser->setUrl(QUrl(url));

        layout = new QVBoxLayout(this);
        layout->addWidget(browser);

        setLayout(layout);
    } catch (const std::exception& e) {
        qDebug() << "DocumentBrowser::__init__";
        qDebug() << e.what();
    }
}

