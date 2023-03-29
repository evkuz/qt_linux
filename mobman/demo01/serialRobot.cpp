#include <QObject>
#include "serialRobot.h"
#include <QDataStream>
#include <QTextStream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <QSerialPort>
#include <QSerialPortInfo>


//SerialRobot::SerialRobot(QObject *parent) : QObject(parent)
SerialRobot::SerialRobot()
{
    // Инициализируем буфер данными QByteArray
    memset(byInputBuffer, 0xEE, sizeof(byInputBuffer)); // robot_buffer_SIZE
    MOVEMENT_DONE = true;
    SerialIsOpened = false;
    qbuf.resize (robot_buffer_SIZE);
    memset(outputData, 0xDD, szData); //Инициализация массива с данными для отправки
   // this->SetCurrentStatus ("wait");
    current_st_index = 0;
//    this->current_status = statuslst.at(current_st_index); // "init" state //"Ready";

    connect(&serial, &QSerialPort::errorOccurred, this, &SerialRobot::serialErrorParcer, Qt::QueuedConnection);

    // Инициализируем все экшены
//    getbox_Action.name = "get_box";
//    getbox_Action.rc = -4;
//    getbox_Action.info = "waiting";

//    parking_Action.name = "parking";
//    parking_Action.rc = -4;
//    parking_Action.info = "waiting";

//    ready_Action.name = "ready";
//    ready_Action.rc = -4;
//    ready_Action.info = "waiting";

//    forMoving_Action.name = "formoving";
//    forMoving_Action.rc = -4;
//    forMoving_Action.info = "waiting"; //Get ready for moving accross

//    putbox_Action.name = "put_box";
//    putbox_Action.rc = -4;
//    putbox_Action.info = "waiting";


//    setservos_Action.name = "setservos=";
//    setservos_Action.rc = -4;
//    setservos_Action.info = "Set the servos to correspondent values";

  //  STAT_getbox_Action = {"MOBMAN", 0, "Action is accepted", "init", };

  //  ActionState putbox_Action {"put_box", -4, "waiting"};

    ManipulatorState.action = "Nothhing";
    ManipulatorState.state = statuslst.at(0); // "init" state
    active_command = "nothing"; //this->actionlst.at(1); // "nothing"

}
//+++++++++++++++++
SerialRobot::~SerialRobot()
{
    LogFile.close();
    serial.close();
    this->deleteLater();
    }
//++++++++++++++++++++++++++++++
void SerialRobot::Log_File_Open(QString lname)
{
    LogFile.setFileName(lname);
    LogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
}
//+++++++++++++++++++++
// ОТкрытие файла для записи координат и углов. Для ускорения процесса набора тестовых точек
void SerialRobot::Source_Points_File_Open(QString fname)
{
    SourceFile.setFileName(fname);         //QIODevice::Truncate
    SourceFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);

}
//++++++++++++++++++++++++++++++
void SerialRobot::Write_To_Log (int value, QString log_message)
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
//++++++++++++++++++++++++++++
void SerialRobot::Write_To_Source(int value, QString points_data)
{
    QDateTime curdate ;
    QTextStream uin(&SourceFile);

    QString str, str2;
    //int value = 0xf000;
    curdate = QDateTime::currentDateTime();
    str = QString::number(value); // Убрать варнинг о неиспользуемом value

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

//++++++++++++++++++++++
// Задаем роботу углы для нужной позиции - отправляем данные для углов в Serial port
void SerialRobot::GoToPosition(QByteArray &position)//, const char *servo)
{
   QString str;
   int value = 0xF002;
   this->MOVEMENT_DONE = false;

   if (!SerialIsOpened) {str = "WARNING !!!! Serial port is NOT opened ! The data has NOT been sent."; this->Write_To_Log(0xF001, str); return;}
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
    this->Write_To_Log(value, str);

}
//+++++++++++++++++++++++++++++++
// Слот сигнала &QSerialPort::readyRead
// Считываем данные из Serial port, т.е. от робота.  code From Robot :
void SerialRobot::ReadFromSerial_Slot ()
{
    QString str;
    int value = 0xF001;
    qint64 numBytes;
        numBytes = serial.bytesAvailable ();
        if (numBytes < 32) {return;}

        str = "There are "; //
        str += QString::number (numBytes);
        str += " bytes from robot to read";
        this->Write_To_Log(value, str);

        qbuf = serial.readAll();
        str = "From Robot : ";
        str += QString(qbuf);
        this->Write_To_Log(value, str);
        // И вот теперь надо вводить флаг проверки текста сообщения на предмет наличия "DONE"

//        std::cout<<"From Serial:" << str.toStdString ()<< std::endl;
        QStringList list_str = str.split(QLatin1Char(' '), Qt::SkipEmptyParts);
//        for (int i=0; i< list_str.size (); i++){
//            this->Write_To_Log(value, list_str.at (i));
//            str = list_str.at (i);
//            if ( str == "DONE!"){this->MOVEMENT_DONE = true;}

//        }
        str="DONE!"; // Актуально при отладке сложного движения
        if (list_str.contains (str)) {this->MOVEMENT_DONE = true; this->Write_To_Log(value, "Robot finished A STEP from queued commands "); }
//        else this->Write_To_Log(value, "Robot still running");


        str="LAST";
        if (list_str.contains (str)) {
            emit this->Moving_Done_Signal();
            this->MOVEMENT_DONE = true;
            this->Write_To_Log(value, "Robot TOTALLY finished complex command");
//            this->current_status = "done";

        }

//   if (this->MOVEMENT_DONE) this->Write_To_Log(value, "Robot finished");
//

}// ReadFromSerial_Slot
//++++++++++++++++++++++++++++++++++++++++++++++++

void SerialRobot::serialErrorParcer(QSerialPort::SerialPortError error)
{
    int value = 0x5555;
    QString str = "";

    switch (error){

    case QSerialPort::NotOpenError:
        str = "This error occurs when an operation is executed that ";
        str += "can only be successfully performed if the device is open.";
        Write_To_Log(value,str);
    break;

    case QSerialPort::WriteError:
        str = "An I/O error occurred while WRITING the data.";
        Write_To_Log(value,str);
        break;
    case QSerialPort::ReadError:
        str = "An I/O error occurred while READING the data.";
        Write_To_Log(value,str);
        break;
    case QSerialPort::ResourceError:
        str = "An I/O error occurred when a resource becomes unavailable, ";
        str += "e.g. when the device is unexpectedly removed from the system.";
        Write_To_Log(value,str);
        break;
    case QSerialPort::UnsupportedOperationError:
        str = "The requested device operation is not supported or prohibited by" ;
        str += "the running operating system.";
        Write_To_Log(value,str);
        break;
    case QSerialPort::TimeoutError:
        str = "A timeout error occurred.";
        Write_To_Log(value,str);
        break;

    case QSerialPort::UnknownError:
        str = "An unidentified error occurred.";
        Write_To_Log(value,str);
        break;
    default:
        str = "Robot Serial port UnknownError";
        Write_To_Log(value,str);

    }



}
//++++++++++++++++++++++++
void SerialRobot::SetCurrentStatus(QString newStatus) {
    this->current_status = newStatus;
//    emit this->StatusChangedSignal(newStatus);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++
int SerialRobot::Open_Port_Slot(QString portname)

{
    bool OK;
    int serial_error;
    int exit_code;
    QString stt;

    serial.setPortName(portname); //portname == "ttyUSB0"
    OK = true;
    serial_error = 777;
    exit_code = 2;
    // OK = serial.open(QIODevice::ReadWrite);
    if (!serial.open(QIODevice::ReadWrite))
    {
        OK = false; serial_error = serial.error();
        this->Write_To_Log(0xFF00, "Error opening Serial port !!! "+portname);
        SerialIsOpened = false;

        stt = "Error code is ";
        stt += QString::number (serial_error);
        //https://doc.qt.io/qt-5/qserialport.html#SerialPortError-enum
        if (serial_error == 1) { stt += " - Device NOT found.";}
        if (serial_error == 3) {stt += " attempting to open an already opened device";}
        this->Write_To_Log(0xFF00,stt);
        serial.clearError(); //Очищаем ошибку, чтобы заново запустить
        exit_code = 0; //false

        //Тут запускаем таймер и открываем порт в таймере

    } //"Error opening Serial port !!!");}

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
            if (OK) this->Write_To_Log(0xFF00, str);
            exit_code = 1; //success
    }
    return exit_code;

}
