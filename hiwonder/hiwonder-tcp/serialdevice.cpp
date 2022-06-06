#include "serialdevice.h"
#include <QTextStream>
#include <stdio.h>
#include <iostream>



SerialDevice::SerialDevice(QObject *parent) : QObject(parent)
{
    connect(&serial, SIGNAL(readyRead()), this, SLOT(ReadFromSerial_Slot()));
}

SerialDevice::~SerialDevice()
{
 serial.close();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SerialDevice::Log_File_Open(QString lname)
{
    LogFile.setFileName(lname);
    LogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SerialDevice::Write_To_Log(int value, QString log_message)
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
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++ REMEMBER WHILE OPENING PORT !!!!!
/*
 * Для работы arduino-плат через com-порт dialout, добавляемся в группу
 * sudo usermod -a -G dialout <username>

where <username> is your Linux user name. You will need to log out and log in again for this change to take effect.

*/
int SerialDevice::Open_Port_Slot(QString portname)
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

}// Open_Port_Slot
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Слот обработки сигнала QSerialPort::readyRead()
// Считываем данные из Serial port.
// А вот как быть с их обработкой ? Куда передавать?
// Передаем через сигнал GetData_Signal(qbuf);
// У каждого девайса свой код будет.
void SerialDevice::ReadFromSerial_Slot()
{
    QString str;
    qint64 numBytes;
        numBytes = serial.bytesAvailable ();

        str = "There are "; //
        str += QString::number (numBytes);
        str += " bytes from device to read";
        this->Write_To_Log(0xF001, str);

        qbuf = serial.readAll();
        // По идее на этом всё. Далее emit signal (GetData_Signal) от том, что данные в массиве готовы к обработке.
        str = "From device : ";
        str += QString(qbuf);
        this->Write_To_Log(0xF001, str);
        // И вот теперь надо вводить флаг проверки текста сообщения на предмет наличия "DONE"
        // Делаем вывод в консоль
        std::cout<<"From Serial:" << str.toStdString ()<< std::endl;
        // Проверка на наличие кода, означающего конец передачи.
        // Логично просто передавать все сообщение "наверх" для последующего анализа.
        // Т.е. в return можно возвращать указатель на буфер с данными.

        emit this->GetData_Signal(qbuf);


}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

