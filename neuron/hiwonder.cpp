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
    MOVEMENT_DONE = true;
    qbuf.resize (robot_buffer_SIZE);
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
   // OK = serial.open(QIODevice::ReadWrite);
    if (!serial.open(QIODevice::ReadWrite)) { OK = false; serial_error = serial.error(); this->Write_To_Log(0xFF00, "Error opening Serial port !!!");} //"Error opening Serial port !!!");}
    stt = QString::number (serial_error);
    this->Write_To_Log(0xFF00,stt);
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
void HiWonder::GoToPosition(QByteArray &position)//, const char *servo)
{
    QString str;
//    int sz = position.size();
//    if (sz > robot_buffer_SIZE) sz = robot_buffer_SIZE;
    this->MOVEMENT_DONE = false;
   position.resize (7);
   serial.waitForBytesWritten();
   serial.write(position);
   serial.flush(); // Пробуем очистить буфер совсем
   serial.waitForBytesWritten();

    // Для проверки
//    str = "To Robot in hex: ";
//    str += QString(position.toHex());
//    //str = str + str2;
//    this->Write_To_Log(0xF001, str);

//    void *const tmp = const_cast<char*>(servo);
//    unsigned char* sData = static_cast<unsigned char*>(tmp);
   unsigned char sData [7]= {0,0,0,0,0,0,0};
   memcpy(&sData, position,7);

    str = "To Robot: ";
    for (int i=0; i<= szData - 1; i++){
    //    str += QString::number(sData[i]);
        //str+= QString::number(position.at(i));
        str+= QString::number(sData[i]);
        str+= ", ";

    }
    this->Write_To_Log(0xF001, str);
//    serial.waitForReadyRead();

//    str = "Ready to read data from robot";
//    this->Write_To_Log(0xF001, str);
//    qbuf = serial.readAll();
//    //qbuf = "askdjhfakjhfak";
//    str = "From Robot :";
//    str += QString(qbuf);
//    this->Write_To_Log(0xF001, str);

}
//+++++++++++++++++++++++++++++++
// code From Robot :
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
    //    //qbuf = "askdjhfakjhfak";
        str = "From Robot : ";
        str += QString(qbuf);
        this->Write_To_Log(0xF001, str);
        // И вот теперь надо вводить флаг проверки текста сообщения на предмет наличия "DONE"
        QStringList list_str = str.split(QLatin1Char(' '), Qt::SkipEmptyParts);
//        for (int i=0; i< list_str.size (); i++){
//            this->Write_To_Log(0xF001, list_str.at (i));
//            str = list_str.at (i);
//            if ( str == "DONE!"){this->MOVEMENT_DONE = true;}

//        }
        str="DONE!";
        if (list_str.contains (str)) {this->MOVEMENT_DONE = true; this->Write_To_Log(0xF001, "Robot finished"); }



//   if (this->MOVEMENT_DONE) this->Write_To_Log(0xF001, "Robot finished");
//   else this->Write_To_Log(0xF001, "Robot still running");

}
