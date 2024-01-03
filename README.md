# Install

1. emerge deps
2. qmake adrenlinerushui.pro
3. make

# Multiscreen (on same card) Support

1. create the following directories within /etc/portage/patches/dev-qt/:

<code>
qtcore-5.15.11-r1
qtgui-5.15.11-r2
qtopengl-5.15.11
qtwidgets-5.15.11-r1
</code>

2. place multiscreen.patch in each of those directories
3. then you can emerge the deps (assuming qt 5.15.11)
