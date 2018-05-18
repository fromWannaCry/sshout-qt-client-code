#-------------------------------------------------
#
# Project created by QtCreator 2018-05-18T03:36:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sshout-qt
TEMPLATE = app


SOURCES += main.cpp\
        connectionwindow.cpp \
    settingsdialog.cpp \
    mainwindow.cpp

HEADERS  += connectionwindow.h \
    settingsdialog.h \
    mainwindow.h \
    serverinformation.h

FORMS    += connectionwindow.ui \
    settingsdialog.ui \
    mainwindow.ui
