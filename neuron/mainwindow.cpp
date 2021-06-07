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
    Robot->GoToPosition(dd, sit_down_position);
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
    Robot->GoToPosition(dd, hwr_Start_position);
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
//+++++++++++++++++++++++++++++++
void MainWindow::on_S1_verSlider_valueChanged(int value)
{
    update_data_from_sliders(0, value);
    ui->servo_1_lineEdit->setText(QString::number(value));
}
//++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::update_data_from_sliders(int index, int value)
{
    Servos[index]   = value;
    on_set_posButton_clicked();
//    const char *   pchar;
//    pchar = static_cast<const char *>(static_cast<char *>(Servos));
//    QByteArray dd = QByteArray::fromRawData(pchar, 6);
//    QString str = "Servos before write to serial ";
//    for (int i=0; i<= DOF - 1; i++){
//        str += QString::number(Servos[i]);
//        str+= ", ";
//    }
//    GUI_Write_To_Log(0xf002,str);
//    Robot->GoToPosition(dd, pchar);


}
//++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_closeButton_clicked()
{
    Robot->serial.close();
}
//+++++++++++++++++++++++++ update servos from LineEdits
void MainWindow::on_servo_1_lineEdit_editingFinished()
{
    Servos[0] = ui->servo_1_lineEdit->text().toInt();
}


void MainWindow::on_servo_2_lineEdit_editingFinished()
{
    Servos[1] = ui->servo_2_lineEdit->text().toInt();
}


void MainWindow::on_servo_3_lineEdit_editingFinished()
{
    Servos[2] = ui->servo_3_lineEdit->text().toInt();
}

void MainWindow::on_servo_4_lineEdit_editingFinished()
{
    Servos[3] = ui->servo_4_lineEdit->text().toInt();
}


void MainWindow::on_servo_5_lineEdit_editingFinished()
{
    Servos[4] = ui->servo_5_lineEdit->text().toInt();
}


void MainWindow::on_servo_6_lineEdit_editingFinished()
{
    Servos[5] = ui->servo_6_lineEdit->text().toInt();
}
//+++++++++++++++++++++++++++++++++++++++++
//Send data from linedits to robot
void MainWindow::on_set_posButton_clicked()
{
    QString str;
    const char *   pchar;
    pchar = static_cast<const char *>(static_cast<char *>(Servos));
    QByteArray dd = QByteArray::fromRawData(pchar, 6);

    Robot->GoToPosition(dd, pchar);
    str = "Servos data written to serial ";
    unsigned char* sData = reinterpret_cast<unsigned char*>(Servos);
    for (int i=0; i<= DOF - 1; i++){
        str += QString::number(sData[i]);
        str+= ", ";
    }
    GUI_Write_To_Log(0xf003,str);



 //serial.write(reinterpret_cast<const char *>(Servos));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_S2_verSlider_valueChanged(int value)
{
    update_data_from_sliders(1, value);
    ui->servo_2_lineEdit->setText(QString::number(value));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_S3_verSlider_valueChanged(int value)
{
    update_data_from_sliders(2, value);
    ui->servo_3_lineEdit->setText(QString::number(value));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++


void MainWindow::on_S4_verSlider_valueChanged(int value)
{
    update_data_from_sliders(3, value);
    ui->servo_4_lineEdit->setText(QString::number(value));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_S5_verSlider_valueChanged(int value)
{
    update_data_from_sliders(4, value);
    ui->servo_5_lineEdit->setText(QString::number(value));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_S6_verSlider_valueChanged(int value)
{
    update_data_from_sliders(5, value);
    ui->servo_6_lineEdit->setText(QString::number(value));
}
