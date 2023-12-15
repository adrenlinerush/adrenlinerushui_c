// videoplayer.cpp

#include "videoplayer.h"
#include <QStyle>

VideoPlayer::VideoPlayer(const QString& filename, bool can_open, QWidget* parent)
    : QWidget(parent)
{
    mediaPlayer = new QMediaPlayer;
    videoWidget = new QVideoWidget;
    mediaPlayer->setVideoOutput(videoWidget);

    playBtn = new QPushButton;
    playBtn->setEnabled(false);
    playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playBtn, &QPushButton::clicked, this, &VideoPlayer::play);

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);
    connect(positionSlider, &QSlider::sliderMoved, this, &VideoPlayer::setPosition);

    if (can_open) {
        shortcut_open = new QShortcut(QKeySequence("Ctrl+O"), this);
        connect(shortcut_open, &QShortcut::activated, this, &VideoPlayer::selectFile);
    }

    if (!filename.isEmpty()) {
        openFile(filename);
    }

    initUI();
}

void VideoPlayer::selectFile()
{

    QString file = QFileDialog::getOpenFileName(this, "Open Movie", QDir::homePath());
    VideoPlayer::openFile(file);
}

void VideoPlayer::openFile(const QString& fileName)
{
    mediaPlayer->setMedia(QUrl::fromLocalFile(fileName));
    playBtn->setEnabled(true);
}

void VideoPlayer::play()
{
    if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
    } else {
        mediaPlayer->play();
    }
}

void VideoPlayer::mediaStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
}

void VideoPlayer::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
}

void VideoPlayer::setPosition(int position)
{
    mediaPlayer->setPosition(position);
}

void VideoPlayer::handleError()
{
    playBtn->setEnabled(false);
    qDebug() << "VideoPlayer.handleError: " << mediaPlayer->errorString();
}

void VideoPlayer::initUI()
{
    controlLayout = new QHBoxLayout;
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(playBtn);
    controlLayout->addWidget(positionSlider);

    layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    layout->addLayout(controlLayout);

    setLayout(layout);

    connect(mediaPlayer, &QMediaPlayer::stateChanged, this, &VideoPlayer::mediaStateChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &VideoPlayer::handleError);
}

