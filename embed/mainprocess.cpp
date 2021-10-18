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


MainProcess::MainProcess(QObject *parent)
    : QObject(parent)
    , readSocket("../../simpledetector_cpp/iqr.socket")

{

    DETECTED = false;
    new_get_request = false;
    thread_counter = 0;

 //   target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
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
    connect(this, &MainProcess::Write_2_Client_Signal, &server, &QSimpleServer::Write_2_Client_SLot);
    // Чтение данных от клиента серверу (из ЦУП)
    connect(&server, SIGNAL(Info_2_Log_Signal(QString)), this, SLOT(Info_2_Log_Slot(QString)));
    //connect(Robot, SIGNAL(StatusChangedSignal(QString)), &server, SLOT(SetCurrentState(QString)));
    //connect(this, SIGNAL(StartTakeAndPutSignal()), this, SLOT(TakeAndPutSlot()));

   // connect(&server, &QTcpServer::newConnection, this, &MainProcess::newConnection_Slot);
    //################### SERIAL SIGNAL/SLOTS ############################
    connect( this, SIGNAL (Open_Port_Signal(QString)), Robot, SLOT(Open_Port_Slot(QString)));
    connect( &Robot->serial, SIGNAL (readyRead()), Robot, SLOT(ReadFromSerial_Slot()));  //&QSerialPort::

    //RMath = new Robo_Math();
    //################### Robo_Math SIGNAL/SLOTS #########################
//    connect( this, SIGNAL (Pass_XY_Signal(int, int)), RMath, SLOT(Pass_XY_Slot(int, int)));
//    connect( RMath, SIGNAL (Return_EL_Signal(float)), this, SLOT(Return_EL_Slot(float)));
//    connect( this, SIGNAL (FW_Kinemaic_Signal(int, int, int, int, int, int)), RMath, SLOT(FW_Kinemaic_Slot(int, int, int, int, int, int)));
//    connect( RMath, SIGNAL(Return_FW_Kinematic_XYZ_Signal(int, int, int, float)), this, SLOT(Return_FW_Kinematic_XYZ_Slot(int, int, int, float)));

//    connect(RMath, SIGNAL (Pass_String_Signal(QString)), this, SLOT(Pass_String_Slot(QString)));
    //#################### Signal to web-server
    connect( Robot, SIGNAL (Moving_Done_Signal()), this, SLOT (Moving_Done_Slot()));;;


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
    emit Open_Port_Signal("ttyUSB0");
    //make_json_answer();




}
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
    memcpy(dd.data(), Servos, 6);
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
    if (Servos[0]>0){ Servos[0]=0;}
    else {Servos[0]=90;}
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


//++++++++++++++++++++++++++
void MainProcess::on_trainButton_clicked()
{
//    for (int i =0; i<= DOF -1; i++)
//    {
//       Servos[i] = train_position[i];
//    }
//    update_LineDits_from_servos ();


    DetectorState state;
    QString str;
    str = "";
//Сразу открываем захват
//    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
//    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
    Servos[0]=0;
  //  update_LineDits_from_servos();

    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){
            try_mcinfer(state.objectX, state.objectY); // Тут меняем current_status = "inprogress". Команда 0 - Переместить открытый хват к кубику.
            X = state.objectX;                        //  Хват открывается в процессе движения робота, а не отдельной командой.
            Y = state.objectY;
            str+="DETECTED: ";
            str += QString::number(state.objectX);
            str += ", ";
            str += QString::number(state.objectY);
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
//    memcpy(dd.data(), Servos, 6);
//    dd.insert(6, 0x31); // Движение "Туда"
//    Robot->GoToPosition(dd);
////    while (!Robot->MOVEMENT_DONE) {;}
   str = "Next movement to robot";
   this->GUI_Write_To_Log (0xF055, str);
   //+++++++++++++++++ 1 make clamp, хватаем кубик
   //on_clampButton_clicked();
   if (Servos[0] > 0){ Servos[0]=0;}
   else {Servos[0]=90;}
   this->send_Data(NOT_LAST);
   //++++++++++++++++++++ 2 make stand up, встаем в исходную точку
   //on_stand_upButton_clicked();
   this->update_Servos_from_position(hwr_Start_position);
   this->send_Data(NOT_LAST);

   //+++++++++++++++++++++ 3 put the cube, наклоняем захват с кубиком к транспортировщику
   // {60, 93, 100, 35, 145, 35};
   this->update_Servos_from_position(put_position);
   memcpy(dd.data(), Servos, 6);
   dd.insert(6, 0x31); // Движение "Туда"
   this->send_Data(BEFORE_LAST); //0xE9, NOT_LAST ==C8
   //+++++++++++++++++++++ 4  Unclamp the gripper, открываем захват, т.е. кладем кубик на транспортировщик
   //on_clampButton_clicked();
   Servos[0]=0;
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++ 5 Приподнять хват, чтобы не задеть тележку.
      this->update_Servos_from_position(after_put_position);
      memcpy(dd.data(), Servos, 6);
      dd.insert(6, 0x30); // Движение "Туда"
      this->send_Data(AFTER_PUT);



//   //+++++++++++++++++++++ 6 go back to start position
//   //on_stand_upButton_clicked();
//   this->update_LineDits_from_position(hwr_Start_position);
//   //   this->repaint();
//   this->update_Servos_from_LineEdits();
//   dd.insert(6, 0x30); // Движение "Обратно"
//   this->send_Data(LASTONE); // The last command


////   //+++++++++++++++++++++ 3 put the cube
////   this->update_LineDits_from_position (put_position);
////   //   this->repaint();
////   update_Servos_from_LineEdits ();
////   memcpy(dd.data(), Servos, 6);
////   dd.insert(6, 0x31); // Движение "Туда"
////   this->send_Data(NOT_LAST);
////   //+++++++++++++++++++++ 4  Unclamp the gripper
////   //on_clampButton_clicked();
////   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
////   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
////   this->send_Data(NOT_LAST);
////   //+++++++++++++++++++++++++++++++++ 5 Приподнять хват, чтобы не задеть тележку.
////   this->update_LineDits_from_position (after_put_position);
////   //   this->repaint();
////   update_Servos_from_LineEdits ();
////   memcpy(dd.data(), Servos, 6);
////   dd.insert(6, 0x31); // Движение "Туда"
////   this->send_Data(NOT_LAST);

////   //+++++++++++++++++++++ 6 go back to start position
////   //on_stand_upButton_clicked();
////   this->update_LineDits_from_position(hwr_Start_position);
////   //   this->repaint();
////   this->update_Servos_from_LineEdits();
////   this->send_Data(LASTONE); // The last command

  }// if (DETECTED)

   DETECTED = false;
   //Robot->current_status = "done"; // Тут еще рано, команды только отправлены.


}//
//++++++++++++++++++++++++++++++++++++
void MainProcess::send_Data(unsigned char thelast)
{
   // QString str;


    QByteArray dd ;
    dd.resize(parcel_size);
    memcpy(dd.data(), Servos, 6);
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
//+++++++ Получили запрос от клиента. Парсим его.
void MainProcess::Info_2_Log_Slot(QString message)
{
    QString str, substr;
    int value = 0xf00f;
    new_get_request = true;
    str = "Get new command : "; str += message;
    GUI_Write_To_Log(0xf00f, str);

//    int sPosition, ePosition; // Индекс строки run в запросе.
//    sPosition = message.indexOf("/run?cmd=");

//   QString  wrong_mess = "/favicon.ico HTTP/1.1";

//    if (!message.contains (wrong_mess))
//    {
//        sPosition += 9;
//        ePosition = message.indexOf("&", sPosition);
//        substr = message.mid(sPosition, (ePosition - sPosition));


//        str = "Получена команда : "; str += substr;
//        GUI_Write_To_Log(0xf00f, str);

        substr = message;

        // changed by Miksarus
        if (substr == "start") {
            //on_clampButton_clicked ();
            Robot->SetCurrentStatus ("init");
            //emit StartTakeAndPutSignal();

            // Движение только начинаем, поэтому обнулим значение LASTONE
            //emit on_trainButton_clicked ();
            str = Robot->current_status;
            //str = "status_from_robot";
            emit Write_2_Client_Signal (str);
         }

        if (substr == "reset") {
            if (Robot->GetCurrentStatus () != "wait"){
                Robot->SetCurrentStatus ("wait");
                str = "Robot changed status, now it is : ";
                str += Robot->current_status;

                GUI_Write_To_Log (value, str);
                str = Robot->current_status;
                //str = "status_from_robot";
                emit Write_2_Client_Signal (str);
            }
         }
//         ///run?cmd=status&123


   if (substr == "status") {
      // str  = "{\n\t\"status\":\"";
       str = Robot->current_status;
     //  str += "\"\n}";

       emit Write_2_Client_Signal (str);
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
