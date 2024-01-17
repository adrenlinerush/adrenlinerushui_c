// markdownbrowser.cpp
#include "markdownbrowser.h"
#include <QFile>
#include <QWebEnginePage>
#include <QMenu>

MarkdownBrowser::MarkdownBrowser(const QString& path, QWidget* parent)
    : QWidget(parent) {
    try {
        browser = new QWebEngineView(this);
        browser->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        browser->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);

        layout = new QVBoxLayout(this);
        layout->addWidget(browser);

	markdownPath = path;
	BuildHTML();

        shortcut_refresh = new QShortcut(QKeySequence("F5"), this);
        connect(shortcut_refresh, &QShortcut::activated, this, &MarkdownBrowser::BuildHTML);
        
        browser->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(browser, &QWebEngineView::customContextMenuRequested,
            this, &MarkdownBrowser::showContextMenu);

        setLayout(layout);
    } catch (const std::exception& e) {
        qDebug() << "DocumentBrowser::__init__";
        qDebug() << e.what();
    }
}

void MarkdownBrowser::showContextMenu(const QPoint &point) {
    qDebug() << "Custom Context Menu Requested";
    QMenu* context = new QMenu;
    context->addAction("Reload Markdown");
    connect(context, &QMenu::triggered, this, &MarkdownBrowser::contextExecute);
    context->exec(mapToGlobal(point));
}

void MarkdownBrowser::contextExecute(QAction* action) {
    if (action->text() == "Reload Markdown") {
        BuildHTML();
    } 
}

void MarkdownBrowser::BuildHTML() {
   qDebug() << "Building Markdown HTML";
   QFile header("markdown/markdown_header.html");
   QFile body(markdownPath);
   QFile footer("markdown/markdown_footer.html");

   QString html;

   if (header.open(QIODevice::ReadOnly | QIODevice::Text)) {
	   QTextStream stream(&header);
	   while (!stream.atEnd()) {
		   html.append(stream.readLine()+"\n");
           }
   }
   header.close();
   if (body.open(QIODevice::ReadOnly | QIODevice::Text)) {
	   QTextStream stream(&body);
           while (!stream.atEnd()) {
		   qDebug() << "Reading a line from the markdown file.";
		   html.append(stream.readLine()+"\n");
           }
   }
   body.close();
   if (footer.open(QIODevice::ReadOnly | QIODevice::Text)) {
	   QTextStream stream(&footer);
           while (!stream.atEnd()) {
		   html.append(stream.readLine()+"\n");
           }
   }
   footer.close();
   browser->setHtml(html);
}
