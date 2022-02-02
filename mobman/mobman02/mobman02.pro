QT += core
QT -= gui

TARGET = mobman02
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/qt_projects/$${TARGET}
!isEmpty(target.path): INSTALLS += target
