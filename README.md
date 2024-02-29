# Install

### Gentoo

1. Add my portage [overlay](https://wiki.adrenlinerush.net/doku.php?id=gentoo_portage_overlay).
2. Set Use Flags 
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
3. Emerge
<pre>
emerge --ask adrenlinerushui
</pre>
4. set env vars
<pre>
QT_QPA_PLATFORM=eglfs
</pre>
or
<pre>
QT_QPA_PLATFORM=linuxfb
</pre>
5. run it
<pre>
adrenlinerusui
</pre>

### Armbian Bookworm (aarch64)

*** Multiscreen Support will be pulled in. ***

1. Add my apt [repo](https://wiki.adrenlinerush.net/doku.php?id=armbian_bookworm_aarch64_-_custom_packages).
2. apt-get update
3. apt-get install adrelinerushui
4. adrenlinerushui

### Multiscreen (on same card) Support

1. copy patches /etc/portage - multiscreen.patch provide support for this 
2. emerge dev-qt/gui
3. create a conf file
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
4. export QT_QPA_EGLFS_KMS_CONFIG=/path/to/conf
