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
    // Инициализируем буфер данными "0хЕЕ" QByteArray
    memset(byInputBuffer, 0xEE, robot_buffer_SIZE); //sizeof(byInputBuffer)
    MOVEMENT_DONE = true;
    qbuf.resize (robot_buffer_SIZE);
    memset(outputData, 0xDD, szData); //Инициализация массива с данными для отправки
   // this->SetCurrentStatus ("wait");
    this->current_status = "wait";
    active_command = "nothing";
//    QMutex hwmutex
}
//+++++++++++++++++
HiWonder::~HiWonder()
{
    LogFile.close();
    serial.close();
    }
//++++++++++++++++++++++++++++++
void HiWonder::Log_File_Open(QString lname)
{
    LogFile.setFileName(lname);
    LogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
}
//+++++++++++++++++++++
// ОТкрытие файла для записи координат и углов. Для ускорения процесса набора тестовых точек
void HiWonder::Source_Points_File_Open(QString fname)
{
    SourceFile.setFileName(fname);         //QIODevice::Truncate
    SourceFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);

}
//++++++++++++++++++++++++++++++
void HiWonder::Write_To_Log (int value, QString log_message)
{
 //   QMutexLocker locker(&hwMutex);
    QDateTime curdate ;
    QTextStream uin(&LogFile);

    QString str, str2;
    //int value = 0xf000;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_message << "\n";

}
//++++++++++++++++++++++++++++
void HiWonder::Write_To_Source(QString points_data)
{
    QDateTime curdate ;
    QTextStream uin(&SourceFile);

    QString str, str2;
    //int value = 0xf000;
    curdate = QDateTime::currentDateTime();

    //str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
   // str = "X, Y";
    //str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << points_data << "\n";


}
//++++++++++++++++ REMEMBER WHILE OPENING PORT !!!!!
/*
 * Для работы arduino-плат через com-порт dialout, добавляемся в группу
 * sudo usermod -a -G dialout <username>

where <username> is your Linux user name. You will need to log out and log in again for this change to take effect.

*/
//+++++++++++ Open Serial port
int HiWonder::Open_Port_Slot(QString portname)

{

    int serial_error;
    int exit_code;
    QString stt;

    serial.setPortName(portname); //portname == "ttyUSB0"
    if (!serial.open(QIODevice::ReadWrite))
    {
        serial_error = serial.error();
        this->Write_To_Log(0xFF00, "Error opening Serial port !!! "+portname);
        SerialIsOpened = false;

        stt = "Error code is ";
        stt += QString::number (serial_error);
        //https://doc.qt.io/qt-5/qserialport.html#SerialPortError-enum
        if (serial_error == 1) {stt += " - Device NOT found.";}
        if (serial_error == 2) {stt += " - An error occurred while attempting to open an already opened device by another process or a user not having enough permission and credentials to open.";}
        if (serial_error == 3) {stt += " - attempting to open an already opened device";}
        this->Write_To_Log(0xFF00,stt);
        serial.clearError(); //Очищаем ошибку, чтобы заново запустить
        exit_code = 0; //false

        //Тут запускаем таймер и открываем порт в таймере



    } //if
    else {
        SerialIsOpened = true;
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
    this->Write_To_Log(0xFF00, str);
    exit_code = 1; //success
    }//else

    return exit_code;

}
//++++++++++++++++++++++
// Задаем роботу углы для нужной позиции - отправляем данные для углов в Serial port
void HiWonder::GoToPosition(QByteArray &position)//, const char *servo)
{
    QString str;
    this->MOVEMENT_DONE = false;
    int btAvailable  = serial.bytesAvailable();
    if (btAvailable >0) {return;} // есть непрочитанные данные, ждем onReadyRead
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
    //    str += QString::number(sData[i]);
        //str+= QString::number(position.at(i));
        str+= QString::number(outputData[i]);
        str+= ", ";

    }
    this->Write_To_Log(0xF001, str);

} //GoToPosition
//+++++++++++++++++++++++++++++++++++++++++++++++
// Invocable
void HiWonder::showError(void *errorMessage)
{
    int value = 0x1717;
    QString *str = (QString *) errorMessage;
    Write_To_Log(value, *str);

}
//+++++++++++++++++++++++++++++++

QString HiWonder::GetCurrentStatus() {
    return this->current_status;
} // GetCurrentStatus
//+++++++++++++++++++++++++++++++
// Слот обработки сигнала QSerialPort::readyRead()
// Считываем данные из Serial port, т.е. от робота.  code From Robot :
void HiWonder::ReadFromSerial_Slot ()
{
    QString str;
    //qint64 numBytes;
    QMutexLocker locker(&hwMutex);
        //numBytes = serial.bytesAvailable ();

//        str = "There are "; //
//        str += QString::number (numBytes);
//        str += " bytes from robot to read";
//        this->Write_To_Log(0xF001, str);

        qbuf = serial.readAll();
        str = "From Robot : ";
        str += QString(qbuf);
        this->Write_To_Log(0xF001, str);
        // И вот теперь надо вводить флаг проверки текста сообщения на предмет наличия "DONE"

//        std::cout<<"From Serial:" << str.toStdString ()<< std::endl;
        QStringList list_str = str.split(QLatin1Char(' '), Qt::SkipEmptyParts);
//        for (int i=0; i< list_str.size (); i++){
//            this->Write_To_Log(0xF001, list_str.at (i));
//            str = list_str.at (i);
//            if ( str == "DONE!"){this->MOVEMENT_DONE = true;}

//        }
        str="DONE!";
        if (list_str.contains (str)) {this->MOVEMENT_DONE = true; this->Write_To_Log(0xF001, "Robot finished"); }
        //else this->Write_To_Log(0xF001, "Robot still running");


        str="LAST";
        if (list_str.contains (str)) {
            this->MOVEMENT_DONE = true;
            this->Write_To_Log(0xF001, "Robot finished complex command");
            this->current_status = "done";
            emit this->Moving_Done_Signal();
        }

//   if (this->MOVEMENT_DONE) this->Write_To_Log(0xF001, "Robot finished");
//

}
//++++++++++++++++++++++++++++++++++++++++
// Слот сигнала QSerialPort::errorOccurred
// Google for enum QSerialPort::SerialPortError
// https://doc.qt.io/qt-5/qserialport.html#SerialPortError-enum
void HiWonder::SerialError_slot(QSerialPort::SerialPortError error)
{
 int value = 0x7070;
 QString str, str1;

 switch (error) {

 case 0:
     str = "No Error";
     break;
 case 1:
     str = "An error occurred while attempting to open an non-existing device.";
     break;

 case 2:
     str = "An error occurred while attempting to open an already opened device by another process";
     str += " or a user not having enough permission and credentials to open.";
     break;
 case 3:
     str = "An error occurred while attempting to open an already opened device in this object.";
     break;
 case 7:
     str = "An I/O error occurred while writing the data.";
     break;
 case 8:
     str = "An I/O error occurred while reading the data.";
     break;



 }

// str1 = "Serial Port ERROR !!!!!!!\n";
// str1 += "Error number:  "; str += QString::number(error);
 str1 = "Serial Port ERROR number ";  str1 += QString::number(error); str1 += " == ";
 str.insert(0, str1);
 this->Write_To_Log(value, str);

}
//++++++++++++++++++++++++
void HiWonder::SetCurrentStatus(QString newStatus) {
    this->current_status = newStatus;
//    emit this->StatusChangedSignal(newStatus);

}
