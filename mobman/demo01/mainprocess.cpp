#include "mainprocess.h"
//#include "ui_MainProcess.h"
#include "positions.h"
#include "hiwonder.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//#include <QtGui>
#include <chrono>
#include <thread>
//#include "nlohmann/json.hpp"

//(QObject *parent)

using json = nlohmann::json;

MainProcess::MainProcess(QObject *parent)
    : QObject(parent)
//    , readSocket("../../simpledetector_cpp/iqr.socket")

{

    //json jsncommand; // Команду извне упакуем в json
    json jsnAnswer;  // ответ tcp-клменту в json

    DETECTED = false;
    new_get_request = false;
    thread_counter = 0;

    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    //QByteArray ba = target_name.toLocal8Bit();
    //g/const char *c_str = ba.data();
    //printf("Appname : %s", c_str);
    Robot = new HiWonder(); // Без этого будет "The program has unexpectedly finished", хотя в начале говорила, что это ambiguous

    Robot->Log_File_Open(Log_File_Name);
  //  Robot->Source_Points_File_Open (SOURCE_POINTS_FILE);

    QString str = "The application \"";  str +=target_name; str += "\"";
    Robot->Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));

    qDebug() << "Started " << target_name;

    GUI_Write_To_Log(0000, "Going to Start QTcpSErver");
    if (server.isListening ()) {

        str = "Listening on address "; str += server.serverAddress().toString();
        str += " and port "; str += QString::number(server.serverPort());//QString::number(server.tcpport);

        GUI_Write_To_Log(0000, str);
         qDebug() << str;
    }

    //+++++++++++++++++++++++++++++++++  signal/slot of Get Request to webserver
    // Отправка данных от сервера клиенту (в  ЦУП)
    connect(this, &MainProcess::Write_2_TcpClient_Signal, &server, &QSimpleServer::Write_2_TcpClient_SLot); // works ?

    // Чтение данных от клиента серверу (из ЦУП)
    //connect(&server, SIGNAL(Info_2_Log_Signal(QString)), this, SLOT(Info_2_Log_Slot(QString))); // Not working
    connect(&server, &QSimpleServer::Data_From_TcpClient_Signal, this, &MainProcess::Data_From_TcpClient_Slot);



    //connect(Robot, SIGNAL(StatusChangedSignal(QString)), &server, SLOT(SetCurrentState(QString)));
    //connect(this, SIGNAL(StartTakeAndPutSignal()), this, SLOT(TakeAndPutSlot()));

   // connect(&server, &QTcpServer::newConnection, this, &MainProcess::newConnection_Slot);
    //################### SERIAL SIGNAL/SLOTS ############################
    connect( this, &MainProcess::Open_Port_Signal, Robot, &HiWonder::Open_Port_Slot);
    connect( &Robot->serial, &QSerialPort::readyRead, Robot, &HiWonder::ReadFromSerial_Slot);  //&QSerialPort::

    //RMath = new Robo_Math();
    //################### Robo_Math SIGNAL/SLOTS #########################
//    connect( this, SIGNAL (Pass_XY_Signal(int, int)), RMath, SLOT(Pass_XY_Slot(int, int)));
//    connect( RMath, SIGNAL (Return_EL_Signal(float)), this, SLOT(Return_EL_Slot(float)));
//    connect( this, SIGNAL (FW_Kinemaic_Signal(int, int, int, int, int, int)), RMath, SLOT(FW_Kinemaic_Slot(int, int, int, int, int, int)));
//    connect( RMath, SIGNAL(Return_FW_Kinematic_XYZ_Signal(int, int, int, float)), this, SLOT(Return_FW_Kinematic_XYZ_Slot(int, int, int, float)));

//    connect(RMath, SIGNAL (Pass_String_Signal(QString)), this, SLOT(Pass_String_Slot(QString)));
    //#################### Signal to web-server
    connect( Robot, &HiWonder::Moving_Done_Signal, this, &MainProcess::Moving_Done_Slot);


    // ============================================== Создаем поток 1 - web-server
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




    //+++++++++++++++ ОТкрываем порт Open_Port_Signal(QString portname); ttyUSB0
    // Arduino NANO виден как ttyUSB0
    // Arduino Mega - как
    emit Open_Port_Signal("ttyACM0"); //"ttyUSB0"
    //make_json_answer();

    //+++++++++ Проверяем, что работает QSerialPort
//    QThread::sleep(1);
//    emit on_clampButton_clicked();
//    QThread::sleep(1);
//    emit on_clampButton_clicked();
//    QThread::sleep(1);
//    emit on_clampButton_clicked();

//    QString tcp_command="/run?cmd=servos=25,35,45,55,65,75,125,222&";
//    http://192.168.1.201:8383/run?cmd=servos=45,90,45,165,0,0,125,222&

//    QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(sit_down_position), 6);
//    dd.append(0x31); // Движение "Туда"
//    Robot->GoToPosition(dd);//, sit_down_position

//    str = "Current servo values are : ";


//    for (int i=0; i< DOF; i++){
//        str += QString::number(Servos[i]);
//        str += ", ";
//    }
// GUI_Write_To_Log(0000, str);
// qDebug() << str;

    str = "On start ";
    Servos_To_Log(str);
 QThread::sleep(1);


}
//++++++++++++++++++++++++++++++++++++++++++++++
MainProcess::~MainProcess()
{
    QString str = "Program is going to be closed";
    GUI_Write_To_Log(0xffff, str);
    qDebug() << str;
    delete this;
    delete Robot;


}
//+++++++++++++++++++++++++++++++++++++++
void MainProcess::GUI_Write_To_Log (int value, QString log_message)
{
    QDateTime curdate ;
    QTextStream uin(&Robot->LogFile);

    QString str, str2;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_message << "\n";


}

void MainProcess::Servos_To_Log(QString message)
{
 QString   str = message;
    str += "Current servo values are : ";


    for (int i=0; i< DOF; i++){
        str += QString::number(Servos[i]);
        str += ", ";
    }
 GUI_Write_To_Log(0000, str);

}


//+++++++++++++++++++++++++++++++++++++++
// open Serial port
void MainProcess::on_openButton_clicked()
{
QString portname = "Some Serial"; //ui->comL->currentText();
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
//+++++++++++++++++++++++++
// Go to "sit" position
void MainProcess::on_sitButton_clicked()
{
    QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(sit_down_position), 6);
    dd.append(0x31); // Движение "Туда"
    Robot->GoToPosition(dd);//, sit_down_position
}
//+++++++++++++++++++++++++++++++
// Go to Initial "Start" position
void MainProcess::on_stand_upButton_clicked()
{
  //  this->update_LineDits_from_position(hwr_Start_position);
    this->update_Servos_from_position(hwr_Start_position);
    //   this->repaint();
    QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(hwr_Start_position), 6);
    dd.append(0x30); // Движение "Обратно"
    dd.append(LASTONE);
    Robot->GoToPosition(dd);//, hwr_Start_position

//    GUI_Write_To_Log(0xff10, str);
}
//+++++++++++++++++++++++++++++++
void MainProcess::on_closeButton_clicked()
{
    Robot->serial.close();
}
//+++++++++++++++++++++++++++++++++++++++++
//Send data from GUI to robot
// Раз нажали кнопку, значит это единственная команда, значит добавляем LASTONE
void MainProcess::on_set_posButton_clicked()
{
    //QString str;


    // const char *   pchar;
    //pchar = static_cast<const char *>(static_cast<char *>(Servos));
   // QByteArray dd = QByteArray::fromRawData(pchar, 6);
    //GUI_Write_To_Log(0xf003,str);
    QByteArray dd ;
    dd.resize(parcel_size);
    memcpy(dd.data(), Servos, DOF);
    dd.insert(parcel_size-2, 0x31); // Движение "Туда"
    dd.insert(parcel_size-1, LASTONE);
    //dd.append(0x31);
    //dd.resize(64);
    //QByteArray dd = QByteArray::fromRawData(Servos, 6);
    Robot->GoToPosition(dd);//, pchar
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
void MainProcess::on_socketButton_clicked()
{
    DetectorState state;
    QString str;
    str = "";
//Сразу открываем захват
//    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
//    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
    Servos[0]=0;
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
       Robot->Write_To_Log(0xf014, str);
       GUI_Write_To_Log(0xf014, str);
    }
}
*/
//+++++++++++++++++++++++++++++++++++++++  ->text().toInt()
void MainProcess::on_clampButton_clicked()
{
    quint8 FULL_OPENED, FULL_CLOSED;
    FULL_CLOSED = 35;
    FULL_OPENED = 90;
    if (Servos[0]>FULL_CLOSED){ Servos[0]=FULL_CLOSED;}
    else {Servos[0]=FULL_OPENED;}
//    update_LineDits_from_servos();

    on_set_posButton_clicked();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Делаем запрос в simpledetecor, XY, вычисляем длину в мм L от основания робота до XY
void MainProcess::on_getXYButton_clicked()
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

    if (server.isListening ()) {

        str = "Listening on address "; str += server.serverAddress().toString();
        str += " and port "; str += QString::number(server.serverPort());//QString::number(server.tcpport);

        GUI_Write_To_Log(0000, str);
    }


}
//++++++++++++++++++++++void Return_XY_Slot(float EL)
void MainProcess::Return_EL_Slot(float EL)
{
    QString str = "Distance value : ";
    str += QString::number(EL);
    GUI_Write_To_Log(0xf112, str);
}
//++++++++++++++++++++++++++++++++++
void MainProcess::Return_FW_Kinematic_XYZ_Slot(int X, int Y, int Z, float EL)
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
void MainProcess::Pass_String_Slot(QString str)
{
    GUI_Write_To_Log(0xf114, str);
}
//+++++++++++++++++++++++++++++++++
//background-color: rgb(26, 148, 255);

//+++++++++++++++++++++++++++
void MainProcess::on_trainButton_clicked()
{
    for (int i =0; i<= DOF -1; i++)
        {
           Servos[i] = 45;
        }
    QByteArray dd ;
    dd.resize(parcel_size);
    memcpy(dd.data(), Servos, 6);
    dd.insert(6, 0x31); // Движение "Туда"
    Robot->GoToPosition(dd);

}
//++++++++++++++++++++++++++
// Обработка сигнала on_trainButton_clicked()
// Выполняем целый набор действий - взять кубик и положить на транспортировщик

//void MainProcess::on_trainButton_clicked()
//{
////    for (int i =0; i<= DOF -1; i++)
////    {
////       Servos[i] = train_position[i];
////    }
////    update_LineDits_from_servos ();


//    //DetectorState state;
//    QString str;
//    str = "";
////Сразу открываем захват
////    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
////    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
//    Servos[0]=0;
//  //  update_LineDits_from_servos();

//    if (readSocket.GetState(&state) == 0)
//      {
//        if (state.isDetected){
//            try_mcinfer(state.objectX, state.objectY); // Тут меняем current_status = "inprogress". Команда 0 - Переместить открытый хват к кубику.
//            X = state.objectX;                        //  Хват открывается в процессе движения робота, а не отдельной командой.
//            Y = state.objectY;
//            str+="DETECTED: ";
//            str += QString::number(state.objectX);
//            str += ", ";
//            str += QString::number(state.objectY);
//            DETECTED = true;

//        } else {
//            str += "NOT DETECTED";
//        }

//       std::cout <<  str.toStdString() << std::endl;
//       Robot->Write_To_Log(0xf014, str);
//       GUI_Write_To_Log(0xf014, str);
//    }

////В этой точке робот опустился над кубиком, открыл захват.

//if (DETECTED)
//    {

//    QByteArray dd ;
//    dd.resize(parcel_size);
////    memcpy(dd.data(), Servos, 6);
////    dd.insert(6, 0x31); // Движение "Туда"
////    Robot->GoToPosition(dd);
//////    while (!Robot->MOVEMENT_DONE) {;}
//   str = "Next movement to robot";
//   this->GUI_Write_To_Log (0xF055, str);
//   //+++++++++++++++++ 1 make clamp, хватаем кубик
//   //on_clampButton_clicked();
//   if (Servos[0] > 0){ Servos[0]=0;}
//   else {Servos[0]=90;}
//   this->send_Data(NOT_LAST);
//   //++++++++++++++++++++ 2 make stand up, встаем в исходную точку
//   //on_stand_upButton_clicked();
//   this->update_Servos_from_position(hwr_Start_position);
//   this->send_Data(NOT_LAST);

//   //+++++++++++++++++++++ 3 put the cube, наклоняем захват с кубиком к транспортировщику
//   // {60, 93, 100, 35, 145, 35};
//   this->update_Servos_from_position(put_position);
//   memcpy(dd.data(), Servos, 6);
//   dd.insert(6, 0x31); // Движение "Туда"
//   this->send_Data(BEFORE_LAST); //0xE9, NOT_LAST ==C8
//   //+++++++++++++++++++++ 4  Unclamp the gripper, открываем захват, т.е. кладем кубик на транспортировщик
//   //on_clampButton_clicked();
//   Servos[0]=0;
//   this->send_Data(NOT_LAST);
//   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//   //+++++++++++++++++++++++++++++++++ 5 Приподнять хват, чтобы не задеть тележку.
////      this->update_Servos_from_position(after_put_position);
////      memcpy(dd.data(), Servos, 6);
////      dd.insert(6, 0x30); // Движение "Туда"
////      this->send_Data(AFTER_PUT);



//   //+++++++++++++++++++++ 6 go back to start position
//   //on_stand_upButton_clicked();
//   this->update_Servos_from_position(hwr_Start_position);
////   dd.insert(6, 0x30); // Движение "Обратно"
//  // this->send_Data(LASTONE); // The last command
//   memcpy(dd.data(), Servos, 6);
//   dd.insert(parcel_size-2, 0x30); // Движение "Обратно"
//   dd.insert(parcel_size-1, LASTONE);

//   // this->ssend_Data(dd); // The last command
//   Robot->GoToPosition(dd);



//  }// if (DETECTED)

//   DETECTED = false;


//}//
//++++++++++++++++++++++++++++++++++++
void MainProcess::send_Data(unsigned char thelast)
{
   // QString str;


    QByteArray dd ;
    dd.resize(parcel_size);
    memcpy(dd.data(), Servos, DOF);
    dd.insert(parcel_size-2, 0x31); // Движение "Туда"
    dd.insert(parcel_size-1, thelast);
    //dd.append(0x31);
    //dd.resize(64);
    //QByteArray dd = QByteArray::fromRawData(Servos, 6);
    Robot->GoToPosition(dd);
}
//+++++++++++++++++++++++++++++++++
// подготовка json-строки с полями ответа в TCP сокет.
void MainProcess::make_json_answer()
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
void MainProcess::Data_From_Web_SLot(QString message)
{
 GUI_Write_To_Log(0xf00E, message);
 new_get_request = true;
 thread_counter +=1;
 //this->ui->threadlabel->setText(QString::number(thread_counter));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++ Получили данные (запрос) от клиента. Парсим.
void MainProcess::Data_From_TcpClient_Slot(QString message)
{
    QByteArray dd ;
    QString str, substr;
    int value = 0xf00f;
    new_get_request = true;
    //str = "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSimpleServer->Info_2_Log_Signal !!!!!!!!!!!!!!!!!!!";
    str = "From TCP Get new command : "; str += message;
    GUI_Write_To_Log(0xf00f, str);

        substr = message;

        // changed by Miksarus
        if (substr == "start") {
            //on_clampButton_clicked ();
            Robot->SetCurrentStatus ("init"); // Перед запуском распознавания
            //emit StartTakeAndPutSignal();

            // Движение только начинаем, поэтому обнулим значение LASTONE
            emit on_trainButton_clicked ();
            str = "Robot current status is ";
            str += Robot->current_status;
            Robot->Write_To_Log(0xf00F, str);

            str = Robot->current_status;
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
//         ///run?cmd=status&123


   if (substr == "status") {
      // str  = "{\n\t\"status\":\"";
       str = Robot->current_status;
     //  str += "\"\n}";

//       QDateTime dt(QDateTime::currentDateTime());
//       //dt.toLocalTime();
//       str = "Current SecsSinceEpoch is ";
//       str += QString::number(dt.toSecsSinceEpoch());
//       GUI_Write_To_Log (value, str);
       emit Write_2_TcpClient_Signal (str);
   }

   if (substr == "sit") {

       QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(sit_down_position), 6);
       dd.append(0x31); // Движение "Туда"
       Robot->GoToPosition(dd);//, sit_down_position
   }//"sit"

   if (substr == "standup") {
       QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(hwr_Start_position), 6);
       dd.append(0x30); // Движение "Обратно"
       dd.append(LASTONE);
       Robot->GoToPosition(dd);//, hwr_Start_position

   }

   if (substr == "clamp") { on_clampButton_clicked();}//"sit"

   if (substr == "parking")
   {
        str = "Before parking memcpy ";
        Servos_To_Log(str);
        memcpy(Servos, mob_parking_position, DOF);
        str = "After parking memcpy ";
        Servos_To_Log(str);
        this->send_Data(LASTONE);
   }


   if (substr == "servo2_20")
   {
       Servos[1]=20;
       QByteArray dd ;
       dd.resize(parcel_size);
       memcpy(dd.data(), Servos, DOF);
       dd.insert(6, 0x31); // Движение "Туда"
       Robot->GoToPosition(dd);

   }
   if (substr == "servo2_90")
   {
       Servos[1]=90;
       QByteArray dd ;
       dd.resize(parcel_size);
       memcpy(dd.data(), Servos, DOF);
       dd.insert(6, 0x31); // Движение "Туда"
       Robot->GoToPosition(dd);

   }

   if (substr == "pos_15") { memcpy(Servos, mob_pos_15, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_16") { memcpy(Servos, mob_pos_16, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_17") { memcpy(Servos, mob_pos_17, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_18") { memcpy(Servos, mob_pos_18, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_19") { memcpy(Servos, mob_pos_19, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_20") { memcpy(Servos, mob_pos_20, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_21") { memcpy(Servos, mob_pos_21, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_22") { memcpy(Servos, mob_pos_22, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_23") { memcpy(Servos, mob_pos_23, DOF);  this->send_Data(LASTONE); }


//++++++++++++++++++ Если команда длинная, а для распознавания
// достаточно первые несколько символов

   if (substr.startsWith("servos=")){
       substr = substr.remove("servos=");
       QStringList list1 = substr.split(QLatin1Char(','));
       for (int i=0; i<DOF; ++i)
       {
           Servos[i] = list1.at(i).toUInt();

//           dd.resize(parcel_size);
//           memcpy(dd.data(), Servos, DOF);
//           dd.insert(6, 0x31); // Движение "Туда"
//           Robot->GoToPosition(dd);

           // so now we have here servos array with actual values
       }//for

       this->send_Data(NOT_LAST);
   }

}



//+++++++++++++++++
void MainProcess::newConnection_Slot()
{
    GUI_Write_To_Log(0xf01f, "There is new TCP connection !!!");
}
//++++++++++++++++++++++++++
// Пишем в лог сообщение, что комплексная команда (например,взять кубик в одной точке и положить в другую)
// А также это индикатор, что команда выполнена и можно, например, отправить эти данные вебсерверу.
void MainProcess::Moving_Done_Slot()
{
    GUI_Write_To_Log(0xFAAA, "Demo cycle finished !!!");
    // Меняем статус, теперь "done"
    std::cout<<"Set DONE to Robot!" << std::endl;
    Robot->SetCurrentStatus ("done");
    if (new_get_request) // Тогда даем сигнал серверу на отправку данных клиенту. Данные уже в буфере TheWeb->status_buffer
    {
     // emit Write_2_Client_Signal(Robot->current_status);
      new_get_request = false;
    }

}
//++++++++++++++++++++++++++


//+++++++++++++++++
void MainProcess::server_New_Connect_Slot()
{
    ;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainProcess::update_Servos_from_position(unsigned char *pos)
{
    for (int i =0; i< DOF; i++) { Servos[i] = pos[i]; }

}


//void MainProcess::on_getBackButton_clicked()
//{
//    QByteArray dd ;
//    dd.resize(parcel_size);

//    //+++++++++++++++++++++ 3 put the cube
//    // {60, 93, 100, 35, 145, 35};
//    this->update_LineDits_from_position (put_position);
//    //   this->repaint();
//    update_Servos_from_LineEdits ();
//    memcpy(dd.data(), Servos, 6);
//    dd.insert(6, 0x31); // Движение "Туда"
//    this->send_Data(BEFORE_LAST); //0xE9, NOT_LAST ==C8
//    //+++++++++++++++++++++ 4  Unclamp the gripper
//    //on_clampButton_clicked();
//    if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
//    else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
//    this->send_Data(NOT_LAST);

//    //+++++++++++++++++++++ 6 go back to start position
//    //on_stand_upButton_clicked();
//    this->update_LineDits_from_position(hwr_Start_position);
//    //   this->repaint();
//    this->update_Servos_from_LineEdits();
//    dd.insert(6, 0x30); // Движение "Обратно"
//    this->send_Data(LASTONE); // The last command

//}

//void MainProcess::on_fixButton_clicked()
//{
//    int value = 0x4444;
//    DetectorState state;
//    QString str = "";

//    Servos[0] = 0;
//    if (readSocket.GetState(&state) == 0)
//      {
//        if (state.isDetected){
//            try_mcinfer(state.objectX, state.objectY); // Тут меняем current_status = "inprogress". Команда 0 - Переместить открытый хват к кубику.
//            X = state.objectX;                        //  Хват открывается в процессе движения робота, а не отдельной командой.
//            Y = state.objectY;

//            str += QString::number(state.objectX);
//            str += ", ";
//            str += QString::number(state.objectY);
//            str += ", ";
//            DETECTED = true;

//        } else {
//            str += "NOT DETECTED";
//        }

//       std::cout <<  str.toStdString() << std::endl;
//       Robot->Write_To_Log(0xf014, str);
//       GUI_Write_To_Log(0xf014, str);
//    }




//    str += ui->All_Servos_lineEdit->text ();
//    Robot->Write_To_Source (value, str);
//}

//void MainProcess::on_PUTButton_clicked()
//{
//    QByteArray dd;
//    //+++++++++++++++++++++ 3 put the cube
//    // {60, 93, 100, 35, 145, 35};
//    this->update_LineDits_from_position (put_position);
//    //   this->repaint();
//    update_Servos_from_LineEdits ();
//    memcpy(dd.data(), Servos, 6);
//    dd.insert(6, 0x31); // Движение "Туда"
//    this->send_Data(BEFORE_LAST); //0xE9, NOT_LAST ==C8
//    //+++++++++++++++++++++ 4  Unclamp the gripper
//    //on_clampButton_clicked();
//    if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
//    else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
//    this->send_Data(NOT_LAST);

//    //+++++++++++++++++++++ 6 go back to start position
//    //on_stand_upButton_clicked();
//    this->update_LineDits_from_position(hwr_Start_position);
//    //   this->repaint();
//    this->update_Servos_from_LineEdits();
//    dd.insert(6, 0x30); // Движение "Обратно"
//    this->send_Data(LASTONE); // The last command

//}
