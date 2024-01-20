# Install

1. copy patches directory to /etc/portage 
2. emerge deps
<pre>
dev-qt/qtcore-5.15.11-r1:5/5.15.11                                                                                                                 
dev-qt/qtdbus-5.15.11:5/5.15
dev-qt/qtdeclarative-5.15.11-r2:5/5.15
dev-qt/qtgraphicaleffects-5.15.11:5
dev-qt/qtgui-5.15.11-r2:5/5.15.11
dev-qt/qtmultimedia-5.15.11:5/5.15
dev-qt/qtnetwork-5.15.11:5/5.15
dev-qt/qtopengl-5.15.11:5/5.15
dev-qt/qtprintsupport-5.15.11:5/5.15
dev-qt/qtquickcontrols-5.15.11:5
dev-qt/qtquickcontrols2-5.15.11:5/5.15
dev-qt/qtsql-5.15.11-r1:5/5.15.11
dev-qt/qttest-5.15.11:5/5.15
dev-qt/qtwebchannel-5.15.11:5/5.15
dev-qt/qtwebengine-5.15.11_p20231120:5/5.15
dev-qt/qtwebview-5.15.11:5/5.15
dev-qt/qtwidgets-5.15.11-r1:5/5.15
dev-qt/qtxml-5.15.11:5/5.15
dev-util/lxqt-build-tools-0.12.0:0
x11-libs/qtermwidget-1.2.0:0/1.2.0
</pre>
with following use flags
<pre>
media-libs/gst-plugins-base -opengl
media-libs/freetype static-libs
app-arch/bzip2 static-libs
sys-libs/zlib static-libs
media-libs/libpng static-libs
virtual/jpeg static-libs
dev-qt/qtgui linuxfb jpeg udev evdev -libinput egl eglfs debug
media-libs/libglvnd X
dev-qt/qtmultimedia widgets gstreamer qml debug
sys-libs/zlib minizip
dev-libs/libxml2 icu
dev-qt/qtwebengine widgets -bindist debug
dev-qt/qtcore icu debug
media-libs/gst-plugins-base egl gbm opengl
media-plugins/gst-plugins-meta alsa dvd ffmpeg flac http mp3 mpeg ogg vorbis
dev-qt/qtprintsupport cups
app-text/ghostscript-gpl cups
</pre>
3. qmake adrenlinerushui.pro
4. make
5. set env vars
<pre>
QT_QPA_EGLFS_KMS_CONFIG=/home/austin/.adrenlinerushui.conf
QT_QPA_PLATFORM=eglfs
</pre>

# Multiscreen (on same card) Support

1. multiscreen.patch provide support for this 
2. configure KMS
<pre>
{
    "device": "/dev/dri/card0",
    "separateScreens": false,
    "outputs": [
        {
            "name": "VGA1",
            "virtualIndex": 0,
            "mode": "1920x1080"
        },
        {
            "name": "HDMI3",
            "virtualIndex": 1,
            "mode": "1920x1080"
        },
        {
            "name": "LVDS1",
            "mode": "off"
        }
    ]
}
</pre>
