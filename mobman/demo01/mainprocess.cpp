#include "mainprocess.h"
//#include "ui_MainProcess.h"
//#include "positions.h"
//#include "SerialRobot.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//#include <QtGui>
#include <chrono>
#include <thread>
#include "tcpParcing.cpp"
#include "processAction.cpp"
//#include "nlohmann/json.hpp"

//(QObject *parent)

//using json = nlohmann::json;

MainProcess::MainProcess(QObject *parent)
    : QObject(parent)
//    , readSocket("../../simpledetector_cpp/iqr.socket")

{
    int value = 0x0000;
    parcel_size  = 6;

    DETECTED = false;
    new_get_request = false;
    thread_counter = 0;
    currentTcpdata = "";

    cvAnswer.detected = false;
    cvAnswer.distance = 0.0;

    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    //QByteArray ba = target_name.toLocal8Bit();
    //g/const char *c_str = ba.data();
    //printf("Appname : %s", c_str);

    // Инициализируем все json-статусы
    qRegisterMetaType<QJsonObject>("QJsonObject");
    jsnStore = new JsonInfo(DEVICE_NAME);
    mainjsnObj = jsnStore->returnJsnActionReset(); //set global value
    jsnStore->createActionList();

    ptrTcpClient = new QObject(nullptr);

    Robot = new SerialRobot();

    Robot->Log_File_Open(Log_File_Name);
    this->LogFile_Open(MOBMAN_LOG);
  //  Robot->Source_Points_File_Open (SOURCE_POINTS_FILE);

    QString str = "The application \"";  str +=target_name; str += "\"";
    //qDebug() << QCoreApplication::instance()->thread();
//    qintptr myThread = QThread::currentThread()->toInt;

    Robot->Write_To_Log(value, str.append(" is started successfully!!!\n"));
    str = "The thread of \"";  str +=target_name; str += "\" is ";
    str += QString("0x%1").arg((qintptr)this->QObject::thread(),16);
    GUI_Write_To_Log(value, str);

    qDebug() << "Started " << target_name;

    GUI_Write_To_Log(value, "Going to Start QTcpServer");

    //+++++++++++++++++++++++++++++++++  signal/slot of Get Request to webserver

    //################### SERIAL SIGNAL/SLOTS ############################
    connect( this, &MainProcess::Open_Port_Signal, Robot, &SerialRobot::Open_Port_Slot);
    connect( &Robot->serial, &QSerialPort::readyRead, Robot, &SerialRobot::ReadFromSerial_Slot);  //&QSerialPort::


    //#################### Signal to web-server
    connect( Robot, &SerialRobot::Moving_Done_Signal, this, &MainProcess::Moving_Done_Slot);


    //++++++++++++++++++++++++++++++++++++ JSON signal/slot ++++++++++++++++++++++++++++++
    connect(this, &MainProcess::SetActionData_Signal, jsnStore, &JsonInfo::SetActionData_Slot, Qt::QueuedConnection);

    // ==============================================
                                                                       // Qt::DirectConnection
                                                                       // Qt::QueuedConnection
                                                                       // Qt::BlockingQueuedConnection
                                                                       // Qt::DirectConnection
/*  Value
    0   AutoConnection (Default) If the receiver lives in the thread that emits the signal, Qt::DirectConnection is used.
        Otherwise, Qt::QueuedConnection is used. The connection type is determined when the signal is emitted.
 *
 *  1 DirectConnection - The slot is invoked immediately, when the signal is emitted. The slot is executed in the emitter's thread,
 *     which is not necessarily the receiver's thread.
 *
    2 QueuedConnection - The slot is invoked when control returns to the event loop of the receiver's thread.
       The slot is executed in the receiver's thread.

    3 BlockingQueuedConnection - The slot is invoked as for the Queued Connection,
       except the current thread blocks until the slot returns. Note: Using this type to connect objects in the same thread will cause deadlock.

*/




    //+++++++++++++++ ОТкрываем порт Open_Port_Signal(QString portname); ttyUSB0
    // Arduino NANO виден как ttyUSB0
    // Arduino Mega - как "ttyACM0"
  //  emit Open_Port_Signal("ttyACM0"); //"ttyUSB0"
    int OKay = Robot->Open_Port_Slot("ttyACM0");
    if (!OKay) { //!Robot->SerialIsOpened
        Robot->current_st_index = 4;

       OKay = Robot->Open_Port_Slot("ttyACM1");

    } // Robot->current_status = statuslst.at(4)

    if (!OKay){
                GUI_Write_To_Log(value, "SerialPort  PROBLEM !!!");
                // ТОгда таймер пускаем ???
    };


    init_json(); // Инициализируем json_status

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
    //
 QThread::sleep(1);


} //MainProcess
//++++++++++++++++++++++++++++++++++++++++++++++
MainProcess::~MainProcess()
{
    QString str = "Program is going to be closed";
    GUI_Write_To_Log(0xffff, str);
    qDebug() << str;
    delete Robot;
    delete jsnStore;
    delete socketCV;
    delete ptrTcpClient;



}
//+++++++++++++++++++++++++++++++++++++++
void MainProcess::GUI_Write_To_Log (int value, QString log_message)
{
    QMutexLocker locker(&mutex);
    QDateTime curdate ;
    QTextStream uin(&this->mobWebLogFile);

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
/*
void MainProcess::on_openButton_clicked()
{
QString portname = "ttyACM0"; //ui->comL->currentText();
emit Open_Port_Signal(portname);
//    serial.setPortName(ui->comL->currentText());
//    serial.open(QIODevice::ReadWrite);
//    serial.setBaudRate(QSerialPort::Baud115200);
//    serial.setDataBits(QSerialPort::Data8);
//    serial.setParity(QSerialPort::NoParity);
//    serial.setStopBits(QSerialPort::OneStop);
//    serial.setFlowControl(QSerialPort::NoFlowControl);

*/

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

}
*/
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
//    quint8 FULL_OPENED, FULL_CLOSED;
//    FULL_CLOSED = 80;
//    FULL_OPENED = 35;
    // Если открыто, то закрываем
    if (Servos[0] < FULL_CLOSED){ Servos[0]=FULL_CLOSED;}
    else {Servos[0]=FULL_OPENED;}

    //on_set_posButton_clicked();
    this->send_Data(LASTONE);
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

//    if (server.isListening ()) {

//        str = "Listening on address "; str += server.serverAddress().toString();
//        str += " and port "; str += QString::number(server.serverPort());//QString::number(server.tcpport);

//        GUI_Write_To_Log(0000, str);
//    }


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

//    int value = 0xcdcd;
    QByteArray dd ;
    dd.resize(parcel_size);
//    QString mystr = "Current Parcel Size is "; mystr += QString::number(parcel_size); // Без этой записи никогда бы не докопался до истины :)
//    GUI_Write_To_Log(value, mystr);

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
  jsn_str += "Under Construction"; //Robot->current_status;
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
// Это ответ на команду status - общие данные о девайсе.
void MainProcess::init_json()
{
     jsnStatus = {
        {"name", DEVICE_NAME},
        {"rc", RC_UNDEFINED}, //RC_SUCCESS
        {"info", "Request Accepted"},
        {"state", "Wait"},
        {"action_list", {
           {
            {"name", "clamp"},
            {"state", "inprogress | done | fail"},
            {"info", "Open/Close clamper"},
            {"rc", "int - action return code"}
           },
           {
            {"name", "formoving"},
            {"state", "inprogress | done | fail"},
            {"info", "Alternative transporting position for device' clamper"},
            {"rc", "int - action return code"}
           },
           {
            {"name", "get_box"},
            {"state", {"waiting","noDetection", "inprogress", "done", "fail"}},
            {"info", "Get the box by clamper, ascing CV about distance in advance"},
            {"st_time", "int - timestamp of last start"},
            {"fin_time", "int - timestamp of finish"},
            {"result", "int - action return code"}
           },
           {
            {"name", "reset"},
            {"state", "succsess | fail"},
            {"info", "Set device status as <Wait>"},
            {"rc", "int - action return code"}
           },
           {
            {"name", "parking"},
            {"state", "inprogress | done | fail"},
            {"info", "Set device's clamper in transporting position"},
            {"rc", "int - action return code"}
            },
           {
            {"name", "setservos="},
            {"state", "inprogress | done | fail"},
            {"info", "Set device's servos at angles specified by the command"},
            {"rc", "int - action return code"}
           },

           } //list
         }//action_list-field

     }; // jsnStatus




     jsnGetServicesAnswer = {
                            {"name" , "getservices"},
                            {"rc", RC_SUCCESS}, //RC_SUCCESS
                            {"info" , "Request Accepted"},
                            {"data", {
                               {
                                 {"name", "getservices"},
                                 {"info", "return list of available services"}
                               },
                               {
                                 {"name", "getactions"},
                                 {"info", "return list of available actions"}
                               }

                             }

                            }//data


                     }; // jsnGetServicesAnswer



     //        -5 - action с таким именем успешно завершен

     // Это просто список без значений.
     jsnGetActionsAnswer = {
                             {"name" , "getactions"},
                             {"rc", RC_SUCCESS}, //RC_SUCCESS
                             {"info" , "Request Accepted"},
         {"data", {
            {
             {"name", "get_box"},
             {"state", {"waiting","noDetection", "inprogress", "done", "fail"}},
             {"info", "Get the box by clamper, ascing CV about distance in advance"},
             {"st_time", "int - timestamp of last start"},
             {"fin_time", "int - timestamp of finish"},
             {"result", "int - action return code"}
            },
            {
             {"name", "reset"},
             {"state", "succsess | fail"},
             {"info", "Set device status as <Wait>"},
             {"rc", "int - action return code"}
            },
            {
              {"name", "parking"},
              {"state", "inprogress | done | fail"},
              {"info", "Set device's clamper in transporting position"},
              {"rc", "int - action return code"}
             },
            {
              {"name", "setservos="},
              {"state", "inprogress | done | fail"},
              {"info", "Set device's servos at angles specified by the command"},
              {"rc", "int - action return code"}
            },
            {
              {"name", "ready"},
              {"state", "inprogress | done | fail"},
              {"info", "Set device's clamper in rady-to-clamp position"},
              {"rc", "int - action return code, 0=successfull, -1=already running"}
             },
             {
              {"name", "formoving"},
              {"state", "inprogress | done | fail"},
              {"info", "Alternative transporting position for device' clamper"},
              {"rc", "int - action return code"}
             }





            } //list
          }//action_list-field

     };//jsnGetActionsAnswer

jsnStatusActionAnswer.insert("name", QJsonValue("get_box"));
jsnStatusActionAnswer.insert("state", QJsonValue("waiting"));
jsnStatusActionAnswer.insert("info", QJsonValue("Get the object via CV"));
jsnStatusActionAnswer.insert("st_time", QJsonValue(0));
jsnStatusActionAnswer.insert("fin_time", QJsonValue(0));
jsnStatusActionAnswer.insert("result", QJsonValue(-5));


} //init_json()
//++++++++++++++++++++++++++++++++++++++++++
// СОздаем сокет, посылаем запрос, считываем ответ.
void MainProcess::request_CV()
{
    /*
     * Now, TCP is based on sending a stream of data, so we cannot expect to get the entire fortune in one go.
     * Especially on a slow network, the data can be received in several small fragments. QTcpSocket buffers up all incoming data
     * and emits readyRead() for every new block that arrives, and it is our job to ensure that we have received all the data
     * we need before we start parsing.
 */


//    // Создание объекта потока - класс QThread
//    threadCV = new QThread;
//    //Создание объекта для сокета - класс QObject
//   // int CVSocketDescriptor = 0x12345678; // Пока просто число.
//    ClientSocket *CVSocketthread = new ClientSocket(CVDev_IP, CVDev_Port);

//    // Заворачиваем QObject в QThread
//    CVSocketthread->moveToThread(threadCV);


//    //Соединение сигнала завершения потока со слотом отложенного удаления
//    connect(CVSocketthread, SIGNAL(finished()), threadCV, SLOT(quit()));
//    connect(CVSocketthread, SIGNAL(finished()), CVSocketthread, SLOT(deleteLater()));
//    connect(threadCV, SIGNAL(started()), CVSocketthread, SLOT(process_TheSocket()),Qt::QueuedConnection);
//    connect(threadCV, SIGNAL(finished()), threadCV, SLOT(deleteLater()));


//    //connect(CVSocketthread,

//    //Запуск потока
//    threadCV->start();
//    //addPendingConnection(sDescriptor);





//    QString str = "Going to create tcp-socket for CVDevice";
//    GUI_Write_To_Log(0xC1C1, str);
    QMutexLocker locker(&mutex);
    socketCV = new QTcpSocket(this);
    socketCV->setSocketOption(QAbstractSocket::KeepAliveOption, true);
    //in.setDevice(socketCV);

    //Соединение сигналов со слотами                        было  CV_onReadyRead_Slot
    connect(socketCV, &QIODevice::readyRead, this, &MainProcess::CV_NEW_onReadyRead_Slot, Qt::QueuedConnection);//, Qt::QueuedConnection);


    // Убираем, т.к. единственная цель была - запись в лог. В продакшене не нужно.
    //connect(socketCV, &QAbstractSocket::disconnected, this, &MainProcess::CV_onDisconnected, Qt::QueuedConnection);

    // Удаляем объект QAbstractSocket иначе утечики памяти и прога вылетит через некоторое время(когда сожрет всю память).
    connect(socketCV, &QAbstractSocket::disconnected, socketCV, &QAbstractSocket::deleteLater);

    connect (this->socketCV, &QTcpSocket::connected, this, &MainProcess::onSocketConnected_Slot);
    socketCV->connectToHost(CVDev_IP, CVDev_Port);

    //Сохраняем значение указателя, иначе вновь пришедший запрос статуса переписывает его.
    this->newPtrTcpClient = this->ptrTcpClient;

//    qDebug() << "$$$$$$$$$$$$$$ request_CV thread " << QThread::currentThread();
//    qDebug() << "$$$$$$$$$$$$$$ request_CV ptrSender " << this->ptrTcpClient;

//    QString mystr = "requestCV Pointer to SENDER QObject is ";
//    mystr += pointer_to_qstring(this->ptrTcpClient);
//    GUI_Write_To_Log(0x9999, mystr);

}
//++++++++++++++++++++++++++++++++++++++++++++++++
void MainProcess::request_New_CV()
{
    ;
//    socketCV->abort();
//    socketCV->connectToHost(CVDev_IP, CVDev_Port);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
int MainProcess::my_round(int n)
{
    // Smaller multiple
        int a = (n / 10) * 10;

        // Larger multiple
        int b = a + 10;

        // Return of closest of two
        return (n - a > b - n)? b : a;

}
//++++++++++++++++++++++++++++++++++++++++++++++++++
// В конкретном проекте с мобильным манипулятором нужно парсить только ответ от CV, так что
// пока не претендуем на универсальность.
// JSON-данные пишем в глобальную переменную jsndataObj
// Нам повезло, что по окончании это ф-ции значение jsndataObj как раз хранит
// значение объекта "data"
void MainProcess::traversJson(QJsonObject json_obj)
{
    QString str;
    bool isDetected;
    int thevalue = 0x5555;
    foreach(const QString& key, json_obj.keys()) {
        str = "";
        QJsonValue value = json_obj.value(key);
        if(!value.isObject() ){
//                      str +=  "Key = "; str += key; str += ", Value = ";
                      // Важно. Сначала нужно привести к типу bool
                      //  QVariant(value).toBool() - не работает
                      if (value.isBool() && key == "detected") {this->DETECTED = value.toBool(); str += QVariant(this->DETECTED).toString();}
                      if (value.isString()) {str += QVariant(value).toString();}
                      if (value.isDouble()) {str += QString::number( QVariant(value).toDouble());}
                      // Хотя у CV в ответе и нет никакого массива, но все же добавим.
                      if (value.isArray()) {
                          QJsonArray jsnArray = value.toArray();
                          // А еще желательно проверить, что у этого value соответствующий key ==  "action_list":
                          // Тут выводим элементы массива, точнее только имена экшенов, т.е. значение name
                          if (key == "action_list") {str += "Many actions, look at output above";}
                      }


                     // str += value.toString();
//                      GUI_Write_To_Log(0x5555, str);
//                      str = "";

          //qDebug() << "Key = " << key << ", Value = " << value;
         }
        else{
            // А теперь определяем тип данных поля.
             //jsndataObj = json_obj["data"].toObject(); // Так тоже работает, но уже есть привязка к конкретному случаю.
             jsndataObj = value.toObject(); // В нашем случае объект единственный - "data"
             // При последующем запуске traversJson попадаем уже в блок  if(!value.isObject() )==true
             // и там достаем "distance"
             // check if there is the key "detected"
//             if (jsndataObj.contains("detected")){

//                 isDetected = jsndataObj.value("detected").toBool();
//                 //this->DETECTED = jsndataObj.value("detected").toBool();

//                 str = "Object Detected value is ";
//                 str += QVariant(isDetected).toString();
//                 GUI_Write_To_Log(thevalue, str);

//                 if (!isDetected){

//                     GUI_Write_To_Log(thevalue, "!!!!!! No Detection. Exit. Try Again !!!!!!!!!!!");
//                     return;
//                 }

                 traversJson(jsndataObj);

//             } //if (jsndataObj.contains("detected"))

             //traversJson(value.toObject());


        }//else

//        GUI_Write_To_Log(0x5555, str);


    }//foreach

}//traverseJson
//++++++++++++++++++++++++++++++++++++++++++++++
// Парсим JSON-ответ от девайсов
// 0x20 (пробел), 0x09 (табуляцию), 0x0A (перевод строки) и 0x0D (возврат каретки).
// Пробелы допускаются до и после «структурных символов» (structural characters) []{}:,
// QString (данные от CV-Device) -> QJsonDocument -> QJsonObject и вот дальше надо парсить.

double MainProcess::parseJSON(QString jsnData)
{
    int value = 0xC7C7;
    QString str, substr;

    int sPosition; // Индекс строки run в запросе. , ePosition
    sPosition = jsnData.indexOf("{");
    substr = jsnData.mid(sPosition);


   // jsnAnswer = ordered_json::parse(substr.toStdString());
   // str.toStdString() = jsnAnswer.value("name");
    //std::stringstream(substr.toStdString()) >> jsnAnswer;


//    str = "Data to be extracted from JSON name field ";
//    GUI_Write_To_Log(value, str);
   // str = QString(jsnAnswer["name"]);
  // GUI_Write_To_Log(value, jsnAnswer["name"]);

//  GUI_Write_To_Log(value, "\n");
//  GUI_Write_To_Log(value, "Received data are as follows !");
//  GUI_Write_To_Log(value, "\n");

    str = substr; // jsnData; // Но тут еще надо обрезать HTTP-заголовки. ОБрезаем все до первого символа '{'
//  GUI_Write_To_Log(value, str);
 // str = "{\" rc\": 0, \"info\": \"success\",\"name\": \"getposition\", \"data\": {\"detected\": true, \"x\": -15.0, \"y\": -60.0, \"width\": 113, \"height\": 108, \"err_angle\": -1.38117702629722, \"distance\": 209.21150512634233}}";
  //Assign the json text to a JSON object
  jsnDoc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
//  if(jsnDoc.isObject() == false) GUI_Write_To_Log(value,"It is not a JSON object");
  if(jsonError.error != QJsonParseError::NoError){
          str = "Error while parcing: "; str += jsonError.errorString();
          GUI_Write_To_Log(value, str);
          return 0.0;
   }       //return;

  //Get the main JSON object and get the data in it
  jsnObj = jsnDoc.object();


//  str = "JSON data :\n";
//  QJsonValue name= jsnObj.value("name");

//  Just for information, so no needed in production
//  GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!!!!! Go to recursive parsing !!!!!!!!!!!!!!!!!!!!");
  traversJson(jsnObj);
//  GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!!!!! Get back from recursive parsing !!!!!!!!!!!!!!!!!!!!");

//  // Парсинг JSON закончили, получили глобальную переменную  jsndataObj - это объект "data" : {}. Извлекаем из него данные.
// Проверяем, что "detected": true
  //this->DETECTED = jsnObj.value("detected").toBool();
//  str = "Detected value after parcing is "; str += QVariant(this->DETECTED).toString();
//  GUI_Write_To_Log(value, str);

  if (this->DETECTED != true){
      jsnStore->isAnyActionRunning = false;

      str = "There is NO DETECTION !!!";
      GUI_Write_To_Log(value, str);
      jsnActionAnswer["name"] =  "get_box"; //jsnStore->jsnActionGetBox.value("name");
      jsnActionAnswer["rc"] = jsnStore->AC_Launch_RC_FAILURE;
      jsnActionAnswer["info"] = jsnStore->DEV_ACTION_INFO_NODETECT;

      jsnDoc = QJsonDocument(jsnActionAnswer);
      str = jsnDoc.toJson(QJsonDocument::Compact);
      // Даем быстрый ответ
      QMetaObject::invokeMethod(this->newPtrTcpClient, "Data_2_TcpClient_Slot",
                                Qt::QueuedConnection,
                                Q_ARG(QString, str),
                                Q_ARG(qintptr, tcpSocketNumber));
      // А теперь следует задать значение "action_list": []
      // Меняем только "rc"
      jsnActionAnswer["rc"] = JsonInfo::AC_Launch_RC_DONE; // Чтобы экшен мог запуститься
//        mainjsnObj["name"] = jsnStore->jsnActionGetBox.value("name").toString();
//        mainjsnObj["info"] = str;
//        mainjsnObj["state"] = jsnStore->DEV_ACTION_STATE_NODETECT;

      // if this causes dangling pointer, try signal|slot data transfer
      jsnStore->setActionData(jsnActionAnswer);
      // emit SetActionData_Signal(mainjsnObj);

      QJsonObject headStatus = {
          {"rc", RC_SUCCESS}, //RC_SUCCESS
          {"state", "Waiting"},
          {"info", "NO DETECTION"}
      };

      jsnStore->setJsnHeadStatus(headStatus);

      jsnActionAnswer = jsnStore->returnJsnActionGetBox();
      jsnDoc = QJsonDocument(jsnActionAnswer);
      str = jsnDoc.toJson(QJsonDocument::Compact);
      str.insert(0, "New value of get_box :\n");
      GUI_Write_To_Log(value,str);

      str = " The value of <jsnStore->isAnyActionRunning> is ";
      str  += QVariant(jsnStore->isAnyActionRunning).toString();
      GUI_Write_To_Log(value,str);

      str = " The value of <DETECTED> is ";
      str  += QVariant(this->DETECTED).toString();
      GUI_Write_To_Log(value,str);


      return 0.0;


  }


    // Вот тут мы откуда-то знаем, что есть ключ "distance"... А если его нет ? надо допиливать...
    // Выведем весь jsndataObj
    double cvdistance = jsndataObj.value("distance").toDouble();
    str = "Got distance value as double : ";
    str += QString::number(cvdistance);

    GUI_Write_To_Log(value, str);

    return cvdistance;
} // parseJSON
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int MainProcess::getIndexCommand(QString myCommand, QList<QString> theList)
{
    bool matched = false;
    int i = 0;
    QString message, str ;
    message = myCommand;
    int value = 0x3355;

    while (!matched and i< theList.size()){
        if(message == theList.at(i)) {
            matched = true;
//            qDebug() << value << "Index is " << i;
        }

        ++i;
    }

    if (!matched) {
        str = "There is now any Matching in command list !!! Unknown command";
        GUI_Write_To_Log(value, str);
        return -1;
    }
    i--;
//    qDebug() << value << "Index value is" << i;
//    if (i>=0) {qDebug() << "Matched string is " << theList.at(i);}

    return i;

}// getIndexCommand
//+++++++++++++++++++++++++++++++++++++++++++++++
// Выясняем текущее состояние экшена перед запуском.
// И если еще не запущен, то меняем состояние на "уже запущен"


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



//+++++++++++++++++
void MainProcess::newConnection_Slot()
{
    GUI_Write_To_Log(0xf01f, "There is new TCP connection !!!");
}
//++++++++++++++++++++++++++
// Пишем в лог сообщение, что комплексная команда (например,взять кубик в одной точке и положить в другую) выполнена.
// А также это индикатор, что команда выполнена и можно, например, отправить эти данные вебсерверу.
void MainProcess::Moving_Done_Slot()
{
    QString str, myname;
    int value = 0xFAAA;
    QMutexLocker locker(&mutex02);

//    GUI_Write_To_Log(value, "Demo cycle finished !!!");
    // Меняем статус, теперь "done"
//    std::cout<<"Set DONE to Robot!" << std::endl;
//    Robot->SetCurrentStatus ("done");

//    GUI_Write_To_Log(value, "Now robot status is !!! DONE !!!");


//    currentCommandIndex = -1; // Нет текущих команд манипулятора

//    str = "Current json object name is ";
//    myname = mainjsnObj.value("name").toString();
//    str += myname;
//    str += "\n\nAction "; str += myname; str += " is finished !!!\n";
//    GUI_Write_To_Log(value, str);
    // Передаем QJsonObject в свой класс и там все меняем через replace...
    // А, по идее, можно данные об объекте извлекать из "action_list": []
    jsnStore->setActionDone(mainjsnObj);

    // Меняем значение.
    jsnStore->isAnyActionRunning = false;

    QJsonObject headStatus = {
        {"rc", RC_WAIT}, //RC_SUCCESS
        {"state", "Wait"},
        {"info", "Waiting for command"}
    };

    jsnStore->setJsnHeadStatus(headStatus);



//    QString str = "Current active command is ";
//    str += Robot->active_command;

//    GUI_Write_To_Log(value, str);

    //Тут тоже надо через switch...
    // Определяем индекс команды в списке MainProcess::tcpCommand
    // Уже нет. Делаем через mainjsnObj
//    int comIndex = getIndexCommand(Robot->active_command, tcpCommand);

//    switch (comIndex) {

//        case 1: //"get_box"  - это экшен (к вопросу о типе)
//                // Меняем статус
//                Robot->getbox_Action.rc = -4;
//        break;

//        }// switch (comIndex)





//    if (Robot->active_command == "setservos=") {
//        Robot->ready_Action.rc = -4; //"Ожидание"
//        GUI_Write_To_Log(value, "The setservos= RC-value have changed");
//        GUI_Write_To_Log(value, "setservos= operation is finished !");
//    }




    // И вот тут тоже можно искать имя команды по индексу и менять данные структур соответственно.
    // Сделано выше. Через jsnStore->setActionDone(mainjsnObj);


}
//++++++++++++++++++++++++++


//+++++++++++++++++
void MainProcess::server_New_Connect_Slot()
{
    ;
}
//++++++++++++++++++++++++++ Слот сигнала Connected()
// Как только socket переходит в статус connected, получаем соответствующий сигнал и тут его обрабатываем
// Формируем HTTP-запрос в CV, отправляем его в CV
void MainProcess::onSocketConnected_Slot()
{
 //in.setDevice(socketCV);
// QString str = "CV connection established";
// int value = 0x7777;
// GUI_Write_To_Log(value, str);

// str = "Current socket state is ";

// if (socketCV->state() == QTcpSocket::ConnectedState){str += " Connected State";}
// else {str += " Some OTHER than Connected State !!!!";}

//    GUI_Write_To_Log(value, str);

// A read buffer size of 0 (the default) means that the buffer has no size limit, ensuring that no data is lost.
//    str = "###################### Current TCPSocket BUFFER SIZE is ";
//    str += QString::number(socketCV->readBufferSize());
//    GUI_Write_To_Log(value, str);



 // А вот теперь готовим команду "/service?name=getposition"
 QString request = "GET ";
 request += "/service?name=getposition";
 request += " HTTP/1.1";
 request += "\r\nHost: ";
 request += CVDev_IP; request+=":"; request += QString::number(CVDev_Port); request+="\r\n";//"192.168.1.201:5001\r\n";
 request += "Accept: */*\r\n";
// request += "Access-Control-Allow-Origin: *\r\n";

 request += "content-type: application/json\r\n";
 request += "Access-Control-Allow-Origin: *\r\n";
 request += "\r\n";

// request += "";

// GUI_Write_To_Log(0xfefe, "The following Data is going to be sent to CV :");
// GUI_Write_To_Log(0xfefe, request.toUtf8());
 socketCV->write(request.toUtf8());

 // Запрос серверу отправили.
 // Ответ от сервера в слоте &MainProcess::CV_NEW_onReadyRead_Slot

 // Дублирование ???
// mainjsnObj = jsnStore->returnJsnActionGetBox();
// jsnStore->isAnyActionRunning = true;

// QString mystr = "onSocketConnected_Slot Pointer to SENDER QObject is ";
// mystr += pointer_to_qstring(this->ptrTcpClient);
// GUI_Write_To_Log(0x9999, mystr);


}
//+++++++++++++++++++++++++++++++++++
// СЛот сигнала QIODevice::readyRead()
// Cчитываем из сокета ответ от камеры CV - это JSON-объект.
// Парсим JSON-ответ, ищем данные о состоянии CV - детекция, расстояние и т.д.
void MainProcess::CV_onReadyRead_Slot()
{

    /*
     * Now, TCP is based on sending a stream of data, so we cannot expect to get the entire fortune in one go. Especially on a slow network,
     * the data can be received in several small fragments. QTcpSocket buffers up all incoming data and emits readyRead() for every new block that arrives,
     * and it is our job to ensure that we have received all the data we need before we start parsing.
     */
    int value = 0xeeee;


    QString nextTcpdata, str;

int befbytes = socketCV->bytesAvailable();
    nextTcpdata = socketCV->readAll();
int afterbytes = socketCV->bytesAvailable();

str = "Bytes before reading "; str += QString::number(befbytes); GUI_Write_To_Log(value, str);

str = "Bytes after reading  "; str += QString::number(afterbytes); GUI_Write_To_Log(value, str);


    if (nextTcpdata.contains("HTTP/1.0 200 OK")) return;

    if (nextTcpdata == currentTcpdata){

        GUI_Write_To_Log(value, "Data still not readed");
        QTimer::singleShot(0, this, &MainProcess::request_CV);
        return;
    }



    currentTcpdata = nextTcpdata;

    //Чтение информации из сокета и вывод в консоль

//    QByteArray qbmessage;
//    qbmessage = socketCV->readAll();
//    qDebug() << qbmessage;
//   // qDebug() << "!!!!!!!!!!!!!!!!!!!!!11 Get Data FROM TCP SOCKET !!!!!!!!!!!!!!!!!!!1";

    //Парсим команду.
    QString message, substr;
   //message = QString(qbmessage);
    message = nextTcpdata;

   // int sPosition, ePosition; // Индекс строки run в запросе.
   // sPosition = message.indexOf("/run?cmd=");

//   QString  wrong_mess = "/favicon.ico HTTP/1.1";

//    if (!message.contains (wrong_mess))
//    {
        GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!! There are some  data from CV device !!!!!!!!!!!!!!!!!!!!");
        GUI_Write_To_Log(value, message);

        // Дальше надо парсить JSON формат

        int sPosition, ePosition; // Индекс строки run в запросе.
        sPosition = message.indexOf("distance");
        if (sPosition <0) return; // Когда сообщение приходит  частями и в этой части нет слова distance, нам такакя часть неинтересна.

        sPosition += 11;
        ePosition = message.indexOf("}", sPosition);
        substr = message.mid(sPosition, (ePosition - sPosition));

        str = "!!!!!!!!!!!!!!!!! The distance is : ";
        str += substr; str += " mm";

        GUI_Write_To_Log(value, str);

        double cvdistance = substr.toDouble();
        str = "!!!!!!!!!!!!!!!!! The distance as double value : ";
        substr =  QString::number(cvdistance);
        str += substr;
        GUI_Write_To_Log(value, str);

        int cvd = round(cvdistance);
        // Получили значение с точностью до 1мм, а нам надо округлить до 10мм.

        // Теперь сопоставляем значение cvd с числами в массиве
        str = "!!!!!!!!!!!!!!!!! The distance as int value : ";
         substr =  QString::number(cvd);
        str += substr;
        GUI_Write_To_Log(value, str);

        int rDistance = my_round(cvd);
        str = "!!!!!!!!!!!!!!!!! The distance as rounded to closest 10x int value : ";
        substr =  QString::number(rDistance);
        str += substr;
        GUI_Write_To_Log(value, str);

        unsigned char *arrPtr;
        arrPtr = mob_parking_position;

        // Выбираем массив углов через switch, потом попробуем через словарь, т.е. ключ - значение, где значением будет массив
        switch (rDistance)
        {
       // unsigned char ptr;

            case 110: arrPtr = mob_pos_11; break;
            case 120: arrPtr = mob_pos_12; break;
            case 130: arrPtr = mob_pos_13; break;
            case 140: arrPtr = mob_pos_14; break;
            case 150: arrPtr = mob_pos_15; break;
            case 160: arrPtr = mob_pos_16; break;
            case 170: arrPtr = mob_pos_17; break;
            case 180: arrPtr = mob_pos_18; GUI_Write_To_Log(value, "!!!!! Matched Go to position !!!!");break;
            case 190: arrPtr = mob_pos_19; break;
            case 200: arrPtr = mob_pos_20; break;
            case 210: arrPtr = mob_pos_21; break;
            case 220: arrPtr = mob_pos_21; break;
            case 230: arrPtr = mob_pos_23; break;


          default:
            substr =  QString::number(rDistance);
            str = "!!!!! Unrecognized position, Go to Parking !!!! ";
            str += substr;
            GUI_Write_To_Log(value, str);
            arrPtr = mob_parking_position;
          break;

        }// switch (rDistance)

        memcpy(Servos, arrPtr, DOF);
        this->send_Data(LASTONE);

        str =  "!!!!! position "; str += QString::number(cvd); str += "mm !!!!";

        GUI_Write_To_Log(value, str);


        //Отсоединение от удаленнного сокета
        //socketCV->disconnectFromHost();

}
//++++++++++++++++++++++++++++++++++++++
// Слот обработки сигнала Disconnected
void MainProcess::CV_onDisconnected()
{
    int value = 0xf1f1;
    GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!! Connection closed to CV device !!!!!!!!!!!!!!!!!!!!");

  //  socketCV->close();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
// Слот сигнала &QIODevice::readyRead, приходит от CV
// Добавляем парсинг JSON-данных от CV
// Также меняем значение mainjsnObj
void MainProcess::CV_NEW_onReadyRead_Slot()
{
    int value = 0xfafa;
    QString message, nextTcpdata, str, substr;

//    QMutexLocker locker(&mutex);

    int befbytes = socketCV->bytesAvailable();

    if (befbytes < 200) {// HTTP/1.0 200 OK == 17 Пришла строка + заголовки, но без тела сообщения(170 символов),
//                          обрабатывать не нужно, выходим.
//        DETECTED = true;
//        GUI_Write_To_Log(value, "Detected [17] is true !!!");
        return;
    }
    nextTcpdata = socketCV->readAll();
    nextTcpdata = nextTcpdata.simplified();


//    GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!! There are some data from CV device !!!!!!!!!!!!!!!!!!!!");
//    GUI_Write_To_Log(value, nextTcpdata);

//    str = "Detected value BEFORE parcing is ";
//    str +=  QVariant(DETECTED).toString();
//    GUI_Write_To_Log(value, str);

    // Запускаю JSON-парсинг
    double cvdistance = parseJSON(nextTcpdata);
//    str = "Bytes readed "; str += QString::number(nextTcpdata.length());
//    GUI_Write_To_Log(value, str);

    // Вот тут detected должно быть установлено !!!
//    if (this->DETECTED != true){
//        jsnStore->isAnyActionRunning = false;

//        str = "There is NO DETECTION !!!";
//        GUI_Write_To_Log(value, str);
//        jsnActionAnswer["name"] =  "get_box"; //jsnStore->jsnActionGetBox.value("name");
//        jsnActionAnswer["rc"] = jsnStore->AC_Launch_RC_FAILURE;
//        jsnActionAnswer["info"] = jsnStore->DEV_ACTION_INFO_NODETECT;

//        jsnDoc = QJsonDocument(jsnActionAnswer);
//        str = jsnDoc.toJson(QJsonDocument::Compact);
//        // Даем быстрый ответ
//        QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot",
//                                  Qt::QueuedConnection,
//                                  Q_ARG(QString, str),
//                                  Q_ARG(qintptr, tcpSocketNumber));
//        // А теперь следует задать значение "action_list": []
//        // Меняем только "rc"
//        jsnActionAnswer["rc"] = JsonInfo::AC_Launch_RC_DONE; // Чтобы экшен мог запуститься
////        mainjsnObj["name"] = jsnStore->jsnActionGetBox.value("name").toString();
////        mainjsnObj["info"] = str;
////        mainjsnObj["state"] = jsnStore->DEV_ACTION_STATE_NODETECT;

//        // if this causes dangling pointer, try signal|slot data transfer
//        jsnStore->setActionData(jsnActionAnswer);
//        // emit SetActionData_Signal(mainjsnObj);

//        QJsonObject headStatus = {
//            {"rc", RC_SUCCESS}, //RC_SUCCESS
//            {"state", "Waiting"},
//            {"info", "NO DETECTION"}
//        };

//        jsnStore->setJsnHeadStatus(headStatus);

//        jsnActionAnswer = jsnStore->returnJsnActionGetBox();
//        jsnDoc = QJsonDocument(jsnActionAnswer);
//        str = jsnDoc.toJson(QJsonDocument::Compact);
//        str.insert(0, "New value of get_box :\n");
//        GUI_Write_To_Log(value,str);

//        str = " The value of <jsnStore->isAnyActionRunning> is ";
//        str  += QVariant(jsnStore->isAnyActionRunning).toString();
//        GUI_Write_To_Log(value,str);

//        str = " The value of <DETECTED> is ";
//        str  += QVariant(this->DETECTED).toString();
//        GUI_Write_To_Log(value,str);


//        return;


//    } // if (this->DETECTED != true)

    if (cvdistance < 1.0) { // Это когда нечитаемый ответ от CV, то в ответ приходит 0.0
//      str = "There is INCORRECT value of distance !!!";
//      GUI_Write_To_Log(value, str);
        jsnStore->isAnyActionRunning = false;

      return;

    }

    cvdistance = 232.24; // Заглушка для тестов

    // Переводим double в int и округляем до ближайшего десятка
    int cvd = round(cvdistance);
    // Получили значение с точностью до 1мм, а нам надо округлить до 10мм. т.к.
    // позиции кубика подобраны с разницей в 1см==10мм
//    str = "The int value of distance is ";
//    substr =  QString::number(cvd);
//    str += substr;
//    GUI_Write_To_Log(value, str);


    unsigned int rDistance = my_round(cvd);
//    str = "!!!!!!!!!!!!!!!!! The distance Is rounded to closest 10x int value : ";
//    substr =  QString::number(rDistance);
//    str += substr;
//    GUI_Write_To_Log(value, str);

    if (rDistance < 110 || rDistance > 270) {
        jsnStore->isAnyActionRunning = false;
        str = "!!!!!!!!!!!!!!!!! The distancCV_NEW_onReadyRead_Slote is out of range !!!!!!!!!! ";
        GUI_Write_To_Log(value, str);
        // Вот тут шлём rc==AC_FAILURE -   "экшен не запустился",
        // ОДнако значение rc ставим
        //Robot->getbox_Action.rc = RC_WAIT; // Ставим экшен RC в "waiting"
        jsnActionAnswer["name"] = "get_box"; // jsnStore->jsnActionGetBox.value("name").toString();
        jsnActionAnswer["rc"]   = jsnStore->AC_Launch_RC_FAILURE;
        jsnActionAnswer["info"] = jsnStore->DEV_ACTION_INFO_OUT;

        jsnDoc = QJsonDocument(jsnActionAnswer);
        str = jsnDoc.toJson(QJsonDocument::Compact);
        //emit Write_2_TcpClient_Signal (jsnDoc.toJson(QJsonDocument::Compact), this->tcpSocketNumber);
        QMetaObject::invokeMethod(this->newPtrTcpClient, "Data_2_TcpClient_Slot",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, str),
                                  Q_ARG(qintptr, tcpSocketNumber));

        // А теперь следует задать значение "action_list": []

        mainjsnObj["rc"] = JsonInfo::AC_Launch_RC_DONE; // Чтобы экшен мог запуститься
        //mainjsnObj["name"] = jsnStore->jsnActionGetBox.value("name").toString();
        mainjsnObj["info"] = str;
        mainjsnObj["state"] = jsnStore->DEV_ACTION_STATE_FAIL;

        // if this causes dangling pointer, try signal|slot data transfer
        jsnStore->setActionData(mainjsnObj);


        return;
    }

    // Вот тут по уму надо передеать rDistance в класс cvdevice;
    // Но пока заколхозим глобальную переменную.
    this->CVDistance = rDistance;
    str = "Saved distance in GLOBAL variable, the value is ";
    str += QString::number(this->CVDistance);
    GUI_Write_To_Log(value, str);


//    str = "Current socket number value is ";
//    str += QString::number(ptrTcpClient);
//    GUI_Write_To_Log(value, str);


    // Быстрый ответ если все нормально (наконец-то :)
    jsnStore->isAnyActionRunning = true;
    //QJsonObject theObj;
    mainjsnObj["name"]  =  "get_box"; //jsnStore->jsnActionGetBox.value("name").toString();
    mainjsnObj["rc"]    = jsnStore->AC_Launch_RC_RUNNING;
    mainjsnObj["info"]  = "Get box by manipulator ARM";// jsnStore->DEV_ACTION_INFO_;
    mainjsnObj["state"] = jsnStore->DEV_ACTION_STATE_RUN;

    //Сравнить с SerialRobot, тут не всегда успевает.
    // Возможно надо заменить глобальный mainjsnObj на локальный, например QJsonObject theObj
    // Хотя изначально так и было...
    jsnStore->setActionData(mainjsnObj);


//  Если приходит команда "get_box", и сразу за ней - другая/следующая, то здесь видим,
//  что this->ptrTcpClient имеет значение уже как раз другой, т.е. следующей команды.
//  А нужный нам адрес объекта, пославшего команду, как раз в this->newPtrTcpClient
//  Вот ему, т.е. this->newPtrTcpClient, и отправляем ответ.

//    QString mystr = "CV_NEW_onReadyRead_Slot Pointer to SENDER QObject is ";
//    mystr += pointer_to_qstring(this->ptrTcpClient);
//    GUI_Write_To_Log(value, mystr);

//    mystr = "CV_NEW_onReadyRead_Slot COPY Pointer to SENDER QObject is ";
//    mystr += pointer_to_qstring(this->newPtrTcpClient);
//    GUI_Write_To_Log(value, mystr);

    jsnDoc = QJsonDocument(mainjsnObj);
    str = jsnDoc.toJson(QJsonDocument::Compact);
    //emit Write_2_TcpClient_Signal (jsnDoc.toJson(QJsonDocument::Compact), this->tcpSocketNumber);
    QMetaObject::invokeMethod(this->newPtrTcpClient, "Data_2_TcpClient_Slot",
                              Qt::QueuedConnection,
                              Q_ARG(QString, str),
                              Q_ARG(qintptr, tcpSocketNumber));

// jsnStore->setActionData(theObj); // Так работает на intel
//    emit SetActionData_Signal(mainjsnObj);

    // Запускаем захват объекта.  Теперь это значение distance отправляем в ф-цию GetBox

//    qDebug() << "$$$$$$$$$$$$$$ CV_NEW_onReadyRead_Slot thread " << QThread::currentThread();
//    qDebug() << "$$$$$$$$$$$$$$ CV_NEW_onReadyRead_Slot ptrSender " << this->ptrTcpClient;
//    qDebug() << "$$$$$$$$$$$$$$ CV_NEW_onReadyRead_Slot newPtrSender " << this->newPtrTcpClient;

    this->GetBox(CVDistance);

} // CV_NEW_onReadyRead_Slot
//+++++++++++++++++++++++++++++++++++++++++
// catch the cube
void MainProcess::GetBox(unsigned int distance)
{
    QString str;
    int value = 0xA9B9;
    unsigned char *arrPtr = mob_parking_position;

// Проверяем подключены ли приводы.

//    str = "I'm in GetBox !!! Current distance is ";
//    str += QString::number(distance);
    //GUI_Write_To_Log(value, str);
    // Пишем в лог Serial порта, чтобы не сливалось со статусами.
//    Robot->Write_To_Log(value,str);


//    str = "Inside GetBox the Current active command is ";
//    str += Robot->active_command;
//    GUI_Write_To_Log(value, str);

// 1st - "unlock"
    Servos[0]=0;
    this->send_Data(NOT_LAST); //NOT_LAST LASTONE


    // Выбираем массив углов через switch, потом попробуем через словарь, т.е. ключ - значение, где значением будет массив
    switch (distance)
    {
   // unsigned char ptr;



    case 110: arrPtr = mob_2_pos_11; break;
    case 120: arrPtr = mob_2_pos_12; break;
    case 130: arrPtr = mob_2_pos_13; break;
    case 140: arrPtr = mob_2_pos_14; break;
    case 150: arrPtr = mob_2_pos_15; break;
    case 160: arrPtr = mob_2_pos_16; break;
    case 170: arrPtr = mob_2_pos_17; break;
    case 180: arrPtr = mob_2_pos_18; break;
    case 190: arrPtr = mob_2_pos_19; break;
    case 200: arrPtr = mob_2_pos_20; break;
    case 210: arrPtr = mob_2_pos_21; break;
    //case 220: arrPtr = mob_2_pos_21; break;
    //case 230: arrPtr = mob_2_pos_23; break;
    //++++++++++++++++++++++++++++++++++++++
    case 220: arrPtr = mob_3_pos_22; break;
    case 230: arrPtr = mob_3_pos_22; break;
    case 240: arrPtr = mob_3_pos_24; break;
    case 250: arrPtr = mob_3_pos_25; break;


      default:
        GUI_Write_To_Log(value, "!!!!! Unrecognized position, Go to Parking !!!!");
        arrPtr = mob_parking_position; break;
        // Если позиция за рамками руки, значит что-то с распознаванием. Переводим в статус "waiting"
        //Robot->getbox_Action.rc = RC_WAIT; //"waiting"
      break;

    } //switch (distance)

    //+++++++++++++++++++++++ Опустить хват для взятия кубика
     memcpy(Servos, arrPtr, DOF);
     this->send_Data(NOT_LAST);  // Уже не LASTONE
/*
 Это мы только опустили захват в нужную точку.
 Далее нужно следующее :

 - Закрыть захват
 - Поднять в позицию "Have_Cube"
 - Жать дальнейший указаний от ЦУП
*/

     //++++++++++++++++++++++ Сжать захват
//    str = "BEFORE CATCH Servo[0] is ";
//    str += QString::number(Servos[0]);
//    GUI_Write_To_Log(value, str);

//     quint8 FULL_OPENED, FULL_CLOSED;
//     FULL_CLOSED = 90;
//     FULL_OPENED = 35;
     Servos[0]=FULL_CLOSED;
     this->send_Data(LASTONE);  // Уже не LASTONE NOT_LAST


     //++++++++++++++++++++++ в позицию formoving, теперь последняя
//     memcpy(Servos, mob_moving_position, DOF);
//     this->send_Data(LASTONE);

//    str = "!!!!!!!!!!!!!!!! The command Action \"";
//    str += Robot->getbox_Action.name; str += "\"";
//    str += " has been sent to manipulator";
//    GUI_Write_To_Log(value, str);
    // А теперь копируем структуру (Robot->getbox_Action) в json



}// CV_NEW_onReadyRead_Slot()
//++++++++++++++++++++++++++++++++++++++
// Слот обработки сигнала data_from_CVDevice_Signal
void MainProcess::data_from_CVDevice_Slot(QString message)
{
    int value = 0xCCCC;
    QString str = "Data from CV Device :";
    str += message;
    GUI_Write_To_Log(value, str);
    // Парсинг данных, а это json...



}
//++++++++++++++++++++++++++++++++++++++
void MainProcess::LogFile_Open(QString fname)
{
    mobWebLogFile.setFileName(fname);
    mobWebLogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
}// LogFile_Open


//+++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainProcess::update_Servos_from_position(unsigned char *pos)
{
    for (int i =0; i< DOF; i++) { Servos[i] = pos[i]; }

}
//+++++++++++++++++++++++++++++
QString MainProcess::pointer_to_qstring(void *ptr)
{
    char temp[16];
    sprintf(temp, "%08p", ptr);
    return QString(static_cast<char *>(temp));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainProcess::returnActionLaunch(QJsonObject &theObj, QObject* theSender)
{
    QString str;
    int value = 0x3939;
//    str = "I'm in returnActionLaunch";
//    GUI_Write_To_Log(value, str);

    if (!Robot->SerialIsOpened){
       jsnStore->isAnyActionRunning = false;
       theObj["rc"] = RC_FAIL;
       theObj["info"] = str;
       theObj["state"] = jsnStore->DEV_ACTION_STATE_FAIL;
       jsnStore->setActionData(theObj);
       str = "Serial port is UNAVAILABLE !!! CAN'T move the ARM !!! Action is FAILED !!!";
       GUI_Write_To_Log(value, str);
       return;
   }




    jsnStore->isAnyActionRunning = true;
    theObj["rc"] = RC_SUCCESS;
    theObj["rc"] = jsnStore->AC_Launch_RC_RUNNING;//RC_SUCCESS; // Now state "inprogress" //theObj
    theObj["state"] = jsnStore->DEV_ACTION_STATE_RUN;

    jsnStore->setActionData(theObj);


    str = QJsonDocument(theObj).toJson(QJsonDocument::Indented);
    QMetaObject::invokeMethod(theSender, "Data_2_TcpClient_Slot",
                              Qt::QueuedConnection,
                              Q_ARG(QString, str),
                              Q_ARG(qintptr, tcpSocketNumber));

    QJsonObject headStatus = {
        {"rc", RC_SUCCESS}, //RC_SUCCESS
        {"state", "Running"},
        {"info", "Action performing"}
    };

    jsnStore->setJsnHeadStatus(headStatus);



    // И вот тут проверяем подключены ли сервоприводы.
    // Это время затратно.


}


