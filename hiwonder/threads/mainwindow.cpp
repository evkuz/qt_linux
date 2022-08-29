#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "positions.h"
#include "hiwonder.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <QtGui>
#include <chrono>
#include <thread>

// /home/ubuntu/iqr_lit/hiwonder  ../simpledetector_cpp/iqr.socket


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , readSocket("/home/ubuntu/iqr_lit/hiwonder/iqr.socket")
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QList ports = QSerialPortInfo


    newYearMode = false;
    //HiWonder::DOF = 8;
    DETECTED = false;
    new_get_request = false;
    thread_counter = 0;
    qspb_list = {ui->servo_1_spinBox, ui->servo_2_spinBox, ui->servo_3_spinBox,
                 ui->servo_4_spinBox, ui->servo_5_spinBox, ui->servo_6_spinBox};

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->comL->addItem(info.portName(),info.portName());
    }



    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    //QByteArray ba = target_name.toLocal8Bit();
    //const char *c_str = ba.data();
    //printf("Appname : %s", c_str);

    Robot = new HiWonder(); // Создаем объект класса HiWonder

    Robot->Log_File_Open(Log_File_Name);
    Robot->Source_Points_File_Open (SOURCE_POINTS_FILE);

    QString str = "The application \"";  str +=target_name; str += "\"";
    Robot->Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));

    GUI_Write_To_Log(0xF000, "Going to Start QTcpServer");
    if (server.isListening ()) {

        str = "Listening on address "; str += server.serverAddress().toString();
        str += " and port "; str += QString::number(server.serverPort());//QString::number(server.tcpport);

        GUI_Write_To_Log(0000, str);
    }


    //+++++++++++++++++++++++++++++++++  signal/slot of Get Request to webserver
    connect(this, &MainWindow::Write_2_TcpClient_Signal, &server, &QSimpleServer::Write_2_TcpClient_Slot);
    connect(&server, SIGNAL(Data_From_TcpClient_Signal(QString)), this, SLOT(Data_From_TcpClient_Slot(QString)));

    //################### SERIAL SIGNAL/SLOTS ############################
    connect( this, SIGNAL (Open_Port_Signal(QString)), Robot, SLOT(Open_Port_Slot(QString)));
    connect( &Robot->serial, SIGNAL (readyRead()), Robot, SLOT(ReadFromSerial_Slot()));  //&QSerialPort::

    RMath = new Robo_Math();
    //################### Robo_Math SIGNAL/SLOTS #########################
    connect( this, SIGNAL (Pass_XY_Signal(int, int)), RMath, SLOT(Pass_XY_Slot(int, int)));
    connect( RMath, SIGNAL (Return_EL_Signal(float)), this, SLOT(Return_EL_Slot(float)));
    connect( this, SIGNAL (FW_Kinemaic_Signal(int, int, int, int, int, int)), RMath, SLOT(FW_Kinemaic_Slot(int, int, int, int, int, int)));
    connect( RMath, SIGNAL(Return_FW_Kinematic_XYZ_Signal(int, int, int, float)), this, SLOT(Return_FW_Kinematic_XYZ_Slot(int, int, int, float)));

    connect(RMath, SIGNAL (Pass_String_Signal(QString)), this, SLOT(Pass_String_Slot(QString)));
    //#################### Signal to web-server
    connect( Robot, SIGNAL (Moving_Done_Signal()), this, SLOT (Moving_Done_Slot()));


    // ============================================== Создаем поток 1 - web-server
//        thread_A = new QThread;
//        chan_A = new MyThread();

//        connect(thread_A, SIGNAL(started()), chan_A, SLOT(A_SLOT()) );
//        connect(chan_A, SIGNAL(finished()), thread_A, SLOT(quit()));
//        connect(chan_A, SIGNAL(Process_A()), TheWeb, SLOT(Output_Data_From_Client_Slot()), Qt::QueuedConnection); //Считываем из железки в ПК по каналу А
                                                                       // Qt::DirectConnection
                                                                       // Qt::QueuedConnection
                                                                       // Qt::BlockingQueuedConnection
                                                                       // Qt::DirectConnection
/*
 * "2) DirectConnection - The slot is invoked immediately, when the signal is emitted. The slot is executed in the emitter's thread,
 *     which is not necessarily the receiver's thread.
 *
    3) QueuedConnection - The slot is invoked when control returns to the event loop of the receiver's thread.
       The slot is executed in the receiver's thread.

    4) BlockingQueuedConnection - The slot is invoked as for the Queued Connection,
       except the current thread blocks until the slot returns. Note: Using this type to connect objects in the same thread will cause deadlock."
*/
//        chan_A->moveToThread(thread_A);
//        chan_A->finthread = false;
//        chan_A->pause_thread = false;




    //+++++++++++++++ ОТкрываем порт Open_Port_Signal(QString portname); ttyUSB0
    //emit Open_Port_Signal("ttyUSB0");

    Robot->Open_Port_Slot("ttyUSB0");
    //make_json_answer();




}
//++++++++++++++++++++++++++++++++++++++++++++++
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
    dd.append(LASTONE); // Всегда последнее ?
    Robot->GoToPosition(dd);//, sit_down_position

//    unsigned char test1[] = {23, 23, 23, 20, 23, 160};
//    unsigned char test2[] = {123, 123, 123, 120, 123, 23};
//    unsigned char test3[] = {93, 93, 93, 20, 93, 160};
//    unsigned char test4[] = {93, 93, 93, 20, 93, 23};
//    unsigned char test5[] = {93, 93, 93, 20, 93, 160};
//    unsigned char test6[] = {93, 93, 93, 20, 93, 23};

//    unsigned char rtest1[] = {93, 93, 33, 48, 93, 16};




//    unsigned char pos1[] = {93, 93, 93, 48, 48, 93};
//    unsigned char pos2[] = {0, 93, 25, 45, 93, 165};
//    unsigned char pos3[] = {63, 93, 25, 45, 93, 165};
//    unsigned char pos4[] = {63, 93, 65, 45, 93, 165};
    //unsigned char pos5[] = {63, 93, 65, 45, 135, 40};

//    QByteArray dd ;
//    dd.resize(HiWonder::szData);
//    dd.insert(HiWonder::szData-2, 0x31);
//    dd.insert(HiWonder::szData-1, LASTONE);

//    memcpy(dd.data(), test1, 6);
//    Robot->GoToPosition(dd);

//    memcpy(dd.data(), test2, 6);
//    Robot->GoToPosition(dd);

//    memcpy(dd.data(), test3, 6);
//    Robot->GoToPosition(dd);

//    memcpy(dd.data(), test4, 6);
//    Robot->GoToPosition(dd);

//    memcpy(dd.data(), test5, 6);
//    Robot->GoToPosition(dd);

//    memcpy(dd.data(), test6, 6);
//    Robot->GoToPosition(dd);


//    memcpy(dd.data(), pos1, 6);
//    Robot->GoToPosition(dd);

//    memcpy(dd.data(), pos2, 6);
//    Robot->GoToPosition(dd);

//    memcpy(dd.data(), pos3, 6);
//    Robot->GoToPosition(dd);

//    memcpy(dd.data(), pos4, 6);
//    Robot->GoToPosition(dd);

//    memcpy(dd.data(), pos5, 6);
//    Robot->GoToPosition(dd);

    //dd.insert(HiWonder::DOF-2, 0x31); // Движение "Туда"
    //dd.insert(HiWonder::DOF-1, LASTONE);

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
    dd.append(0x30); // Движение "Обратно"
    dd.append(LASTONE);
    Robot->GoToPosition(dd);//, hwr_Start_position

//    GUI_Write_To_Log(0xff10, str);
}
//+++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_closeButton_clicked()
{
    Robot->serial.close();
}
//+++++++++++++++++++++++++ update servos from LineEdits

//+++++++++++++++++++++++++++++++++++++++++
//Send data from GUI to robot
// Раз нажали кнопку, значит это единственная команда, значит добавляем LASTONE
void MainWindow::on_set_posButton_clicked()
{
    //QString str;


    // const char *   pchar;
    //pchar = static_cast<const char *>(static_cast<char *>(Servos));
   // QByteArray dd = QByteArray::fromRawData(pchar, 6);
    //GUI_Write_To_Log(0xf003,str);
    QByteArray dd ;
    dd.resize(HiWonder::szData);
    memcpy(dd.data(), Servos, 6);
    dd.insert(HiWonder::szData-2, 0x31); // Движение "Туда"
    dd.insert(HiWonder::szData-1, LASTONE);
    //dd.append(0x31);
    //dd.resize(64);
    //QByteArray dd = QByteArray::fromRawData(Servos, 6);
    Robot->GoToPosition(dd);//, pchar
    //########### данные пользователя
//    str = "Training data : ";
//    str += QString::number(X);str+= ", ";
//    str += QString::number(Y);str+= ", ";
// unsigned char* sData = reinterpret_cast<unsigned char*>(Servos);
//sData[szData-1] = dd.at(szData-1);
//for (int i=0; i<= szData - 1; i++){
//    str += QString::number(sData[i]);
//    str+= ", ";
//}
////########## данные серво

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
    Servos[0]=0;
    update_LineDits_from_servos();

    int kf = 100000;
    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){
            try_mcinfer(state.objectX*kf, state.objectY*kf);
            X = state.objectX;
            Y = state.objectY;
            str+="DETECTED: ";
            str += QString::number(state.objectX*kf);
            str += ", ";
            str += QString::number(state.objectY*kf);


        } else {
            str += "NOT DETECTED";
        }

       std::cout <<  str.toStdString() << std::endl;
       Robot->Write_To_Log(0xf014, str);
       GUI_Write_To_Log(0xf014, str);
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
    for (int i =0; i< Robot->DOF; i++)
    {
       // qle_list[i]->setText(QString::number(Servos[i]));
        qspb_list[i]->setValue((Servos[i]));
       // GUI_Write_To_Log(0xf010, QString::number (Servos[i]));
    }
}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::update_LineDits_from_position(const char *pos)
{
    for (int i =0; i< Robot->DOF; i++)
    {
        //qle_list[i]->setText(QString::number(pos[i]));
        qspb_list[i]->setValue(pos[i]);
    }

}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::update_LineDits_from_position(unsigned char *pos)
{
    for (int i =0; i< Robot->DOF; i++)
    {
        //qle_list[i]->setText(QString::number(pos[i]));
        qspb_list[i]->setValue(pos[i]);
    }

}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::update_Servos_from_LineEdits(void)
{
    for (int i =0; i< Robot->DOF; i++)
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


    /*
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
*/

   // str = "sdklfjlk";
    make_json_answer();

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
    str += QString::number(Z); str += ", ";
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
    int num, value;
    QString str;
    QString data = ui->All_Servos_lineEdit->text();
    QStringList list;

    value = 0xf010;
    str="The data from line edit are : ";
    str.append(data);
    GUI_Write_To_Log(value, data);

    QRegExp rx("[, ]");// match a comma or a space
  //  QStringList list = {"100", "100", "100", "100", "100", "100" };
    list = data.split(rx, Qt::SkipEmptyParts);
    num = list.size (); //Число элементов, начиная с 1
    if (num < Robot->DOF) {
        str = "Данных НЕДОСТАТОЧНО !!! Передано всего "; str.append (QString::number(list.size()));
        //Если данных недостаточно, то меняем цвет кнопки и строки ввода, для подсказки
        GUI_Write_To_Log(0xf010, str);
        str = "background-color: rgb(26, 148, 255)";
        ui->submitButton->setStyleSheet (str);

        str = "background-color: rgb(255, 235, 11)";
        ui->All_Servos_lineEdit->setStyleSheet (str);

    }
    if (num == Robot->DOF)
      {
        // А теперь все это в qle_list
        for (int i =0; i< Robot->DOF; i++)
        {
            //qle_list[i]->setText(QString::number(Servos[i]));
            qspb_list[i]->setValue(list.at(i).toInt());

        }
        // Если данные введены верно, то возвращаем цвета кнопки и строки ввода к значениям по умолчанию.
        ui->submitButton->setStyleSheet ("");
        ui->All_Servos_lineEdit->setStyleSheet ("");
        GUI_Write_To_Log(value, "Данные переданы корректно");
     }//(num == DOF -1)

//    GUI_Write_To_Log(0xf010, str); //QString::number(list.size())
//    for (int i=0; i< list.size(); ++i ) {
//        GUI_Write_To_Log(0xf010, list.at(i).toLocal8Bit().constData());
//    }


this->repaint();
}
//++++++++++++++++++++++++++
void MainWindow::on_trainButton_clicked()
{

    DetectorState state;
    QString str;
    str = "";
//Сразу открываем захват
//    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
//    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
    Servos[0]=0;
  //  update_LineDits_from_servos();

    int kf = 100000;
    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){
            try_mcinfer(state.objectX*kf, state.objectY*kf); // Тут меняем current_status = "inprogress". Команда 0 - Переместить открытый хват к кубику.
            X = state.objectX;                        //  Хват открывается в процессе движения робота, а не отдельной командой.
            Y = state.objectY;
            str+="DETECTED: ";
            str += QString::number(state.objectX*kf);
            str += ", ";
            str += QString::number(state.objectY*kf);
            DETECTED = true;

        } else {
            str += "NOT DETECTED";
        }

       std::cout <<  str.toStdString() << std::endl;
       Robot->Write_To_Log(0xf014, str);
       GUI_Write_To_Log(0xf014, str);
    }

//В этой точке робот опустил захват, открыл захват.

if (DETECTED)
    {

    QByteArray dd ;
    dd.resize(HiWonder::szData);
   str = "Next movement to robot";
   this->GUI_Write_To_Log (0xF055, str);
   //+++++++++++++++++ 1 make clamp
   //on_clampButton_clicked();
   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
   this->send_Data(NOT_LAST);
   //++++++++++++++++++++ 2 make stand up
   //on_stand_upButton_clicked();
   this->update_LineDits_from_position(hwr_Start_position);
   this->repaint();
   this->update_Servos_from_LineEdits();
   this->send_Data(NOT_LAST);

   //+++++++++++++++++++++ 3 put the cube
   // {60, 93, 100, 35, 145, 35};
   this->update_LineDits_from_position (put_position);
   this->repaint();
   update_Servos_from_LineEdits ();
   memcpy(dd.data(), Servos, 6);
   dd.insert(6, 0x31); // Движение "Туда"
   this->send_Data(BEFORE_LAST); //0xE9, NOT_LAST ==C8
   //+++++++++++++++++++++ 4.1  Unclamp the gripper
   //on_clampButton_clicked();
   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++


   //+++++++++++++++++++++ 6 go back to start position
   //on_stand_upButton_clicked();
   this->update_LineDits_from_position(hwr_Start_position);
   this->repaint();
   this->update_Servos_from_LineEdits();
   memcpy(dd.data(), Servos, 6);
   dd.insert(HiWonder::szData-2, 0x30); // Движение "Обратно"
   dd.insert(HiWonder::szData-1, LASTONE);

   // this->ssend_Data(dd); // The last command
   Robot->GoToPosition(dd);

//   //+++++++++++++++++++++ 3 put the cube
//   this->update_LineDits_from_position (put_position);
//   this->repaint();
//   update_Servos_from_LineEdits ();
//   memcpy(dd.data(), Servos, 6);
//   dd.insert(6, 0x31); // Движение "Туда"
//   this->send_Data(NOT_LAST);
//   //+++++++++++++++++++++ 4  Unclamp the gripper
//   //on_clampButton_clicked();
//   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
//   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
//   this->send_Data(NOT_LAST);
//   //+++++++++++++++++++++++++++++++++ 5 Приподнять хват, чтобы не задеть тележку.
//   this->update_LineDits_from_position (after_put_position);
//   this->repaint();
//   update_Servos_from_LineEdits ();
//   memcpy(dd.data(), Servos, 6);
//   dd.insert(6, 0x31); // Движение "Туда"
//   this->send_Data(NOT_LAST);

//   //+++++++++++++++++++++ 6 go back to start position
//   //on_stand_upButton_clicked();
//   this->update_LineDits_from_position(hwr_Start_position);
//   this->repaint();
//   this->update_Servos_from_LineEdits();
//   this->send_Data(LASTONE); // The last command

  }// if (DETECTED)

   DETECTED = false;

}
//++++++++++++++++++++++++++++++++++++
// Тут массив Servos преобразуется в массив QByteArray, с добавлением 2 с лужебных байт
void MainWindow::send_Data(unsigned char thelast)
{
   // QString str;


    QByteArray dd ;
    dd.resize(HiWonder::szData);
    memcpy(dd.data(), Servos, Robot->DOF);
    if (newYearMode) {dd.insert(HiWonder::szData-2, NEWYEAR_MV);} // Режим "НГ"
    else {dd.insert(HiWonder::szData-2, FORWARD_MV);} // Движение "Туда"

    dd.insert(HiWonder::szData-1, thelast);
    //dd.append(0x31);
    //dd.resize(64);
    //QByteArray dd = QByteArray::fromRawData(Servos, 6);
    Robot->GoToPosition(dd);
}

//+++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++
// подготовка json-строки с полями ответа в TCP сокет.
void MainWindow::make_json_answer()
{
  int value = 0x1111;
  QString jsn_str = "{";
  jsn_str += "\r\n";
  jsn_str += "\"status\":";
  jsn_str += "\"";
  jsn_str += Robot->current_status;
  jsn_str += "\","; jsn_str += "\r\n";
  jsn_str += "\"return_code\":";
  jsn_str += "\"";
  jsn_str +="0";
  jsn_str += "\","; jsn_str += "\r\n";
  jsn_str += "\"active_command\":";
  jsn_str += "\"";
  jsn_str +="TheCommand";

  jsn_str += "\","; jsn_str += "\r\n";
  jsn_str += "\"comment\":";
  jsn_str += "\"";
  jsn_str +="Some helpful data";
  jsn_str += "\"";
  jsn_str += "\r\n";
  jsn_str += "}";

  QString str = "Строка ответа в формате json :\n";
  str += jsn_str;
  GUI_Write_To_Log(value, str);

  // Строку подготовили, отправляем в TcpServer
 this->rAnswer = jsn_str;
 }

//++++++++++++++++++++++++++
// Пришел запрос от вебсервера. Весь запрос в message
void MainWindow::Data_From_Web_SLot(QString message)
{
 GUI_Write_To_Log(0xf00E, message);
 new_get_request = true;
 thread_counter +=1;
 this->ui->threadlabel->setText(QString::number(thread_counter));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++ Получили запрос от клиента. Парсим его.
void MainWindow::Data_From_TcpClient_Slot(QString message)
{
    QString str, substr;
    int value = 0xf00f;
    new_get_request = true;
    str = "Get new command : "; str += message;
    GUI_Write_To_Log(0xf00f, str);

        substr = message;

        // changed by Miksarus
        if (substr == "start") {
            //on_clampButton_clicked ();
            Robot->SetCurrentStatus ("init");
            //emit StartTakeAndPutSignal();

            // Движение только начинаем, поэтому обнулим значение LASTONE
            emit on_trainButton_clicked ();
            str = Robot->current_status;
            //str = "status_from_robot";
            emit Write_2_TcpClient_Signal (str);
         }

        if (substr == "reset") {
            if (Robot->GetCurrentStatus () != "wait"){
                Robot->SetCurrentStatus ("wait");
                str = "Robot changed status, now it is : ";
                str += Robot->current_status;

                GUI_Write_To_Log (value, str);
                str = Robot->current_status;
                //str = "status_from_robot";
                emit Write_2_TcpClient_Signal (str);
            }
         }


   if (substr == "status") {
       str = Robot->current_status;
       emit Write_2_TcpClient_Signal (str);
   }
//++++++++++++++++++ Если команда длинная, а для распознавания
// достаточно первые несколько символов, то выделяем по ним

   if (substr.startsWith("setservos=")){
       substr = substr.remove("setservos=");
       QStringList list1 = substr.split(QLatin1Char(','));
       for (int i=0; i<Robot->DOF; ++i)
       {
           Servos[i] = list1.at(i).toUInt();
       }//for
           this->send_Data(NOT_LAST);
   }
}

//++++++++++++++++++++++++++++++++++
// added by Miksarus
void MainWindow::TakeAndPutSlot()
{
    DetectorState state;
    QString str;
    str = "";

    Servos[0]=0;
    update_LineDits_from_servos();

    bool isDetected = false;

    while (!isDetected) {
        if (readSocket.GetState(&state) == 0)
        {
            isDetected = state.isDetected;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    try_mcinfer(state.objectX, state.objectY);

    str+="DETECTED: ";
    str += QString::number(state.objectX);
    str += ", ";
    str += QString::number(state.objectY);

    std::cout <<  str.toStdString() << std::endl;
    Robot->Write_To_Log(0xf014, str);
    GUI_Write_To_Log(0xf014, str);

    //В этой точке робот опустил захват, открыл захват.

    QByteArray dd ;
    dd.resize(HiWonder::szData);
    str = "Next movement to robot";
    this->GUI_Write_To_Log (0xF055, str);
   //+++++++++++++++++ make clamp
   //on_clampButton_clicked();
   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
   this->send_Data(NOT_LAST);
   //++++++++++++++++++++ make stand up
   //on_stand_upButton_clicked();
   this->update_LineDits_from_position(hwr_Start_position);
   this->repaint();
   this->update_Servos_from_LineEdits();
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++ put the cube
   this->update_LineDits_from_position (put_position);
   this->repaint();
   update_Servos_from_LineEdits ();
   memcpy(dd.data(), Servos, 6);
   dd.insert(6, 0x31); // Движение "Туда"
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++ Unclamp the gripper
   //on_clampButton_clicked();
   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++++++++++++++
   this->update_LineDits_from_position (after_put_position);
   this->repaint();
   //update_Servos_from_LineEdits ();
   //memcpy(dd.data(), Servos, 6);
   //dd.insert(6, 0x31); // Движение "Туда"
   //this->send_Data(NOT_LAST);

   //+++++++++++++++++++++ go back to start position
   //on_stand_upButton_clicked();
   this->update_LineDits_from_position(hwr_Start_position);
   this->repaint();
   this->update_Servos_from_LineEdits();
   this->send_Data(LASTONE); // The last command
}

void MainWindow::newConnection_Slot()
{
    GUI_Write_To_Log(0xf01f, "There is new TCP connection !!!");
}
//++++++++++++++++++++++++++
// Пишем в лог сообщение, что комплексная команда (например,взять кубик в одной точке и положить в другую)
// А также это индикатор, что команда выполнена и можно, например, отправить эти данные вебсерверу.
void MainWindow::Moving_Done_Slot()
{
    GUI_Write_To_Log(0xFAAA, "Demo cycle finished !!!");
    // Меняем статус, теперь "done"
    std::cout<<"Set DONE to Robot!" << std::endl;
    Robot->SetCurrentStatus ("done");
    if (new_get_request) // Тогда даем сигнал серверу на отправку данных клиенту. Данные уже в буфере TheWeb->status_buffer
    {
     // emit Write_2_TcpClient_Signal(Robot->current_status);
      new_get_request = false;
    }

}
//++++++++++++++++++++++++++


//+++++++++++++++++
void MainWindow::server_New_Connect_Slot()
{
    ;
}

//+++++++++++++++++
// отладка процесса возврата после поклажи кубика на транспортировщика
void MainWindow::on_getBackButton_clicked()
{
    QByteArray dd ;
    dd.resize(HiWonder::szData);

    //+++++++++++++++++++++ 3 put the cube
    // {60, 93, 100, 35, 145, 35};
    this->update_LineDits_from_position (put_position);
    this->repaint();
    update_Servos_from_LineEdits ();
    memcpy(dd.data(), Servos, 6);
    dd.insert(6, 0x31); // Движение "Туда"
    this->send_Data(BEFORE_LAST); //0xE9, NOT_LAST ==C8
    //+++++++++++++++++++++ 4  Unclamp the gripper
    //on_clampButton_clicked();
    if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
    else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
    this->send_Data(NOT_LAST);

    //+++++++++++++++++++++ 6 go back to start position
    //on_stand_upButton_clicked();
    this->update_LineDits_from_position(hwr_Start_position);
    this->repaint();
    this->update_Servos_from_LineEdits();
    dd.insert(6, 0x30); // Движение "Обратно"
    this->send_Data(LASTONE); // The last command

}
//++++++++++++++++++++++++++++++++++++++++++++
// По нажатию кнопки "Fix data" происходит запись в файл обучающей выборки координаты точки с камеры CV,
// и все позиции приводов, соответствующие даннным координатам.
// Позиции приводов берутся из строки Line_Edit - "ui->All_Servos_lineEdit", а не из массива Servos
// Так намного быстрее протекает набор точек.
void MainWindow::on_fixButton_clicked()
{
    int value = 0x4444;
    DetectorState state;
    QString str = "";

    Servos[0] = 0;
    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){
           // try_mcinfer(state.objectX, state.objectY); // Тут меняем current_status = "inprogress". Команда 0 - Переместить открытый хват к кубику.
            X = state.objectX;                        //  Хват открывается в процессе движения робота, а не отдельной командой.
            Y = state.objectY;

            str += QString::number(state.objectX);
            str += ", ";
            str += QString::number(state.objectY);
            str += ", ";
            DETECTED = true;

        } else {
            str += "NOT DETECTED";
        }

       str += ui->All_Servos_lineEdit->text();
       //std::cout <<  str.toStdString() << std::endl;
       //Robot->Write_To_Log(0xf014, str);
       GUI_Write_To_Log(value, str);
       Robot->Write_To_Source (str);
    }




}// on_fixButton_clicked()
//+++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_PUTButton_clicked()
{
    QByteArray dd;
    //+++++++++++++++++++++ 3 put the cube, NW_1st
    // {60, 93, 100, 35, 145, 35};
    //this->update_LineDits_from_position (put_position);
    //this->update_LineDits_from_position(nw_1st_position);
    this->update_LineDits_from_position(mv_01_tree_position);   this->repaint();
    update_Servos_from_LineEdits ();
    memcpy(dd.data(), Servos, 6);
    //dd.insert(6, 0x31); // Движение "Туда"
    dd.insert(6, FORWARD_MV); // Движение в режиме "НГ"
    this->send_Data(LASTONE); //0xE9, NOT_LAST ==C8
    //+++++++++++++++++++++ 4  Unclamp the gripper
    //on_clampButton_clicked();
//    if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
//    else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
//    this->send_Data(NOT_LAST);

    //+++++++++++++++++++++ 6 go back to start position
    //on_stand_upButton_clicked();
    //this->update_LineDits_from_position(hwr_Start_position);
    this->update_LineDits_from_position(mv_02_tree_position);
    this->repaint();
    this->update_Servos_from_LineEdits();
    //dd.insert(6, 0x30); // Движение "Обратно"
    dd.insert(6, FORWARD_MV); // Движение в режиме "НГ"
    //this->send_Data(LASTONE); // The last command
    this->send_Data(LASTONE);
    //+++++++++++++++++++++++++++++++++++++++++++++++
    this->update_LineDits_from_position(mv_03_tree_position);  this->repaint(); this->update_Servos_from_LineEdits();
    dd.insert(6, FORWARD_MV); // Движение в режиме "НГ"
    this->send_Data(LASTONE); // The last command
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    this->update_LineDits_from_position(mv_04_tree_position);  this->repaint(); this->update_Servos_from_LineEdits();
    dd.insert(6, FORWARD_MV); // Движение в режиме "НГ"
    this->send_Data(LASTONE); // The last command
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    this->update_LineDits_from_position(mv_05_tree_position);  this->repaint(); this->update_Servos_from_LineEdits();
    dd.insert(6, FORWARD_MV); // Движение в режиме "НГ"
    this->send_Data(LASTONE); // The last command
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    this->update_LineDits_from_position(hwr_Start_position);  this->repaint(); this->update_Servos_from_LineEdits();
    dd.insert(6, FORWARD_MV); // Движение в режиме "НГ"
    this->send_Data(LASTONE); // The last command


}
//++++++++++++++++++++++++++++++++++++++++++++
// Данные обратно из qspinboxes to LineEdit
void MainWindow::on_GetBackFromServoButton_clicked()
{
   QString str = "";
    for (int i =0; i< Robot->DOF; i++)
    {
        str += QString::number((qspb_list[i]->value()));
        str += ", ";
    }
    str.truncate(str.lastIndexOf(","));
    GUI_Write_To_Log(0xf016, str);
    ui->All_Servos_lineEdit->setText(str);
}
//+++++++++++++++++++++++++++++++++++++++++++
//Выполняет последовательность позиций из файла
void MainWindow::on_fromFileButton_clicked()
{// Открываем файл, читаем построчно, копируем в массив Servos, отправляем в SerialPort
    QString str;
    int value = 0xCACA;

    QByteArray dd ;
    dd.resize(Robot->DOF);

    GUI_Write_To_Log(value, "Before opening command file");
    Robot->Command_List_File_Open(COMMAND_LIST_FILE);
    GUI_Write_To_Log(value, "After opening command file");

    QTextStream in(&Robot->CommandFile);
    QString line;
    QStringList list1;
    while (!in.atEnd())
    {
       line = in.readLine();
       GUI_Write_To_Log(value, line);
       list1 = line.split(QLatin1Char(','));
//       str = "РАзмер списка : "; str += QString::number(list1.size());
//       GUI_Write_To_Log(value, str);

       for (int i=0; i < list1.size(); i++)
       {GUI_Write_To_Log(value, list1.at(i).toLocal8Bit().constData()); }

//       GUI_Write_To_Log(value, "Now start insert to qbyteArray");
       for (int i=0; i< Robot->szData; ++i)
       {
         //Servos[i] = list1.at(i).toUInt();
         dd.insert(i, list1.at(i).toStdString().c_str());  //convert a QString to a const char *
       }//for
       //this->send_Data(NOT_LAST);
       // Записали значения углов. Еще 2 байта надо записать
       dd.insert(Robot->DOF -2, FORWARD_MV); // Движение "Туда"
       dd.insert(Robot->DOF -1, LASTONE);

       //memcpy(dd.data(), Servos, DOF);
       //Robot->GoToPosition(dd);
           GUI_Write_To_Log(value, "There are following data in command file ");
           GUI_Write_To_Log(value, line);


    }//while (!in.atEnd())


//    str = Robot->CommandFile.readLine();
//    GUI_Write_To_Log(value, "There are following data in command file ");
//    GUI_Write_To_Log(value, str);



    Robot->CommandFile.close();
}
