QT += core network serialport
QT -= gui

CONFIG += c++11 console
TARGET = demo01
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$(EXTLIBS)
#INCLUDEPATH += $$system( echo $EXTLIBS )
SOURCES += main.cpp \
    jsoninfo.cpp \
    clientsocket.cpp \
    cvdevice.cpp \
    hiwonder.cpp \
    json_answer.cpp \
    mainprocess.cpp \
    processAction.cpp \
    tcpParcing.cpp  \
    $$(EXTLIBS)/manipulator/qsimpleserver.cpp \
    $$(EXTLIBS)/manipulator/qsocketthread.cpp


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/qt_projects/$${TARGET}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ProcessAction.h \
    jsoninfo.h \
    clientsocket.h \
    cvdevice.h \
    evkuz.h \
    hiwonder.h \
    positions.h \
    protocol.h \
    mainprocess.h \
    tcpParcing.h \
    workflow.h \
    $$(EXTLIBS)/manipulator/qsimpleserver.h \
    $$(EXTLIBS)/manipulator/qsocketthread.h

