QT += core network  serialport
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$(LOCLIBS)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    jsoninfo.cpp \
        main.cpp \
    hiwonder.cpp \
    neuron.cpp \
    $$(LOCLIBS)/manipulator/meta/SocketClient.cpp \
    mainprocess.cpp \
    $$(LOCLIBS)/manipulator/meta/mcinfer.cpp \
    tcpParcing.cpp \
    $$(LOCLIBS)/manipulator/meta/qsimpleserver.cpp \
    $$(LOCLIBS)/manipulator/meta/qsocketthread.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    evkuz.h \
    hiwonder.h \
    jsoninfo.h \
    $$(LOCLIBS)/manipulator/meta/mcinfer.h \
    neuron.h \
    positions.h \
    protocol.h \
    $$(LOCLIBS)/manipulator/meta/SocketClient.h \
    mainprocess.h \
    $$(LOCLIBS)/manipulator/meta/qsimpleserver.h \
    $$(LOCLIBS)/manipulator/meta/qsocketthread.h
