// documentbrowser.h
#ifndef DOCUMENTBROWSER_H
#define DOCUMENTBROWSER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QUrl>

class DocumentBrowser : public QWidget {

public:
    DocumentBrowser(const QString& url, QWidget* parent = nullptr);

private:
    QWebEngineView* browser;
    QVBoxLayout* layout;
};

#endif // DOCUMENTBROWSER_H

