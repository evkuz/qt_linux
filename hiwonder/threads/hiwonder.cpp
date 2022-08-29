#include <QObject>
#include "hiwonder.h"
#include <QDataStream>
#include <QTextStream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <QSerialPort>
#include <QSerialPortInfo>


//HiWonder::HiWonder(QObject *parent) : QObject(parent)
HiWonder::HiWonder()
{
    MOVEMENT_DONE = true;
    qbuf.resize (robot_buffer_SIZE);
    memset(outputData, 0xDD, szData); //Инициализация массива с данными для отправки
    this->current_status = "wait";
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

void HiWonder::Source_Points_File_Open(QString fname)
{
    SourceFile.setFileName(fname);         //QIODevice::Truncate
    SourceFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);

}

void HiWonder::Command_List_File_Open(QString lstname)
{
    CommandFile.setFileName(lstname);
    CommandFile.open(QIODevice::ReadOnly | QIODevice::Text);

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

void HiWonder::Write_To_Source(QString points_data)
{
    QTextStream uin(&SourceFile); uin << points_data << "\n";
}
//++++++++++++++++ REMEMBER WHILE OPENING PORT !!!!!
/*
 * sudo usermod -a -G dialout <username>

where <username> is your Linux user name. You will need to log out and log in again for this change to take effect.

*/
void HiWonder::Open_Port_Slot(QString portname)

{
    bool OK;
    int serial_error;
    QString stt;

    serial.setPortName(portname); //portname == "ttyUSB0"
    OK = true;
    serial_error = 1;
   // OK = serial.open(QIODevice::ReadWrite);
    if (!serial.open(QIODevice::ReadWrite)) { OK = false; serial_error = serial.error(); this->Write_To_Log(0xFF00, "Error opening Serial port !!!");} //"Error opening Serial port !!!");}
    stt = QString::number (serial_error);
    this->Write_To_Log(0xFF00,stt);

    // https://www.linuxhowtos.org/data/6/perror.txt
//    if (!serial.open(QIODevice::ReadWrite)) {
//        processError(tr("Can't open %1, error code %2")
//                     .arg(serial.portName()).arg()));


    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    QString str = "Serial port "; str += portname; str += " is opened";
    if (OK) this->Write_To_Log(0xFF00, str);

}
//++++++++++++++++++++++
// Задаем роботу углы для нужной позиции - отправляем данные для углов в Serial port
void HiWonder::GoToPosition(QByteArray &position)//, const char *servo)
{
   int value = 0xF001;
   QString str;
   this->MOVEMENT_DONE = false;
   position.resize (szData);
   serial.write(position);
   serial.waitForBytesWritten();

//    void *const tmp = const_cast<char*>(servo);
//    unsigned char* sData = static_cast<unsigned char*>(tmp);
   //unsigned char sData [7]= {0,0,0,0,0,0,0};
   memcpy(&outputData, position,szData);
   // Данные роботу отправили, теперь запись в лог об этом

    str = "To Robot: ";
    for (int i=0; i< szData; i++){
        str+= QString::number(outputData[i]);
        str+= ", ";
    }
    str.truncate(str.lastIndexOf(","));
    this->Write_To_Log(value, str);

}
//+++++++++++++++++++++++++++++++
// Слот сигнала QSerialPort::readyRead()
// DATA From Robot :
void HiWonder::ReadFromSerial_Slot ()
{
    QString str;
    int numBytes;
        numBytes = serial.bytesAvailable ();

        str = "There are "; //
        str += QString::number (numBytes);
        str += " bytes from robot to read";
        this->Write_To_Log(0xF001, str);

        qbuf = serial.readAll();
        str = "From Robot : ";
        str += QString(qbuf);
        this->Write_To_Log(0xF001, str);
        // И вот теперь надо вводить флаг проверки текста сообщения на предмет наличия "DONE"

        std::cout<<"From Serial:" << str.toStdString ()<< std::endl;
        QStringList list_str = str.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        str="DONE!";
        if (list_str.contains (str)) {this->MOVEMENT_DONE = true; this->Write_To_Log(0xF001, "Robot finished"); }
        else this->Write_To_Log(0xF001, "Robot still running");


        str="LAST";
        if (list_str.contains (str)) {
            this->MOVEMENT_DONE = true;
            this->Write_To_Log(0xF001, "Robot finished complex command");
            this->current_status = "done";
            emit this->Moving_Done_Signal();
        }


} // ReadFromSerial_Slot
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void HiWonder::SetCurrentStatus(QString newStatus) {
    this->current_status = newStatus;
//    emit this->StatusChangedSignal(newStatus);

}
