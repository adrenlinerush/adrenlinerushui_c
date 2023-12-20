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

class FileBrowser : public QWidget {
    Q_OBJECT

public:
    FileBrowser(QWidget* parent = nullptr);

private slots:
    void closeTab(int index);
    void updateDirListing();
    void itemActivated();
    bool isBinaryFile(const std::string& filename);

private:
    QHBoxLayout* layout;
    QGroupBox* file_group;
    QVBoxLayout* file_group_layout;
    QListWidget* files;
    QTabWidget* view;
    QLineEdit* dir_display;
    QString dir;
    QSplitter *splitter;

    void openMediaPlayer(const QString& filepath);
    void openTextFile(const QString& filepath);
    void openBrowser(const QString& filepath);
};

#endif // FILEBROWSER_H

