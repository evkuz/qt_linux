#include "mainprocess.h"
//#include "ui_MainProcess.h"
#include "positions.h"
//#include "hiwonder.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//#include <QtGui>
#include <chrono>
#include <thread>
//(QObject *parent)

MainProcess::MainProcess(QObject *parent)
    : QObject(parent)
    , readSocket("../iqr.socket")

{
    int value = 0x0000;
    QString str, S3;

    DETECTED = false;
    new_get_request = false;
    thread_counter = 0;

    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    //QByteArray ba = target_name.toLocal8Bit();
    //g/const char *c_str = ba.data();
    //printf("Appname : %s", c_str);

    // Инициализируем все json-статусы
    jsnStore = new JsonInfo();
    mainjsnObj = jsnStore->returnJsnActionReset(); //set global value
//    jsnStore->init_json();
// Сейчас объект jsnObj хранит данные из jsnOB1.
// Получаем объект jsnObj через returnJsonObject()

//    str = QJsonDocument(jsnStore->returnJsonObject()).toJson(QJsonDocument::Indented);
//    str = QJsonDocument(jsnStore->jsnActionList).toJson(QJsonDocument::Indented);
//    QString S3 = "JSON object 1 : \n";
//    S3 += str;
    jsnStore->createActionList();
//    str = jsnStore->returnJsnData();
//    GUI_Write_To_Log(value, str);

//   traversJson(jsnStore->jsnHeadStatus);

    Robot = new HiWonder(); // Без этого будет "The program has unexpectedly finished", хотя в начале говорила, что это ambiguous

    Robot->Log_File_Open(Log_File_Name);
    this->LogFile_Open(HWR_WEB_LOGFILE);
    str = "The application \"";  str +=target_name; str += "\"";
    Robot->Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));

    // write to the same faile &Robot->LogFile
    GUI_Write_To_Log(0000, "Going to Start QTcpSErver");
    if (server.isListening ()) {

        str = "Listening on address "; str += server.serverAddress().toString();
        str += " and port "; str += QString::number(server.serverPort());//QString::number(server.tcpport);

        GUI_Write_To_Log(0000, str);
    }

    qDebug() << QThread::idealThreadCount() << " - the ideal number of threads that can be run on the system.";
//    Robot->Write_To_Log(value, S3);

//  s1 хранит данные данные об одном экшене : nlohmann::jsnActionTST, преобразуем его в QJsonObject jsnObj2
//    str = QString::fromStdString(jsnStore->s1); // json-овтет остальное
//    str = QJsonDocument(jsnStore->returnJsonObject2()).toJson(QJsonDocument::Indented);
//    S3 = "JSON ACTION object : \n";
//    S3 += str;
//    Robot->Write_To_Log(value, S3);

    // Inititalize actions, each as QJsonObject.
    // Initialize actions_list as empty QJsonArray
    //jsnStore->init_actions(); // already in jsnStore

    str = QJsonDocument(jsnStore->jsnActionList).toJson(QJsonDocument::Indented);
    S3 = "JSON ACTIONLIST : \n";
    S3 += str;
    Robot->Write_To_Log(value, S3);

    // Вызываем объединение json-объектов по имени экшена. Т.е. к шапке добавляем данные
    // конкретного экшена. В данном случае эти данные заранее в переменной s1
//     jsnStore->makeJson_Answer_Slot("clamp"); // Получаем строку из 2 объектов в переменной jsnData
//     str = jsnStore->jsnData;
//     str = jsnStore->returnJsnData();
//     S3 = "JSON MERGED object : \n";
//     S3 += str;
//     Robot->Write_To_Log(value, S3);

//    str = "ActionList size is ";
//    str += QString::number(jsnStore->jsnActionList.size());
//    str += " elements";
//    Robot->Write_To_Log(value, str);


//    traversJson(jsnStore->jsnObj);
// Тут мы знаем имя переменной. Или достаточно имя метода...
//     traversJson(jsnStore->returnJsonObject());
    //+++++++++++++++++++++++++++++++++  signal/slot of Get Request to webserver
    // Отправка данных от сервера клиенту (в  ЦУП)
    connect(this, &MainProcess::Write_2_TcpClient_Signal, &server, &QSimpleServer::Write_2_TcpClient_SLot, Qt::QueuedConnection); // works ?

    // Чтение данных от клиента серверу (из ЦУП)
    //connect(&server, SIGNAL(Info_2_Log_Signal(QString)), this, SLOT(Info_2_Log_Slot(QString))); // Not working
    connect(&server, &QSimpleServer::Data_From_TcpClient_Signal, this, &MainProcess::Data_From_TcpClient_Slot, Qt::QueuedConnection);

    //################### SERIAL SIGNAL/SLOTS ############################
    connect( this, &MainProcess::Open_Port_Signal, Robot, &HiWonder::Open_Port_Slot);
    connect( &Robot->serial, &QSerialPort::readyRead, Robot, &HiWonder::ReadFromSerial_Slot, Qt::QueuedConnection);  //&QSerialPort::
    connect(&Robot->serial, &QSerialPort::errorOccurred, Robot, &HiWonder::SerialError_slot, Qt::QueuedConnection);
    //#################### Signal to web-server
    connect( Robot, &HiWonder::Moving_Done_Signal, this, &MainProcess::Moving_Done_Slot);
//    connect()
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

//    str = "Current Servo values are : ";
//    str += QString::number(Servos[0]);
//    GUI_Write_To_Log(value, str);

    int OKay = Robot->Open_Port_Slot("ttyUSB0");
    if (!OKay) { //!Robot->SerialIsOpened
        Robot->current_st_index = 4;

       OKay = Robot->Open_Port_Slot("ttyUSB1");

    } // Robot->GetCurrentStatus() = statuslst.at(4)

    if (!OKay){
                GUI_Write_To_Log(value, "SerialPort  PROBLEM !!!");
                // state = "fail"
                jsnStore->setHeadStatusFail();
                Robot->SetCurrentStatus("Fail");
                jsnStore->isAnyActionRunning = false;
                qDebug() << "SerialPort  PROBLEM !!!";

                // ТОгда таймер на переоткрытие порта пускаем ???
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
    delete jsnStore;


}
//+++++++++++++++++++++++++++++++++++++++
void MainProcess::GUI_Write_To_Log (int value, QString log_message)
{
//    QMutexLocker locker(&mutex);
    QDateTime curdate ;
    QTextStream uin(&this->hwrWebLogFile); //&Robot->LogFile

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
    int value = 0xf014;
    DetectorState state;
    QString str, fstr;
    str = "on_trainButton_Clicked";
    GUI_Write_To_Log(value, str);
    fstr = "";
//Сразу открываем захват
//    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
//    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
    Servos[0]=0;

    str = "Before Read SOCket";
    GUI_Write_To_Log(value, str);

    // Нужна проверка, запущен ли сервис robot.hiwonder.service
    if (readSocket.GetState(&state) == 0)
      {
        str = "Inside IF !!!!!!!!!!!!!";
        GUI_Write_To_Log(value, str);

        if (state.isDetected){

            str = "Just float values of state : ";
            fstr.setNum(state.objectX);
            str += fstr;
            str += ", ";
            fstr.setNum(state.objectY);
            str += fstr;
            GUI_Write_To_Log(value, str);


            int kf = 100000;
            try_mcinfer(state.objectX*kf, state.objectY*kf); // Тут меняем current_status = "inprogress". Команда 0 - Переместить открытый хват к кубику.
            X = state.objectX;                        //  Хват открывается в процессе движения робота, а не отдельной командой.
            Y = state.objectY;
//            std::cout << "DETECTED: " << state.objectX << " " << state.objectY << std::endl;
            //str += QString::number(state.objectX);
            fstr.setNum(state.objectX);
            str += fstr;
            str += ", ";
            //str += QString::number(state.objectY);
            fstr.setNum(state.objectY);
            str += fstr;
            DETECTED = true;

            mainjsnObj["rc"] = JsonInfo::AC_Launch_RC_RUNNING; // Чтобы экшен мог запуститься
            mainjsnObj["name"] = "start";
            mainjsnObj["info"] = str;
            mainjsnObj["state"] = DEV_ACTION_STATE_RUN;

            jsnStore->setActionData(mainjsnObj); // Переписали данные экшена в массиве jsnstore->jsnObjArray
            str = QJsonDocument(mainjsnObj).toJson(QJsonDocument::Indented);

        } else {
            str = " NOT DETECTED";
            // change status to "NoDetection"
           // mutex.lock();
            GUI_Write_To_Log(value, str);
            //qDebug() << str;
            jsnStore->isAnyActionRunning = false;
            jsnStore->currentStatus.rc = -5; // No Detection
            jsnStore->currentStatus.info = "No Detection";
            jsnStore->currentStatus.state = jsnStore->statuslst.at(jsnStore->INDEX_NODETECTION).toStdString();
            jsnStore->struc_2_jsnObj(); // Копируем данные структуры в ordered_json jsnStatus

            jsnStore->returnJsnStatus(); // Берем данные из ordered_json jsnStatus и передаем в QJsonObject::jsnObj

            jsnStore->setActionStart2NoDetection(); // jsnstor->jsnData changing also and having these last data
            //
//            str = jsnStore->returnJsnData();
            // теперь делаем экшен доступным для перезапуска
            mainjsnObj["rc"] = JsonInfo::AC_Launch_RC_DONE; // Чтобы экшен мог запуститься
            mainjsnObj["name"] = "start";
            mainjsnObj["info"] = str;
            mainjsnObj["state"] = DEV_ACTION_STATE_FAIL;

           // jsnStore->setActionData(mainjsnObj); cause dangling pointer, so commented

            jsnStore->jsnActionStart["rc"] = JsonInfo::AC_Launch_RC_DONE;

//            QString s3 = "NO DETECTION";
            str = "NO DETECTION";
            GUI_Write_To_Log(value, str);
            str = "Now the value of jsnStore->isAnyActionRunning : ";
            str += QVariant(jsnStore->isAnyActionRunning).toString();
            GUI_Write_To_Log(value, str);

            str = jsnStore->returnJsnData();
        }

//       std::cout <<  str.toStdString() << std::endl;
//       Robot->Write_To_Log(value, str);
//       GUI_Write_To_Log(value, str);
    }//if (state.iqsDetected)

    // Ответ на запуск экшена start отправили

    emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);

//В этой точке робот опустился над кубиком, открыл захват.

if (DETECTED)
    {

        QByteArray dd ;
        dd.resize(parcel_size);
        str = "Next movement to robot";
        this->GUI_Write_To_Log (value, str);
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
   //this->update_Servos_from_position(hwr_Start_position);
   memcpy(Servos, hwr_Start_position, 6);
   memcpy(dd.data(), Servos, 6);
   dd.insert(parcel_size-2, 0x30); // Движение "Обратно"
   dd.insert(parcel_size-1, LASTONE);

   // this->ssend_Data(dd); // The last command
   Robot->GoToPosition(dd);

  }// if (DETECTED)

//else {
//    str = "The LAST NO Detection !!!";
//    this->GUI_Write_To_Log (value, str);

//}

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
  jsn_str += Robot->GetCurrentStatus();
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
    int value = 0xEEBB;
    QString str = "I'm in put_box";
    GUI_Write_To_Log(value, str);

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

void MainProcess::traversJson(QJsonObject json_obj)
{
    QString str;
    bool isDetected, boolValue;
    int thevalue = 0x5555;
    foreach(const QString& key, json_obj.keys()) {
        str = "";
        QJsonValue value = json_obj.value(key);
        if(!value.isObject() ){
                      str +=  "Key = "; str += key; str += ", Value = ";
                      // Важно. Сначала нужно привести к типу bool
                      //  QVariant(value).toBool() - не работает
                      if (value.isBool()) {boolValue = value.toBool(); str += QVariant(boolValue).toString();}
                      if (value.isString()) {str += QVariant(value).toString();}
                      if (value.isDouble()) {str += QString::number( QVariant(value).toDouble());}
                      // Хотя у CV в ответе и нет никакого массива, но все же добавим.
                      if (value.isArray()) {
                          QJsonArray jsnArray = value.toArray();
                          // А еще желательно проверить, что у этого value соответствующий key ==  "action_list":
                          // Тут выводим элементы массива, точнее только имена экшенов, т.е. значение name
                          // ... и где ??? Переводим массив в объект и передаем на рекурсию ?
                          if (key == "action_list") {str += "Many actions, look at output above";}
                          str += "All values above";
                          QJsonObject arrObj;
                          for (int i =0; i < jsnArray.size(); ++i){
                              arrObj = jsnArray[i].toObject();
                              traversJson(arrObj);
                          }
                      }


                     // str += value.toString();
                      GUI_Write_To_Log(0x5555, str);
                      str = "";

          //qDebug() << "Key = " << key << ", Value = " << value;
         }
        else{
            // А теперь определяем тип данных поля.
             //jsndataObj = json_obj["data"].toObject(); // Так тоже работает, но уже есть привязка к конкретному случаю.
            QJsonObject jsndataObj;

            jsndataObj = value.toObject(); // В нашем случае объект единственный - "data"
             // check if there is the key "detected"
             if (jsndataObj.contains("detected")){

                 isDetected = jsndataObj.value("detected").toBool();

                 str = "Detected value is ";
                 str += QVariant(isDetected).toString();
                 GUI_Write_To_Log(thevalue, str);

                 if (!isDetected){
                     GUI_Write_To_Log(thevalue, "!!!!!! Exit. Try Again !!!!!!!!!!!");
                     return;
                 }

                 traversJson(jsndataObj);

             } //if (jsndataObj.contains("detected"))

             //traversJson(value.toObject());


        }//else

        GUI_Write_To_Log(0x5555, str);


    }//foreach

}//traverseJson

//+++++++++++++++++++++++++++++++++++++++++++++++++++

int MainProcess::getIndexCommand(QString myCommand, QList<QString> theList)
{
    int i = 0;
    QString message, str ;
    message = myCommand;
    int value = 0x3355;
    str = "getIndex";
    GUI_Write_To_Log(value, str);


    i = theList.indexOf(message);
    if (i == -1){
        str = "There is now any Matching in command list !!! Unknown command";
        GUI_Write_To_Log(value, str);
    }

//    qDebug() << "getIndex Index value is" << i;
//    if (i>=0) {qDebug() << "getIndex Matched string is " << theList.at(i);}

    return i;

}// getIndexCommand
//+++++++++++++++++++++++++++++++++++++++++++++++++++
// - обрабатываем текущий статус экшена
// - запускаем экшен, если надо
void MainProcess::ProcessAction(int indexOfCommand, QJsonObject &theObj)
{
    int value;
    QString str;

    value = 0x1122;
    str = "I'm in  ProcessAction"; GUI_Write_To_Log(value, str);

// Меняем содержмое theObj в зависимости от rc
    str = theObj.value("rc").toString();
    GUI_Write_To_Log(value, str);
    int returnCode = theObj.value("rc").toInt() ;

    switch (returnCode) {

    case 0: //rc==0 (уже запущен)=="Is running" -> Выходим, ничего не меняем
            // Сюда даже не должны попадать, т.к. это отсеивается в tcpparcing.cpp
        str = "Action "; str += theObj.value("name").toString();
        str += " is already running. Should wait it to finish";
        GUI_Write_To_Log(value, str);
        theObj["rc"] = RC_FAIL;
        theObj["state"] = DEV_ACTION_STATE_RUN;
        theObj["info"] = DEV_ACTION_INFO;
    break;

    case -1: // action с таким именем не найден
        str = QJsonDocument(theObj).toJson(QJsonDocument::Indented);
        // Вот это нужно, отправляем ответ на запуск экшена
        emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);
        str.insert(0, "There is wrong action command !!! : \n");
        GUI_Write_To_Log(value, str);
        break;

    case -2: // // action с таким именем не запустился меняем только info
        //str = "Serial port is UNAVAILABLE !!! CAN'T move the ARM !!!";
        str = "Did not started last time. Changing rc to ";
        str += RC_WAIT;
        str += ", so Try again."; //"There is another action running at the moment"
        theObj["info"] = str;
        GUI_Write_To_Log(value, str);
        // change gor next request to be successfull
        theObj["rc"] = RC_WAIT;
        // Так надо ж поменять значение самого экшена в классе JsonInfo !!!
        jsnStore->setActionData(theObj);


//        launchActionAnswer["name"] = theObj.value("name").toString();
//        launchActionAnswer["rc"]   = RC_SUCCESS;
//        launchActionAnswer["info"] = DEV_HEAD_INFO_REQUEST;
        str = QJsonDocument(theObj).toJson(QJsonDocument::Indented);
        // Вот это нужно, отправляем ответ на запуск экшена
        emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);



        break;

    case -3: // (уже запущен) -> Выходим, ничего не меняем
        str = "Action "; str += theObj.value("name").toString();
        str += " is STILL running. Try RESET action or wait for finsih.";
        theObj["info"] = "Already In progress. Try RESET action or wait for finish.";
        break;

    case -4: // Выполнен, т.е. "waiting" ? тогда запускаем

        // 1. Проверяем, открыт ли SerialPort ? Если нет то
        //    - Пишем в лог.
         if (!Robot->SerialIsOpened){
            str = "Serial port is UNAVAILABLE !!! CAN'T move the ARM !!! Action is FAILED !!!";
            theObj["rc"] = RC_FAIL;
            theObj["info"] = str;
            theObj["state"] = DEV_ACTION_STATE_FAIL;
            GUI_Write_To_Log(value, str);
            jsnStore->setActionData(theObj);
        }
        else { // Все нормально, запускаем манипулятор

            jsnStore->isAnyActionRunning = true;
            theObj["rc"] = RC_SUCCESS; // Now state "inprogress" //theObj
            theObj["state"] = DEV_ACTION_STATE_RUN;

            str = QJsonDocument(theObj).toJson(QJsonDocument::Indented);
            // Вот это нужно, отправляем ответ на запуск экшена

            // while detection is not checked don's send that it's ok
            if (indexOfCommand != 5) {
                emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);
                jsnStore->setActionData(theObj);


            }

            QJsonObject headStatus = {
                {"rc", RC_SUCCESS}, //RC_SUCCESS
                {"state", "Running"},
                {"info", "Action performing"}
            };

            jsnStore->setJsnHeadStatus(headStatus);

//            launchActionAnswer["name"] = theObj.value("name").toString();
//            launchActionAnswer["rc"] = RC_SUCCESS;
//            launchActionAnswer["info"] = DEV_HEAD_INFO_REQUEST;
//            str = QJsonDocument(launchActionAnswer).toJson(QJsonDocument::Indented);
//            // Вот это нужно, отправляем ответ на запуск экшена
//            emit Write_2_TcpClient_Signal(str);
//            str.insert(0, "Data supposed to be sent to tcp, but only to log : \n");
//            GUI_Write_To_Log(value, str);

            // Теперь запускаем манипулятор. Определяем команду из списка
            QJsonValue myjsnValue;
            switch (indexOfCommand) {
//            case 1: //reset, у текущего (всех) action делаем rc = -4
//                jsnStore->setActionDone(theObj);
//                break;

            case 2: // clamp
                if(Servos[0]==0) { Servos[0]=90;}
                else {Servos[0]=0;}
                this->send_Data(LASTONE);
                break;
            case 4: // stand up
                memcpy(Servos, hwr_Start_position, DOF);
                this->send_Data(LASTONE);

                str = "STANDUP ACTION, current theObj value :\n";
                str += QJsonDocument(theObj).toJson(QJsonDocument::Indented);
                GUI_Write_To_Log(value,str);

                break;
            case 5: //start
                // - хватаем кубик
                // - встаём в исходную точку
                str = "Going to move  the cube";
                GUI_Write_To_Log(value,str);
                on_trainButton_clicked ();
                break;
            case 6: // "put_box"
                str = "Going to put the box down";
                GUI_Write_To_Log(value,str);
                put_box();
                break;
            case 10: // "lock"
                str = "Going to lock the gripper";
                GUI_Write_To_Log(value,str);
                Servos[0]=90;
                this->send_Data(LASTONE); //NOT_LAST LASTONE

                break;
            case 11: // "unlock"
                str = "Going to unlock the gripper";
                GUI_Write_To_Log(value,str);
                Servos[0]=0;
                this->send_Data(LASTONE); //NOT_LAST LASTONE

                break;


            case 13: //collapse
                memcpy(Servos, collaps, DOF);
                this->send_Data(LASTONE);
//                jsnStore->setActionData(theObj);

//                str = "Collapse data as mainjsnObj after UPDATING !\n";
//                str += QJsonDocument(theObj).toJson(QJsonDocument::Indented);
//                GUI_Write_To_Log(value,str);


//                str = "COLLAPSE jsnACTION() object AFTER theObj RETURN !!!\n";
//                str += QJsonDocument(jsnStore->returnJsnActionCollapse()).toJson(QJsonDocument::Indented);
//                GUI_Write_To_Log(value,str);

                break;
            default: // Нет команды с таким индексом. Дубль из Data_From_TcpClient_Slot, там уже была проверка
                str = "The Command with index ";
                str += QString::number(indexOfCommand);
                str += " is not found";
                GUI_Write_To_Log(value, str);
            break;
            } // switch (indexOfCommand)
        } //else
        break; // case -4:
    default:
        str = "The return Code with index ";
        str += QString::number(returnCode);
        str += " is not found";
        GUI_Write_To_Log(value, str);
    break;


    } // switch (returnCode)

//    str = "PROCESSING ACTION IS FINISHED";
//    GUI_Write_To_Log(value,str);
}// ProcessAction
//+++++++++++++++++++++++++++++++++++++++++++++++++++
// Определяем, Есть ли в данный момент работающий экшен
//
bool MainProcess::isThereActiveAction()
{
    // Перебираем список  actionListp, определяем у кого state == "inprogress"
    // формируем новый список
    // добавляем его в jsnData
    return true ;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
// Открываем отдельный лог-файл для tcp-данных
void MainProcess::LogFile_Open(QString fname)
{
    hwrWebLogFile.setFileName(fname);
    hwrWebLogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++
// Слот сигнала HiWonder::Moving_Done_Signal
// Пишем в лог сообщение, что комплексная команда (например,взять кубик в одной точке и положить в другую)
// А также это индикатор, что команда выполнена и можно, например, отправить эти данные вебсерверу.
void MainProcess::Moving_Done_Slot()
{
    QString str, myname;
    int value = 0xFAAA;
    //int result;
    // Этот слот вызывается сигналом в котором уже QMutexLocker locker(&mutex)
// Однако вызывается из другого класса, а там другой mutex
//QMutexLocker locker(&mutex);
    GUI_Write_To_Log(value, "Demo cycle finished !!!");
    // Меняем статус, теперь "done"
//    std::cout<<"Set DONE to Robot!" << std::endl;
    Robot->SetCurrentStatus ("done");

    str = "Current json object name is ";
    myname = mainjsnObj.value("name").toString();
    //result = QString::compare(myname, Robot->active_command); // 0 -> equals
    str += myname;
    GUI_Write_To_Log(value, str);
    // Вот как бэ не совсем правильно... надо передавать QJsonObject в свой класс и там все менять...
    jsnStore->setActionDone(mainjsnObj);

    // Меняем значение.
    jsnStore->isAnyActionRunning = false;

    QJsonObject headStatus = {
        {"rc", RC_WAIT}, //RC_SUCCESS
        {"state", "Wait"},
        {"info", "Waiting for command"}
    };

    jsnStore->setJsnHeadStatus(headStatus);

    str = "Action "; str += myname; str += " finished !!!";
    GUI_Write_To_Log(value, str);
    //Создаем список активных экшенов, и там не должно быть текущего, т.к. он DONE
//    str = "Now full list of ACTIVE actions : \n";
//    jsnStore->createActionList();
//    str += QJsonDocument(jsnStore->jsnActionList).toJson(QJsonDocument::Indented);
//    GUI_Write_To_Log(value, str);

//    str = "Upon exit from switch jsnActionCollape value : \n";
//    str += QJsonDocument(jsnStore->jsnActionCollapse).toJson(QJsonDocument::Indented);
//    GUI_Write_To_Log(value, str);


}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
// Пришло из GUI-версии. В console-версии тоже нужно.
void MainProcess::update_Servos_from_position(unsigned char *pos)
{
    for (int i =0; i< DOF; i++) { Servos[i] = pos[i]; }

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
