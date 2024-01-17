// markdownbrowser.cpp
#include "markdownbrowser.h"
#include <QFile>

MarkdownBrowser::MarkdownBrowser(const QString& path, QWidget* parent)
    : QWidget(parent) {
    try {
        browser = new QWebEngineView(this);
        browser->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        browser->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);

        layout = new QVBoxLayout(this);
        layout->addWidget(browser);

	BuildHTML(path);

        setLayout(layout);
    } catch (const std::exception& e) {
        qDebug() << "DocumentBrowser::__init__";
        qDebug() << e.what();
    }
}

void MarkdownBrowser::BuildHTML(const QString& path) {
   qDebug() << "Building Markdown HTML";
   QFile header("markdown/markdown_header.html");
   QFile body(path);
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
