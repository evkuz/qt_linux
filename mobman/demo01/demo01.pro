QT += core network serialport
QT -= gui

CONFIG += c++11 console
TARGET = demo01
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    hiwonder.cpp \
    neuron.cpp \
    qsimpleserver.cpp \
    qsocketthread.cpp \
#    SocketClient.cpp \
    mainprocess.cpp \
    mcinfer.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/qt_projects/$${TARGET}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    evkuz.h \
    hiwonder.h \
    mcinfer.h \
    neuron.h \
    positions.h \
    protocol.h \
    qsimpleserver.h \
    qsocketthread.h \
#    SocketClient.h \
    mainprocess.h
