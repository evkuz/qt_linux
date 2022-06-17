QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    ../src/CDobot.cpp \
#    ../src/CDobotCommunicator.cpp \
#    ../src/CDobotConnector.cpp \
#    ../src/CDobotProtocol.cpp \
#    ../src/ComPlatform/Message.cpp \
#    ../src/ComPlatform/Packet.cpp \
#    ../src/ComPlatform/RingBuffer.cpp \
#    ../src/DobotDevice/UdpPort.cpp \
#   ../src/DobotDevice/UdpSearch.cpp \
#    ../src/DobotDll.cpp \
#    commandList.cpp \
    dobotcommands.cpp \
    main.cpp \
    mainwindow.cpp \
    robot.cpp \
    serialdevice.cpp

#INCLUDEPATH +=/home/ubuntu/iqr_lit/dobot-magic/src
#INCLUDEPATH +=/home/ubuntu/iqr_lit/dobot-magic/src/ComPlatform
#INCLUDEPATH +=/home/ubuntu/iqr_lit/dobot-magic/src/DobotDevice

HEADERS += \
#    ../src/CDobot.h \
#    ../src/CDobotCommunicator.h \
#    ../src/CDobotConnector.h \
#    ../src/CDobotProtocol.h \
#   ../src/ComPlatform/Message.h \
#   ../src/ComPlatform/Packet.h \
#   ../src/ComPlatform/ProtocolDef.h \
#   ../src/ComPlatform/ProtocolID.h \
#    ../src/ComPlatform/RingBuffer.h \
#    ../src/DobotDevice/UdpPort.h \
#    ../src/DobotDevice/UdpSearch.h \
#    ../src/DobotDll.h \
#    ../src/DobotType.h \
#    commandList.h \
    dobotcommands.h \
    eklib.h \
    mainwindow.h \
    evkuz.h \
    positions.h \
    protocol.h \
    robot.h \
    serialdevice.h


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
