// filebrowser.cpp
#include "filebrowser.h"
#include "videoplayer.h"
#include "documentbrowser.h"
#include "markdownbrowser.h"
#include "terminal.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <QDir>
#include <QFont>
#include <QString>
#include <QVector>
#include <algorithm>
#include <qtermwidget5/qtermwidget.h>
#include <QApplication>
#include <QInputDialog>
#include <QList>

FileBrowser::FileBrowser(QWidget* parent)
    : QWidget(parent), dir(QDir::homePath()) {
    try {
        layout = new QHBoxLayout(this);
	splitter = new QSplitter(Qt::Horizontal);
        file_group = new QGroupBox();
	file_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        file_group_layout = new QVBoxLayout(file_group);
        file_group->setLayout(file_group_layout);

        files = new QListWidget();
        connect(files, &QListWidget::itemActivated, this, &FileBrowser::itemActivated);
        connect(files, &QListWidget::itemClicked, this, &FileBrowser::itemClicked);

	files->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(files, &QListWidget::customContextMenuRequested, 
			        this, &FileBrowser::showOpenWithMenu);

        
        view = new QTabWidget();
        view->setTabsClosable(true);
        connect(view, &QTabWidget::tabCloseRequested, this, &FileBrowser::closeTab);
        connect(view, &QTabWidget::tabBarDoubleClicked, this, &FileBrowser::renameTab);
	connect(view, &QTabWidget::currentChanged, this, &FileBrowser::tabChanged);

        dir_display = new QLineEdit();
        dir_display->setReadOnly(true);

        file_group_layout->addWidget(dir_display);
        file_group_layout->addWidget(files);
        splitter->addWidget(file_group);
        splitter->addWidget(view);
	splitter->setStretchFactor(1,4);
	layout->addWidget(splitter);

        qDebug() << dir;
        updateDirListing();

        setLayout(layout);

    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::__init__";
        qDebug() << e.what();
    }
}

void FileBrowser::tabChanged(int index) {
    topLevelWidget()->activateWindow();
    view->currentWidget()->setFocus();   
}

void FileBrowser::showOpenWithMenu(const QPoint &point) {
    qDebug() << "Custom Context Menu Requested";
    QMenu* openWith = new QMenu;
    openWith->addAction("Vim");
    openWith->addAction("Webengine");
    openWith->addAction("Markdown");
    openWith->addAction("Media Player");
    openWith->addAction("Bash (Execute)");
    openWith->addAction("Bash (New Shell Here)");
    connect(openWith, &QMenu::triggered, this, &FileBrowser::openWithExecute);
    openWith->exec(mapToGlobal(point));
}

void FileBrowser::openWithExecute(QAction* action) {
    try {
	    QString itemText = files->currentItem()->text();
	    QString path = QDir::cleanPath(dir + "/" + itemText);
	    if (action->text() == "Vim") {
		openTextFile(path);
	    } else if (action->text() == "Markdown") {
		markdownViewer(path);
	    } else if (action->text() == "Webengine") {
		openBrowser(path);
	    } else if (action->text() == "Media Player") {
		openMediaPlayer(path);
	    } else if (action->text() == "Bash (Execute)") {
		openTerminal(path, "Terminal");
	    } else if (action->text() == "Bash (New Shell Here)") {
		openTerminal("cd " + dir, "Terminal");
	    }
     } catch (const std::exception& e) {
            qDebug() << "Error in context menu.";
	    qDebug() << e.what();
     }
}

void FileBrowser::renameTab(int i) {
    QString newLabel = QInputDialog::getText(this, "", "What would you like to label this tab as:");
    if (newLabel.isEmpty())
        return;
    view->setTabText(i, newLabel);
}

bool FileBrowser::isBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }

    constexpr size_t bufferSize = 1024;
    std::vector<char> buffer(bufferSize);
    file.read(buffer.data(), bufferSize);

    if (file.gcount() == 0) {
        return false;
    }

    auto nullCharacter = std::find(buffer.begin(), buffer.end(), '\0');

    return nullCharacter != buffer.end();
}

void FileBrowser::closeTab(int index) {
    try {
	delete view->widget(index);
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::closeTab";
        qDebug() << e.what();
    }
}

void FileBrowser::updateDirListing() {
    try {
        files->clear();
        dir_display->setText(dir);

        QDir directory(dir);
	directory.setFilter(QDir::Files | QDir::Hidden | QDir::AllDirs | QDir::System);
        for (const auto& entry : directory.entryList()) {
            qDebug() << entry;
            files->addItem(entry);
	    if (!QFileInfo(dir + "/" + entry).isFile()) {
		   files->item(files->count()-1)->setForeground(Qt::blue);
	    }
        }
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::updateDirListing";
        qDebug() << e.what();
    }
}

void FileBrowser::itemClicked() {
    topLevelWidget()->activateWindow();
    files->setFocus();
}

void FileBrowser::itemActivated() {
    try {
        qDebug() << dir;
        QString itemText = files->currentItem()->text();
        qDebug() << itemText;
        if (itemText == "..") {
            if (dir.count('/') == 1) {
                dir = '/';
            } else if (dir != "/") {
                dir = dir.section('/', 0, -2);
            }
            updateDirListing();
        } else {
            QString path = QDir::cleanPath(dir + "/" + itemText);
            if (QFileInfo(path).isDir()) {
                dir = path;
                updateDirListing();
            } else {
                if (QFileInfo(path).isFile()) {
		    if (isBinaryFile(path.toStdString())) {
		       QString suffix = QFileInfo(itemText).suffix();

		       QList<QString> mediaList;
		       mediaList.append("mp4");
		       mediaList.append("mp3");
		       mediaList.append("wav");
		       mediaList.append("avi");
		       mediaList.append("mkv");


		       QList<QString> browserList;
		       browserList.append("pdf");
		       browserList.append("jpg");
		       browserList.append("gif");
		       browserList.append("bmp");
		       browserList.append("png");

                       if (mediaList.contains(suffix)) { 
		               openMediaPlayer(path);
			       return;
     		       }
		       if (browserList.contains(suffix)) {
                               openBrowser(path);
			       return;
		       }

                           QMessageBox::StandardButton reply = QMessageBox::question(
				this,
				"", 
				"Unknown File Type.\nWould you like to open with vim?",
				QMessageBox::Yes | QMessageBox::No
			    );
                           if (reply == QMessageBox::Yes) {
			        openTextFile(path);
                           }
			   return;
		    } else {
                       openTextFile(path);
		    }
                }
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::itemActivated";
        qDebug() << e.what();
    }
}

void FileBrowser::openMediaPlayer(const QString& filepath) {
    try {
        qDebug() << "Open Media Player: " << filepath;
        VideoPlayer *mediaplayer = new VideoPlayer(filepath, false);
	QString filename = QFileInfo(filepath).fileName();
        view->addTab(mediaplayer, filename);
        view->setCurrentIndex(view->count() - 1);
	view->currentWidget()->setFocus();
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::openMediaPlayer";
        qDebug() << e.what();
    }
}

void FileBrowser::openTextFile(const QString& filepath) {
     QString filename = QFileInfo(filepath).fileName();
     openTerminal("vim " + filepath, filename);
}

void FileBrowser::openTerminal(const QString& cmd, const QString& tabName) {
    try {
        qDebug() << "Open Terminal Command: " << cmd;
        Terminal *console = new Terminal();
        console->runCommand(cmd);
        view->addTab(console, tabName);
        view->setCurrentIndex(view->count() - 1);
	view->currentWidget()->setFocus();
	connect(console, &Terminal::finished, [this, console](){
            int tabIndex = view->indexOf(console);
            if (tabIndex != -1) {
                view->removeTab(tabIndex);
            }});
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::openTextFile";
        qDebug() << e.what();
    }
}

void FileBrowser::openBrowser(const QString& filepath) {
    try {
        qDebug() << "Open Browser: " << filepath;
	QString filename = QFileInfo(filepath).fileName();
        DocumentBrowser *mediabrowser = new DocumentBrowser("file://" + filepath);
        view->addTab(mediabrowser, filename);
        view->setCurrentIndex(view->count() - 1);
	view->currentWidget()->setFocus();
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::openBrowser";
        qDebug() << e.what();
    }
}

void FileBrowser::markdownViewer(const QString& filepath) {
    try {
        qDebug() << "Open Markdown Browser: " << filepath;
        QString filename = QFileInfo(filepath).fileName();
        MarkdownBrowser *markdownbrowser = new MarkdownBrowser(filepath);
        view->addTab(markdownbrowser, filename);
        view->setCurrentIndex(view->count() - 1);
        view->currentWidget()->setFocus();
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::markdownViewer";
        qDebug() << e.what();
    }
}

