#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "positions.h"
#include "hiwonder.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , readSocket("../../simpledetector_cpp/iqr.socket")
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QList ports = QSerialPortInfo



    qspb_list = {ui->servo_1_spinBox, ui->servo_2_spinBox, ui->servo_3_spinBox,
                 ui->servo_4_spinBox, ui->servo_5_spinBox, ui->servo_6_spinBox};

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->comL->addItem(info.portName(),info.portName());
    }

    // Прячем QlineEdit-ы и QSlider-ы
//    for (int i =0; i<= DOF -1; i++)
//    {
//        qle_list[i]->setVisible(false);
//        slider_list[i]->setVisible(false);
//    }


    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    //QByteArray ba = target_name.toLocal8Bit();
    //g/const char *c_str = ba.data();
    //printf("Appname : %s", c_str);
    Robot = new HiWonder(); // Без этого будет "The program has unexpectedly finished", хотя в начале нговорила, что это ambiguous
    Robot->Log_File_Open(Log_File_Name);

    QString str = "The application \"";  str +=target_name; str += "\"";
    Robot->Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));

    connect( this, SIGNAL (Open_Port_Signal(QString)), Robot, SLOT(Open_Port_Slot(QString)));

    RMath = new Robo_Math();
    //Robo_Math
    connect( this, SIGNAL (Pass_XY_Signal(int, int)), RMath, SLOT(Pass_XY_Slot(int, int)));
    connect(RMath, SIGNAL (Return_EL_Signal(float)), this, SLOT(Return_EL_Slot(float)));
    connect( this, SIGNAL (FW_Kinemaic_Signal(int, int, int, int, int, int)), RMath, SLOT(FW_Kinemaic_Slot(int, int, int, int, int, int)));
    connect( RMath, SIGNAL(Return_FW_Kinematic_XYZ_Signal(int, int, int, float)), this, SLOT(Return_FW_Kinematic_XYZ_Slot(int, int, int, float)));

    connect(RMath, SIGNAL (Pass_String_Signal(QString)), this, SLOT(Pass_String_Slot(QString)));


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
    this->update_LineDits_from_position(sit_down_position);
    this->repaint();

    QByteArray dd = QByteArray::fromRawData(sit_down_position, 6);
    dd.append(0x31); // Движение "Туда"
    Robot->GoToPosition(dd);//, sit_down_position
   // update_LineDits_from_servos();
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
    this->update_LineDits_from_position(hwr_Start_position);
    this->repaint();
    QByteArray dd = QByteArray::fromRawData(hwr_Start_position, 6);
   // dd.resize(7);
    dd.append(0x30); // Движение "Обратно"
    Robot->GoToPosition(dd);//, hwr_Start_position
    dd = QByteArray(reinterpret_cast<const char*>(hwr_Start_position), 10);
    QString str = QString(dd);

    //QByteArray cc;
    //cc.replace(hwr_Start_position, dd);
    this->update_Servos_from_LineEdits();
    str = "Data : ";
    for (int i =0; i<= szData -1; i++)
    {
       // str += QString::number(Servos[i]);

        str += QString::number(dd.at(i));
        str += ", ";
    }


    //str += QString(cc);
    str += " From standup";



    GUI_Write_To_Log(0xff10, str);
}
//+++++++++++++++++++++++++++++++
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

//+++++++++++++++++++++++++++++++++++++++++
//Send data from linedits to robot
void MainWindow::on_set_posButton_clicked()
{
    QString str;
    DetectorState state;
    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){
            //try_mcinfer(state.objectX, state.objectY);
            X = state.objectX;
            Y = state.objectY;
            str+="DETECTED: ";
            str += QString::number(state.objectX);
            str += ", ";
            str += QString::number(state.objectY);


        } else {
            str += "NOT DETECTED";
        }
      }//if (readSocket.GetState(&state) == 0)
    // const char *   pchar;
    //pchar = static_cast<const char *>(static_cast<char *>(Servos));
   // QByteArray dd = QByteArray::fromRawData(pchar, 6);
    GUI_Write_To_Log(0xf003,str);
    QByteArray dd ;
    dd.resize(64);
    memcpy(dd.data(), Servos, 6);
    dd.insert(6, 0x31);
    //dd.append(0x31); // Движение "Туда"
    //dd.resize(64);
    //QByteArray dd = QByteArray::fromRawData(Servos, 6);
    Robot->GoToPosition(dd);//, pchar
    //########### данные пользователя
    str = "Training data : ";
    str += QString::number(X);str+= ", ";
    str += QString::number(Y);str+= ", ";
unsigned char* sData = reinterpret_cast<unsigned char*>(Servos);
sData[szData-1] = dd.at(szData-1);
for (int i=0; i<= szData - 1; i++){
    str += QString::number(sData[i]);
    str+= ", ";
}
//########## данные серво
GUI_Write_To_Log(0xf003,str);

    str = "Servos data written to serial ";

    for (int i=0; i<= szData - 1; i++){
        str += QString::number(sData[i]);
        str+= ", ";
    }
    GUI_Write_To_Log(0xf003,str);



 //serial.write(reinterpret_cast<const char *>(Servos));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_socketButton_clicked()
{
    DetectorState state;
    QString str;
    str = "";
//Сразу открываем захват
//    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
//    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
    update_LineDits_from_servos();

    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){
            try_mcinfer(state.objectX, state.objectY);
            X = state.objectX;
            Y = state.objectY;
            str+="DETECTED: ";
            str += QString::number(state.objectX);
            str += ", ";
            str += QString::number(state.objectY);


        } else {
            str += "NOT DETECTED";
        }

       std::cout <<  str.toStdString() << std::endl;
       Robot->Write_To_Log(0xf010, str);
       GUI_Write_To_Log(0xf010, str);
    }
}
//+++++++++++++++++++++++++++++++++++++++  ->text().toInt()
void MainWindow::on_clampButton_clicked()
{
    if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
    else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}


    update_LineDits_from_servos();
    on_set_posButton_clicked();
}
//++++++++++++++++++++++++++++++++++++++
void MainWindow::update_LineDits_from_servos(void)
{
    for (int i =0; i<= DOF -1; i++)
    {
       // qle_list[i]->setText(QString::number(Servos[i]));
        qspb_list[i]->setValue((Servos[i]));
        GUI_Write_To_Log(0xf010, QString::number (Servos[i]));
    }
}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::update_LineDits_from_position(const char *pos)
{
    for (int i =0; i<= DOF -1; i++)
    {
        //qle_list[i]->setText(QString::number(pos[i]));
        qspb_list[i]->setValue(pos[i]);
    }

}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::update_Servos_from_LineEdits(void)
{
    for (int i =0; i<= DOF -1; i++)
    {
       // Servos[i] = qle_list[i]->text().toInt();
        Servos[i] = qspb_list[i]->value();
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_servo_1_spinBox_valueChanged(int arg1)
{
    Servos[0] = arg1;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_servo_2_spinBox_valueChanged(int arg1)
{
    Servos[1] = arg1;
}

void MainWindow::on_servo_3_spinBox_valueChanged(int arg1)
{
    Servos[2] = arg1;
}

void MainWindow::on_servo_4_spinBox_valueChanged(int arg1)
{
    Servos[3] = arg1;
}

void MainWindow::on_servo_5_spinBox_valueChanged(int arg1)
{
    Servos[4] = arg1;
}

void MainWindow::on_servo_6_spinBox_valueChanged(int arg1)
{
    Servos[5] = arg1;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Делаем запрос в simpledetecor, XY, вычисляем длину в мм L от основания робота до XY
void MainWindow::on_getXYButton_clicked()
{
    QString str;
//    DetectorState state;
//    if (readSocket.GetState(&state) == 0)
//      {
//        if (state.isDetected){
//            X = state.objectX;
//            Y = state.objectY;
//            str+="DETECTED: ";
//            str += QString::number(state.objectX);
//            str += ", ";
//            str += QString::number(state.objectY);


//        } else {
//            str += "NOT DETECTED";
//        }
//      }//if (readSocket.GetState(&state) == 0)
      GUI_Write_To_Log(0xf233, "Have got X,Y ");
      X = 500; Y=500;
      str += QString::number(X); str += ", ";
      str += QString::number(Y);
      GUI_Write_To_Log(0xf233, str);
      emit Pass_XY_Signal(X,Y);

      float dl1 = abs(qSin(115));
      str = "sin(115) = ";
      GUI_Write_To_Log(0xf233, str);
      str += QString::number(dl1);
      ui->All_Servos_lineEdit->setText(str);
      GUI_Write_To_Log(0xf233, "No go to Kinematic !");
      emit FW_Kinemaic_Signal(48, 25, 133, RMath->el1, RMath->el2, RMath->el3 ); //1190, 356

}
//++++++++++++++++++++++void Return_XY_Slot(float EL)
void MainWindow::Return_EL_Slot(float EL)
{
    QString str = "Distance value : ";
    str += QString::number(EL);
    GUI_Write_To_Log(0xf112, str);
}
//++++++++++++++++++++++++++++++++++
void MainWindow::Return_FW_Kinematic_XYZ_Slot(int X, int Y, int Z, float EL)
{                //But should be as follows :
    QString str = "Cube coordinates X, Y, L : ";
    str += QString::number(X); str += ", ";
    str += QString::number(Y); str += ", ";
    //str += QString::number(Z); str += ", ";
    str += QString::number(EL);
    GUI_Write_To_Log(0xf1122, str);
    str = "But should be as follows : ";
    str += "1190, 356, 230";// str += ", ";
    GUI_Write_To_Log(0xf1122, str);
}
//+++++++++++++++++++++++++++++++++++++++
void MainWindow::Pass_String_Slot(QString str)
{
    GUI_Write_To_Log(0xf114, str);
}
//+++++++++++++++++++++++++++++++++
//background-color: rgb(26, 148, 255);

void MainWindow::on_submitButton_clicked()
{
    int num;
    QString str;
    QString data = ui->All_Servos_lineEdit->text();
    QStringList list;

    str="The data from line edit are : ";
    str.append(data);
    GUI_Write_To_Log(0xf010, data);

    QRegExp rx("[, ]");// match a comma or a space
  //  QStringList list = {"100", "100", "100", "100", "100", "100" };
    list = data.split(rx, Qt::SkipEmptyParts);
    num = list.size (); //Число элементов, начиная с 1
    if (num < DOF) {
        str = "Данных НЕДОСТАТОЧНО !!! Передано всего "; str.append (QString::number(list.size()));
        //Если данных недостаточно, то меняем цвет кнопки и строки ввода, для подсказки
        GUI_Write_To_Log(0xf010, str);
        str = "background-color: rgb(26, 148, 255)";
        ui->submitButton->setStyleSheet (str);
        str = "background-color: rgb(255, 235, 11)";
        ui->All_Servos_lineEdit->setStyleSheet (str);

    }
    if (num == DOF)
      {
        // А теперь все это в qle_list
        for (int i =0; i<= DOF -1; i++)
        {
            //qle_list[i]->setText(QString::number(Servos[i]));
            qspb_list[i]->setValue(list.at(i).toInt());

        }
        // Если данные введены верно, то возвращаем цвета кнопки и строки ввода к значениям по умолчанию.
        ui->submitButton->setStyleSheet ("");
        ui->All_Servos_lineEdit->setStyleSheet ("");
        GUI_Write_To_Log(0xf010, "Данные переданы корректно");
     }//(num == DOF -1)

//    GUI_Write_To_Log(0xf010, str); //QString::number(list.size())
//    for (int i=0; i< list.size(); ++i ) {
//        GUI_Write_To_Log(0xf010, list.at(i).toLocal8Bit().constData());
//    }


this->repaint();
}
