QT += core network serialport
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = demoJSON


TEMPLATE = app

INCLUDEPATH += $$(EXTLIBS)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        cvdevice.cpp \
        hiwonder.cpp \
        json_answer.cpp \
        qsimpleserver.cpp \
        qsocketthread.cpp \
        mainprocess.cpp \
        tcpParcing.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/qt_projects/$${TARGET}
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    cvdevice.h \
    evkuz.h \
    hiwonder.h \
    positions.h \
    protocol.h \
    qsimpleserver.h \
    qsocketthread.h \
    mainprocess.h \

