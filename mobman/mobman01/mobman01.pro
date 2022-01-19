#-------------------------------------------------
#
# Project created by QtCreator 2021-11-03T17:24:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mobman01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/qt_projects/$${TARGET}
!isEmpty(target.path): INSTALLS += target
