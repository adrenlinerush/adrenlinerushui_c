# Automatically generated by qmake (3.1) Mon Mar 20 15:40:48 2023
######################################################################

QT += core gui network widgets webengine webenginewidgets multimedia multimediawidgets

TEMPLATE = app
TARGET = AdrenlinerushUI
INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
SOURCES += \
    lineedit.cpp \
    documentbrowser.cpp \
    webenginepage.cpp\
    webengineview.cpp \
    browser.cpp \
    tinyexpr.c \
    calculator.cpp \
    videoplayer.cpp \
    terminal.cpp \
    filebrowser.cpp \
    markdownbrowser.cpp \
    mainwindow.cpp \
    mdiarea.cpp \
    mdisubwindow.cpp \
    statusbar.cpp \
    main.cpp

HEADERS += \
    lineedit.h \
    documentbrowser.h \
    webenginepage.h \
    webenginepage.h \
    browser.h \
    tinyexpr.h \
    calculator.h \
    videoplayer.h \
    terminal.h \
    filebrowser.h \
    markdownbrowser.h \
    mainwindow.h \
    mdiarea.h \
    mdisubwindow.h \
    statusbar.h

LIBS += -lqtermwidget5 

CONFIG+=debug
