#include <QObject>
#include "hiwonder.h"
#include <QDataStream>
#include <QTextStream>
#include <stdlib.h>
#include <stdio.h>

#include <QSerialPort>
#include <QSerialPortInfo>


//HiWonder::HiWonder(QObject *parent) : QObject(parent)
HiWonder::HiWonder()
{
    // Инициализируем буфер данными
    memset(byInputBuffer, 0xEE, robot_buffer_SIZE); //sizeof(byInputBuffer)

}
//+++++++++++++++++
HiWonder::~HiWonder()
{
    LogFile.close();
    }
//++++++++++++++++++++++++++++++
void HiWonder::Log_File_Open(QString lname)
{
    LogFile.setFileName(lname);
    LogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
}
//++++++++++++++++++++++++++++++
void HiWonder::Write_To_Log (int value, QString log_message)
{
    QDateTime curdate ;
    QTextStream uin(&LogFile);

    QString str, str2;
    //int value = 0xf000;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_message << "\n";

}
//++++++++++++++++
void HiWonder::Open_Port_Slot(QString portname)
{
    serial.setPortName(portname);
    serial.open(QIODevice::ReadWrite);
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    QString str = "Serial port is opened";
    this->Write_To_Log(0xFF00, str);

}
//++++++++++++++++++++++
void HiWonder::GoToPosition(QByteArray &position, const char *servo)
{
    QString str, str2;
    int sz = position.size();
    if (sz > robot_buffer_SIZE) sz = robot_buffer_SIZE;

   serial.write(position);
   serial.waitForBytesWritten();

    // Для проверки
//    str = "To Robot in hex: ";
//    str += QString(position.toHex());
//    //str = str + str2;
//    this->Write_To_Log(0xF001, str);

    void *const tmp = const_cast<char*>(servo);
    unsigned char* sData = static_cast<unsigned char*>(tmp);

    str = "To Robot: ";
    for (int i=0; i<= DOF - 1; i++){
        str += QString::number(sData[i]);
        str+= ", ";
    }
    this->Write_To_Log(0xF001, str);
    serial.waitForReadyRead();

    str = "Ready to read data from robot";
    this->Write_To_Log(0xF001, str);
    qbuf = serial.readAll();
    //qbuf = "askdjhfakjhfak";
    str = "From Robot ";
    str += QString(qbuf);
    this->Write_To_Log(0xF001, str);

}
