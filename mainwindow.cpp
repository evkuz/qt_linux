#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "positions.h"
#include "hiwonder.h"

#include <stdlib.h>
#include <stdio.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QList ports = QSerialPortInfo



    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->comL->addItem(info.portName(),info.portName());
    }

    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    QByteArray ba = target_name.toLocal8Bit();
    const char *c_str = ba.data();
    printf("Appname : %s", c_str);
    Robot = new HiWonder(); // Без этого будет "The program has unexpectedly finished", хотя в начале писала, что это ambiguous
    Robot->Log_File_Open(Log_File_Name);

    QString str = "The application \"";  str +=target_name; str += "\"";
    Robot->Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));

    connect( this, SIGNAL (Open_Port_Signal(QString)), Robot, SLOT(Open_Port_Slot(QString)));



}

MainWindow::~MainWindow()
{
    GUI_Write_To_Log(0xffff, "Program is going to be closed");
    delete ui;
    delete Robot;

}
//+++++++++++++++++++++++++++++++++++++++
void MainWindow::GUI_Write_To_Log (int value, QString log_message)
{
    QDateTime curdate ;
    QTextStream uin(&Robot->LogFile);

    QString str, str2;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_message << "\n";


}


//+++++++++++++++++++++++++++++++++++++++
void MainWindow::on_openButton_clicked()
{
QString portname = ui->comL->currentText();
emit Open_Port_Signal(portname);
//    serial.setPortName(ui->comL->currentText());
//    serial.open(QIODevice::ReadWrite);
//    serial.setBaudRate(QSerialPort::Baud115200);
//    serial.setDataBits(QSerialPort::Data8);
//    serial.setParity(QSerialPort::NoParity);
//    serial.setStopBits(QSerialPort::OneStop);
//    serial.setFlowControl(QSerialPort::NoFlowControl);



   /*
    if (!serial.open(QIODevice::ReadWrite)) {
        processError(tr("Can't open %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setBaudRate(QSerialPort::Baud19200)) {
        processError(tr("Can't set rate 9600 baud to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setDataBits(QSerialPort::Data8)) {
        processError(tr("Can't set 8 data bits to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setParity(QSerialPort::NoParity)) {
        processError(tr("Can't set no patity to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setStopBits(QSerialPort::OneStop)) {
        processError(tr("Can't set 1 stop bit to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setFlowControl(QSerialPort::NoFlowControl)) {
        processError(tr("Can't set no flow control to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }
    */
}

void MainWindow::on_sitButton_clicked()
{
    QByteArray dd = QByteArray::fromRawData(sit_down_position, 6);
    Robot->GoToPosition(dd);
//    for (int i = 0; i<= 57; i++)
//    {
//        dd.append("A");
//    }
//    serial.write(dd);


    //serial.flush();
    //serial.waitForBytesWritten(50);
}
//+++++++++++++++++++++++++++++++

void MainWindow::on_stand_upButton_clicked()
{
    QByteArray dd = QByteArray::fromRawData(hwr_Start_position, 6);
    Robot->GoToPosition(dd);
    dd = QByteArray(reinterpret_cast<const char*>(hwr_Start_position), 10);
    QString str = QString(dd);

    QByteArray cc;
    //cc.replace(hwr_Start_position, dd);

    str = "Data : ";
    for (int i =0; i<= DOF-1; i++)
    {
        str += QString::number(Servos[i]);
        str += ", ";
    }


    //str += QString(cc);
    str += " From standup";



    GUI_Write_To_Log(0xff10, str);
}

void MainWindow::on_S1_verSlider_valueChanged(int value)
{
    Servos[0]   = value;
    const char *   pchar;
    pchar = static_cast<const char *>(static_cast<void *>(Servos));
    QByteArray dd = QByteArray::fromRawData(pchar, 6);
    Robot->GoToPosition(dd);


}
