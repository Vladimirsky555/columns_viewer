#-------------------------------------------------
#
# Project created by QtCreator 2023-10-05T10:07:05
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = columns_viewer
TEMPLATE = app
win32: RC_ICONS = $$PWD/bin/icons/app.ico
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += \
        main.cpp \
	mainwindow.cpp \
	database.cpp \
	globalcolors.cpp \
    func.cpp

HEADERS += \
	mainwindow.h \
	database.h \
	gconst.h \
	globalcolors.h \
    func.h

FORMS += \
        mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bin/img.qrc \
    bin/styles.qrc
