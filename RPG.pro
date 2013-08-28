#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T12:05:52
#
#-------------------------------------------------

QT       += core gui widgets
QT       += network

TARGET = RPG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chatview.cpp \
    statusview.cpp \
    gameview.cpp \
    server.cpp \
    chatconnection.cpp \
    client.cpp \
    connectdialog.cpp \
    startserverdialog.cpp \
    preferencesdialog.cpp \
    gamepiece.cpp \
    grid.cpp \
    gridadjustdialog.cpp \
    baritem.cpp \
    bargamepiece.cpp \
    bardice.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    chatview.h \
    statusview.h \
    gameview.h \
    constants.h \
    server.h \
    chatconnection.h \
    client.h \
    connectdialog.h \
    startserverdialog.h \
    preferencesdialog.h \
    gamepiece.h \
    grid.h \
    gridadjustdialog.h \
    baritem.h \
    bargamepiece.h \
    bardice.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    connectdialog.ui \
    startserverdialog.ui \
    preferencesdialog.ui \
    gridadjustdialog.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc
