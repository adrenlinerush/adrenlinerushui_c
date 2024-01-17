// documentbrowser.h
#ifndef MARKDOWNBROWSER_H
#define MARKDOWNBROWSER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QUrl>
#include <QShortcut>

class MarkdownBrowser : public QWidget {

public:
    MarkdownBrowser(const QString& path, QWidget* parent = nullptr);

private:
    QWebEngineView* browser;
    QVBoxLayout* layout;
    QString markdownPath;
    QShortcut* shortcut_refresh;

    void BuildHTML();
    void showContextMenu(const QPoint &point);
    void contextExecute(QAction* action);
};

#endif // MARKDOWNBROWSER_H

