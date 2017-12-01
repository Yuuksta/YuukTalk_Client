#-------------------------------------------------
#
# Project created by QtCreator 2017-11-02T16:47:54
#
#-------------------------------------------------

QT       += core gui
QT       += network

CONFIG   += crypto

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YuukTalk_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chatroom.cpp \
    signuproom.cpp \
    filesender.cpp \
    filereceiver.cpp

HEADERS  += mainwindow.h \
    chatroom.h \
    signuproom.h \
    filesender.h \
    filereceiver.h

FORMS    += mainwindow.ui \
    chatroom.ui \
    signuproom.ui \
    filesender.ui \
    filereceiver.ui

RESOURCES +=
