# Install

1. emerge deps
2. qmake adrenlinerushui.pro
3. make
4. set env vars
<code>
QT_QPA_EGLFS_KMS_CONFIG=/home/austin/.adrenlinerushui.conf
QT_QPA_PLATFORM=eglfs
</code>

# Multiscreen (on same card) Support

1. create the following directories within /etc/portage/patches/dev-qt/ (any ebuild you're going to compile with QT5_MODULE="qtbase"):

<code>
qtcore-5.15.11-r1
qtdbus-5.15.11
qtgui-5.15.11-r2
qtnetwork-5.15.11
qtopengl-5.15.11
qtsql-5.15.11-r1
qttest-5.15.11
qtwidgets-5.15.11-r1
qtxml-5.15.11
</code>

2. place multiscreen.patch in each of those directories
3. then you can emerge the deps (assuming qt 5.15.11)
4. configure KMS
<code>
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
</code>
