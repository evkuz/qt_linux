QT += core network serialport
QT -= gui

CONFIG += c++11 console
TARGET = demo01
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$(EXTLIBS)
#INCLUDEPATH += $$system( echo $EXTLIBS )
SOURCES += main.cpp \
    clientsocket.cpp \
    cvdevice.cpp \
    hiwonder.cpp \
    json_answer.cpp \
    qsimpleserver.cpp \
    qsocketthread.cpp \
    mainprocess.cpp \
    tcpParcing.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/qt_projects/$${TARGET}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    clientsocket.h \
    cvdevice.h \
    evkuz.h \
    hiwonder.h \
    positions.h \
    protocol.h \
    qsimpleserver.h \
    qsocketthread.h \
    mainprocess.h \
    workflow.h

