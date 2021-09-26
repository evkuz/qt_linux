QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QT += serialport
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SocketClient.cpp \
    main.cpp \
    mainwindow.cpp \
    hiwonder.cpp \
    mcinfer.cpp \
    neuron.cpp \
    qsimpleserver.cpp \
    robo_math.cpp \
    mythread.cpp \
    webserver.cpp


HEADERS += \
    SocketClient.h \
    evkuz.h \
    mainwindow.h \
    mcinfer.h \
    move_servos.h \
    neuron.h \
    positions.h \
    hiwonder.h \
    qsimpleserver.h \
    robo_math.h \
    mythread.h \
    webserver.h

FORMS += \
    mainwindow.ui

UI_DIR = $$PWD

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    пример использования.txt
