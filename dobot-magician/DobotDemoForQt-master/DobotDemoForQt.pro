#-------------------------------------------------
#
# Project created by E.Kuznetsov 17.06.2022
#
#-------------------------------------------------

QT       += core gui network xml serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DobotDemoForQt
TEMPLATE = app


SOURCES += main.cpp\
        ../src/CDobot.cpp \
        ../src/CDobotCommunicator.cpp \
        ../src/CDobotConnector.cpp \
        ../src/CDobotProtocol.cpp \
        ../src/ComPlatform/Message.cpp \
        ../src/ComPlatform/Packet.cpp \
        ../src/ComPlatform/RingBuffer.cpp \
        ../src/DobotDevice/UdpPort.cpp \
        ../src/DobotDevice/UdpSearch.cpp \
        ../src/DobotDll.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ../src/CDobot.h \
    ../src/CDobotCommunicator.h \
    ../src/CDobotConnector.h \
    ../src/CDobotProtocol.h \
    ../src/ComPlatform/Message.h \
    ../src/ComPlatform/Packet.h \
    ../src/ComPlatform/ProtocolDef.h \
    ../src/ComPlatform/ProtocolID.h \
    ../src/ComPlatform/RingBuffer.h \
    ../src/DobotDevice/UdpPort.h \
    ../src/DobotDevice/UdpSearch.h \
    ../src/DobotDll.h \
    ../src/DobotType.h \
    ../src/dobotdll_global.h \
    evkuz.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../src/
#INCLUDEPATH += ../src/ComPlatform
#INCLUDEPATH += ../src/DobotDevice

