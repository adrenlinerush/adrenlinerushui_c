// filebrowser.cpp
#include "filebrowser.h"
#include "videoplayer.h"
#include "documentbrowser.h"
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
        
        view = new QTabWidget();
        view->setTabsClosable(true);
        connect(view, &QTabWidget::tabCloseRequested, this, &FileBrowser::closeTab);
        connect(view, &QTabWidget::tabBarDoubleClicked, this, &FileBrowser::renameTab);

        dir_display = new QLineEdit();
        dir_display->setReadOnly(true);

        file_group_layout->addWidget(dir_display);
        file_group_layout->addWidget(files);
        splitter->addWidget(file_group);
        splitter->addWidget(view);
	layout->addWidget(splitter);

        qDebug() << dir;
        updateDirListing();

        setLayout(layout);

    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::__init__";
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
        //std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    // Read a portion of the file into a vector
    constexpr size_t bufferSize = 1024;
    std::vector<char> buffer(bufferSize);
    file.read(buffer.data(), bufferSize);

    if (file.gcount() == 0) {
        //std::cerr << "Error reading file: " << filename << std::endl;
        return false;
    }

    // Check for the presence of null characters in the buffer
    auto nullCharacter = std::find(buffer.begin(), buffer.end(), '\0');

    // If null characters are found, the file is likely binary
    return nullCharacter != buffer.end();
}

void FileBrowser::closeTab(int index) {
    try {
        //view->widget(index)->close();
        //view->removeTab(index);
	delete view->widget(index);
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::closeTab";
        qDebug() << e.what();
    }
}

void FileBrowser::updateDirListing() {
    try {
        files->clear();
        //files->addItem("..");
        dir_display->setText(dir);

        QDir directory(dir);
	directory.setFilter(QDir::Files | QDir::Hidden | QDir::AllDirs | QDir::System);
        for (const auto& entry : directory.entryList()) {
            qDebug() << entry;
            files->addItem(entry);
        }
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::updateDirListing";
        qDebug() << e.what();
    }
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
        // Implement the logic to open media player
        // You can replace the following line with the actual implementation
        qDebug() << "Open Media Player: " << filepath;
        VideoPlayer *mediaplayer = new VideoPlayer(filepath, false);
	QString filename = QFileInfo(filepath).fileName();
        view->addTab(mediaplayer, filename);
        view->setCurrentIndex(view->count() - 1);
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::openMediaPlayer";
        qDebug() << e.what();
    }
}

void FileBrowser::openTextFile(const QString& filepath) {
    try {
        // Implement the logic to open text file
        // You can replace the following line with the actual implementation
        qDebug() << "Open Text File: " << filepath;
        QTermWidget *console = new QTermWidget();
        QFont font = QApplication::font();
        font.setFamily("Terminus");
        font.setPointSize(6);
        console->setTerminalFont(font);
        console->setColorScheme("Linux");
        connect(console, &QTermWidget::termKeyPressed, console,
               [=](const QKeyEvent *key) -> void {
                 if (key->matches(QKeySequence::Copy)) {
                   console->copyClipboard();
                 } else if (key->matches(QKeySequence::Paste)) {
                   console->pasteClipboard();
                 }
               });
        console->sendText("vim  " + filepath  + "\n");
	QString filename = QFileInfo(filepath).fileName();
        view->addTab(console, filename);
        view->setCurrentIndex(view->count() - 1);
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::openTextFile";
        qDebug() << e.what();
    }
}

void FileBrowser::openBrowser(const QString& filepath) {
    try {
        // Implement the logic to open browser
        // You can replace the following line with the actual implementation
        qDebug() << "Open Browser: " << filepath;
	QString filename = QFileInfo(filepath).fileName();
        DocumentBrowser *mediabrowser = new DocumentBrowser("file://" + filepath);
        view->addTab(mediabrowser, filename);
        view->setCurrentIndex(view->count() - 1);
    } catch (const std::exception& e) {
        qDebug() << "FileBrowser::openBrowser";
        qDebug() << e.what();
    }
}

