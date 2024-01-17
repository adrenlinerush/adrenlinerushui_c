// documentbrowser.h
#ifndef MARKDOWNBROWSER_H
#define MARKDOWNBROWSER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QUrl>

class MarkdownBrowser : public QWidget {

public:
    MarkdownBrowser(const QString& path, QWidget* parent = nullptr);

private:
    QWebEngineView* browser;
    QVBoxLayout* layout;

    void BuildHTML(const QString& path);
};

#endif // MARKDOWNBROWSER_H

