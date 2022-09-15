QT += core network serialport
QT -= gui

CONFIG += c++11 console
TARGET = demo01
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$(LOCLIBS)
#INCLUDEPATH += $$system( echo $LOCLIBS )
SOURCES += main.cpp \
    jsoninfo.cpp \
    clientsocket.cpp \
    cvdevice.cpp \
    json_answer.cpp \
    mainprocess.cpp \
    serialRobot.cpp \
    tcpParcing.cpp  \
    $$(LOCLIBS)/manipulator/meta/qsimpleserver.cpp \
    $$(LOCLIBS)/manipulator/meta/qsocketthread.cpp


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/qt_projects/$${TARGET}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    jsoninfo.h \
    clientsocket.h \
    cvdevice.h \
    evkuz.h \
    positions.h \
    protocol.h \
    mainprocess.h \
    serialRobot.h \
    tcpParcing.h \
    workflow.h \
    $$(LOCLIBS)/manipulator/meta/qsimpleserver.h \
    $$(LOCLIBS)/manipulator/meta/qsocketthread.h

