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
    QByteArray dd = QByteArray::fromRawData(sit_down_position, 6);
    for (int i = 0; i<= 57; i++)
    {
        dd.append("A");
    }
    serial.write(dd);
    //serial.flush();
    //serial.waitForBytesWritten(50);
}
//+++++++++++++++++++++++++++++++
void MainWindow::Log_File_Open(QString lname)
{
    LogFile.setFileName(lname);
    LogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
}

void MainWindow::on_stand_upButton_clicked()
{
    QByteArray dd = QByteArray::fromRawData(hwr_Start_position, 6);
    for (int i = 0; i<= 57; i++)
    {
        dd.append("A");
    }
    serial.write(dd);
    //serial.flush();
    //serial.waitForBytesWritten(50);

}

void MainWindow::on_closeButton_clicked()
{
    serial.close();
}
//+++++++++++++++++++++++++ update servos from LineEdits
void MainWindow::on_servo_1_lineEdit_editingFinished()
{
    servos[0] = ui->servo_1_lineEdit->text().toInt();
}


void MainWindow::on_servo_2_lineEdit_editingFinished()
{
    servos[1] = ui->servo_2_lineEdit->text().toInt();
}


void MainWindow::on_servo_3_lineEdit_editingFinished()
{
    servos[2] = ui->servo_3_lineEdit->text().toInt();
}

void MainWindow::on_servo_4_lineEdit_editingFinished()
{
    servos[3] = ui->servo_4_lineEdit->text().toInt();
}


void MainWindow::on_servo_5_lineEdit_editingFinished()
{
    servos[4] = ui->servo_5_lineEdit->text().toInt();
}


void MainWindow::on_servo_6_lineEdit_editingFinished()
{
    servos[5] = ui->servo_6_lineEdit->text().toInt();
}
//+++++++++++++++++++++++++++++++++++++++++
//Send data from linedits to robot
void MainWindow::on_set_posButton_clicked()
{// Преобразуем указатель на массив char в указатель на массив DWORD


  //  mA = static_cast<DWORD *>(static_cast<void *>(ek_a));

    //unsigned char *data;
    //data = static_cast<unsigned char *>(static_cast<uint8_t *>(servos));
    //QByteArray dd = QByteArray(reinterpret_cast<unsigned char *>(data));
   // const char *dd = reinterpret_cast<const char *>(servos);
  //  reinterpret_cast<const char*>(foo())
   // QByteArray ba(dd);

 //   QByteArray dd = QByteArray::fromRawData(data, 6);
 /*
    for (int i = 0; i<= 57; i++)
    {
        dd.append("A");
    }
 */
 serial.write(reinterpret_cast<const char *>(servos));
}
