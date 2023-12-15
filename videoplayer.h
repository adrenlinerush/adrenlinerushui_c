// videoplayer.h

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QShortcut>
#include <QKeySequence>

class VideoPlayer : public QWidget {
//    Q_OBJECT

public:
    VideoPlayer(const QString& filename = nullptr, bool can_open = true, QWidget* parent = nullptr);

private slots:
    void openFile(const QString& fileName = nullptr);
    void selectFile();
    void play();
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void handleError();

private:
    QMediaPlayer* mediaPlayer;
    QVideoWidget* videoWidget;
    QPushButton* playBtn;
    QSlider* positionSlider;
    QHBoxLayout* controlLayout;
    QVBoxLayout* layout;

    QShortcut* shortcut_open;

    void initUI();
};

#endif // VIDEOPLAYER_H

