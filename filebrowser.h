// filebrowser.h
#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QDebug>
#include <QSplitter>
#include <QMessageBox>
#include <QMenu>

class FileBrowser : public QWidget {
    Q_OBJECT

public:
    FileBrowser(QWidget* parent = nullptr);

private slots:
    void closeTab(int index);
    void updateDirListing();
    void itemActivated();
    void itemClicked();
    bool isBinaryFile(const std::string& filename);
    void renameTab(int i);
    void showOpenWithMenu(const QPoint &point);
    void tabChanged(int index);

private:
    QHBoxLayout* layout;
    QGroupBox* file_group;
    QVBoxLayout* file_group_layout;
    QListWidget* files;
    QTabWidget* view;
    QLineEdit* dir_display;
    QString dir;
    QSplitter* splitter;
    //QMenu* openWith;

    void openMediaPlayer(const QString& filepath);
    void openLibreOffice(const QString& filepath);
    void openTextFile(const QString& filepath);
    void openBrowser(const QString& filepath);
    void openTerminal(const QString& cmd, const QString& tabName);
    void openWithExecute(QAction* action);
    void markdownViewer(const QString& filepath);
    void createDirectory();
    void createFile();
    void deletePath(const QString& path);
};

#endif 
// FILEBROWSER_H

