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
//(QObject *parent)

MainProcess::
MainProcess(QObject *parent)
    : QObject(parent)
    , readSocket("../iqr.socket")

{
    int value = 0x0000;

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

    GUI_Write_To_Log(0000, "Going to Start QTcpSErver");
    if (server.isListening ()) {

        str = "Listening on address "; str += server.serverAddress().toString();
        str += " and port "; str += QString::number(server.serverPort());//QString::number(server.tcpport);

        GUI_Write_To_Log(0000, str);
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
    connect( this, SIGNAL (Open_Port_Signal(QString)), Robot, SLOT(Open_Port_Slot(QString)));
    connect( &Robot->serial, SIGNAL (readyRead()), Robot, SLOT(ReadFromSerial_Slot()));  //&QSerialPort::

    //#################### Signal to web-server
    connect( Robot, SIGNAL (Moving_Done_Signal()), this, SLOT (Moving_Done_Slot()));;;


    // ============================================== Создаем поток 1 - web-server
    // connect(chan_A, SIGNAL(Process_A()), TheWeb, SLOT(Output_Data_From_Client_Slot()), Qt::QueuedConnection); //Считываем из железки в ПК по каналу А
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
    // Arduino Mega - как "ttyACM0"
    // emit Open_Port_Signal("ttyUSB1");

    str = "Current Servo values are : ";
    str += QString::number(Servos[0]);
    GUI_Write_To_Log(value, str);

    int OKay = Robot->Open_Port_Slot("ttyUSB0");
    if (!OKay) { //!Robot->SerialIsOpened
        Robot->current_st_index = 4;

       OKay = Robot->Open_Port_Slot("ttyUSB1");

    } // Robot->current_status = statuslst.at(4)

    if (!OKay){
                GUI_Write_To_Log(value, "SerialPort  PROBLEM !!!");
                // ТОгда таймер пускаем ???
    };
    //+++++++++ Проверяем, что работает QSerialPort, открываем/закрываем хват
//    QThread::sleep(2);
//    //on_clampButton_clicked();
//    str = "Make UNLOCK";
//    GUI_Write_To_Log(value, str);
//    Data_From_TcpClient_Slot("unlock");
//    QThread::sleep(1);
//    //on_clampButton_clicked();
//    str = "Make LOCK";
//    GUI_Write_To_Log(value, str);
//    Data_From_TcpClient_Slot("lock");
//    QThread::sleep(1);
//    str = "And NOW Current Servo[0] value is : ";
//    str += QString::number(Servos[0]);
//    GUI_Write_To_Log(value, str);


} //MainProcess
//++++++++++++++++++++++++++++++++++++++++++++++
MainProcess::~MainProcess()
{
    GUI_Write_To_Log(0xffff, "Program is going to be closed");
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

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainProcess::on_clampButton_clicked()
{
    if (Servos[0]>0){ Servos[0]=0;}
    else {Servos[0]=90;}
    this->send_Data(LASTONE); // Движение "Туда" по умолчанию

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Обработка сигнала on_trainButton_clicked()
// Выполняем целый набор действий - взять кубик и положить на транспортировщик
void MainProcess::on_trainButton_clicked()
{

    DetectorState state;
    QString str, fstr;
    str = "";
    fstr = "";
//Сразу открываем захват
//    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
//    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
    Servos[0]=0;

    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){

            str = "Just float values of state : ";
            fstr.setNum(state.objectX);
            str += fstr;
            str += ", ";
            fstr.setNum(state.objectY);
            str += fstr;
            GUI_Write_To_Log(0xf014, str);


            int kf = 100000;
            try_mcinfer(state.objectX*kf, state.objectY*kf); // Тут меняем current_status = "inprogress". Команда 0 - Переместить открытый хват к кубику.
            X = state.objectX;                        //  Хват открывается в процессе движения робота, а не отдельной командой.
            Y = state.objectY;
            std::cout << "DETECTED: " << state.objectX << " " << state.objectY << std::endl;
            //str += QString::number(state.objectX);
            fstr.setNum(state.objectX);
            str += fstr;
            str += ", ";
            //str += QString::number(state.objectY);
            fstr.setNum(state.objectY);
            str += fstr;
            DETECTED = true;

        } else {
            str += "NOT DETECTED";
        }

       std::cout <<  str.toStdString() << std::endl;
       Robot->Write_To_Log(0xf014, str);
       GUI_Write_To_Log(0xf014, str);
    }

//В этой точке робот опустился над кубиком, открыл захват.

if (DETECTED)
    {

        QByteArray dd ;
        dd.resize(parcel_size);
        str = "Next movement to robot";
        this->GUI_Write_To_Log (0xF055, str);
   //+++++++++++++++++ 1 make clamp, хватаем кубик
   //on_clampButton_clicked();
        Servos[0]=90;
        this->send_Data(NOT_LAST); // Движение "Туда" по умолчанию
   //++++++++++++++++++++ 2 make stand up, встаем в исходную точку
   //on_stand_upButton_clicked();
  // this->update_Servos_from_position(hwr_Start_position);
        memcpy(Servos, hwr_Start_position, DOF);
        this->send_Data(LASTONE);

   //+++++++++++++++++++++ 3 put the cube, наклоняем захват с кубиком к транспортировщику
   // {60, 93, 100, 35, 145, 35};
//   this->update_Servos_from_position(put_2_mobman);
//   memcpy(dd.data(), Servos, 6);
//   dd.insert(6, 0x31); // Движение "Туда"
//   this->send_Data(NOT_LAST); //BEFORE_LAST==0xE9, NOT_LAST ==C8
//   //+++++++++++++++++++++ 4  Unclamp the gripper, открываем захват, т.е. кладем кубик на пол, чтобы его взял транспортировщик
//   //on_clampButton_clicked();
//   Servos[0]=0;
//   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++ 5 Приподнять хват, чтобы не задеть тележку.
//      this->update_Servos_from_position(after_put_2_mobman);
//      memcpy(dd.data(), Servos, 6);
//      dd.insert(6, 0x30); // Движение "Туда"
//      this->send_Data(AFTER_PUT);



   //+++++++++++++++++++++ 6 go back to start position
   this->update_Servos_from_position(hwr_Start_position);
   memcpy(dd.data(), Servos, 6);
   dd.insert(parcel_size-2, 0x30); // Движение "Обратно"
   dd.insert(parcel_size-1, LASTONE);

   // this->ssend_Data(dd); // The last command
   Robot->GoToPosition(dd);

  }// if (DETECTED)

   DETECTED = false;

}//on_trainButton_clicked()
//++++++++++++++++++++++++++++++++++++
void MainProcess::send_Data(unsigned char thelast)
{
    QByteArray dd ;
    dd.resize(parcel_size);
    memcpy(dd.data(), Servos, DOF);
    dd.insert(parcel_size-2, 0x31); // Движение "Туда"
    dd.insert(parcel_size-1, thelast);
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
//++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainProcess::put_box()
{
    QByteArray dd ;
    dd.resize(parcel_size);

    //+++++++++++++++++++++ 3 put the cube, наклоняем захват с кубиком к транспортировщику
    // {60, 93, 100, 35, 145, 35};
    //this->update_Servos_from_position(put_2_mobman);
    //memcpy(dd.data(), put_2_mobman, 6);
    memcpy(Servos, put_2_mobman, DOF);
    this->send_Data(NOT_LAST);
//    dd.insert(parcel_size-2, 0x31); // Движение "Туда"
//    dd.insert(parcel_size-1, NOT_LAST);
    //BEFORE_LAST==0xE9, NOT_LAST ==C8
//    Robot->GoToPosition(dd);
    //+++++++++++++++++++++ 4  Unclamp the gripper, открываем захват, т.е. кладем кубик на пол, чтобы его взял транспортировщик
    //unlock
    Servos[0]=0;
    this->send_Data(NOT_LAST);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++ 5 Приподнять хват, чтобы не задеть тележку.
//       this->update_Servos_from_position(after_put_2_mobman);
       memcpy(dd.data(), after_put_2_mobman, DOF);
       dd.insert(parcel_size-2, 0x30); // Движение "обратно"
       dd.insert(parcel_size-1, AFTER_PUT);
       Robot->GoToPosition(dd);

    //+++++++++++++++++++++ 6 go back to start position
//    this->update_Servos_from_position(hwr_Start_position);
    memcpy(dd.data(), hwr_Start_position, 6);
    dd.insert(parcel_size-2, 0x30); // Движение "Обратно"
    dd.insert(parcel_size-1, LASTONE);

    // this->ssend_Data(dd); // Нет. т.к. там всегда движения "Туда"
    Robot->GoToPosition(dd);

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++ Получили данные (запрос) от клиента. Парсим.
void MainProcess::Data_From_TcpClient_Slot(QString message)
{
    QString str, substr;
    int value = 0xf00f;
    new_get_request = true;
    //str = "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSimpleServer->Info_2_Log_Signal !!!!!!!!!!!!!!!!!!!";
    str = "From TCP Get new command : "; str += message;
    GUI_Write_To_Log(0xf00f, str);

        substr = message;

        if (substr == "start") {
            Robot->SetCurrentStatus ("init"); // Перед запуском распознавания

            on_trainButton_clicked ();
            str = "Robot current status is ";
            str += Robot->current_status;
            Robot->Write_To_Log(0xf00F, str);

//            str = Robot->current_status;
//            emit Write_2_TcpClient_Signal (str);
         }

        if (substr == "put_box"){
            Robot->SetCurrentStatus ("inprogress"); // Перед запуском распознавания
            put_box();


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

       emit Write_2_TcpClient_Signal (str);
   }

   if (substr == "sit") {
       // Go to "sit" position
       QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(sit_down_position), 6);
       dd.append(0x31); // Движение "Туда"
       dd.append(LASTONE); // Всегда последнее ?
       Robot->GoToPosition(dd);//, sit_down_position
   }//"sit"

   if (substr == "standup") {
       // Go to Initial "Start" position
       QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(hwr_Start_position), 6);
       dd.append(0x30); // Движение "Обратно"
       dd.append(LASTONE); // Всегда последнее ?
       Robot->GoToPosition(dd);//, hwr_Start_position

   }

   if (substr =="clamp") {

       str = "######## Try to lock/unlock the gripper #########";
       str += "\n";
       str += "Current clamper value is ";
       str += QString::number(Servos[0]);
 //      GUI_Write_To_Log(value, str);
       if(Servos[0]==0) { Servos[0]=90;}
       else {Servos[0]=0;}
       this->send_Data(LASTONE);
   }//"clamp"

   if (substr =="lock") {

       str = "######## Try to lock the gripper ######### ";
       str += "Current clamper value is ";
       str += QString::number(Servos[0]);
//       GUI_Write_To_Log(value, str);
       Servos[0]=90;
       this->send_Data(NOT_LAST); //NOT_LAST LASTONE
   }//"lock"

   if (substr =="unlock") {

       str = "######## Try to UNlock the gripper ######### ";
       str += "Current clamper value is ";
       str += QString::number(Servos[0]);
//       GUI_Write_To_Log(value, str);
       Servos[0]=0;
       this->send_Data(NOT_LAST); //NOT_LAST LASTONE
   }//"unlock"

   if (substr == "close") {
       Robot->serial.close();

   }

   // Запрашиваем в Arduino значения углов сервоприводов
   if (substr == "getservos") {
       QByteArray dd ;
       dd.resize(parcel_size);
       memcpy(dd.data(), get_values_position, parcel_size);
       Robot->GoToPosition(dd);

   }



}
//++++++++++++++++++++++++++
// Слот сигнала HiWonder::Moving_Done_Signal
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
// Пришло из GUI-версии. В console-версии тоже нужно.
void MainProcess::update_Servos_from_position(unsigned char *pos)
{
    for (int i =0; i< DOF; i++) { Servos[i] = pos[i]; }

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
