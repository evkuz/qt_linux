#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "positions.h"

#include <stdlib.h>
#include <stdio.h>
#include <QDataStream>
#include <QTextStream>


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
    Log_File_Open(Log_File_Name);

    QString str = "The application \"";  str +=target_name; str += "\""; Write_To_Log(str.append(" is started successfully!!!\n"));


}

MainWindow::~MainWindow()
{
    delete ui;
    LogFile.close();
}
//+++++++++++++++++++++++++++++++++++++++

void MainWindow::Write_To_Log (QString log_message)
{
    QDateTime curdate ;
    QTextStream uin(&LogFile);

    QString str, str2;
    int value = 0xf000;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_message;

}
//+++++++++++++++++++++++++++++++++++++++
void MainWindow::on_openButton_clicked()
{


    serial.setPortName(ui->comL->currentText());
    serial.open(QIODevice::ReadWrite);
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);



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
    QByteArray dd = QByteArray::fromRawData(hwr_Start_position, 6);
    serial.write(dd, 6);
    serial.flush();
}
//+++++++++++++++++++++++++++++++
void MainWindow::Log_File_Open(QString lname)
{
    LogFile.setFileName(lname);
    LogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
}
