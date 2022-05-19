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
    QString str;

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

    str = QJsonDocument(jsnStore->returnJsonObject()).toJson(QJsonDocument::Indented);
    QString S3 = "JSON object 1 : \n";
    S3 += str;

   // traversJson(jsnStore->jsnObj);

    Robot = new HiWonder(); // Без этого будет "The program has unexpectedly finished", хотя в начале говорила, что это ambiguous

    Robot->Log_File_Open(Log_File_Name);
  //  Robot->Source_Points_File_Open (SOURCE_POINTS_FILE);

    str = "The application \"";  str +=target_name; str += "\"";
    Robot->Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));

    GUI_Write_To_Log(0000, "Going to Start QTcpSErver");
    if (server.isListening ()) {

        str = "Listening on address "; str += server.serverAddress().toString();
        str += " and port "; str += QString::number(server.serverPort());//QString::number(server.tcpport);

        GUI_Write_To_Log(0000, str);
    }

    Robot->Write_To_Log(value, S3);

//  s1 хранит данные данные об одном экшене : nlohmann::jsnActionTST, преобразуем его в QJsonObject jsnObj2
//    str = QString::fromStdString(jsnStore->s1); // json-овтет остальное
    str = QJsonDocument(jsnStore->returnJsonObject2()).toJson(QJsonDocument::Indented);
    S3 = "JSON ACTION object : \n";
    S3 += str;
    Robot->Write_To_Log(value, S3);

    // Inititalize actions, each as QJsonObject.
    // Initialize actions_list as empty QJsonArray
    jsnStore->init_actions();

    // Вызываем объединение json-объектов по имени экшена. Т.е. к шапке добавляем данные
    // конкретного экшена. В данном случае эти данные заранее в переменной s1
     jsnStore->makeJson_Answer_Slot("clamp"); // Получаем строку из 2 объектов в переменной jsnData
//     str = jsnStore->jsnData;
     str = jsnStore->returnJsnData();
     S3 = "JSON MERGED object : \n";
     S3 += str;
     Robot->Write_To_Log(value, S3);





//    traversJson(jsnStore->jsnObj);
// Тут мы знаем имя переменной. Или достаточно имя метода...
     traversJson(jsnStore->returnJsonObject());
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
    int value = 0xf014;
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
            // change status to "NoDetection"
            jsnStore->currentStatus.rc = -5; // No Detection
            jsnStore->currentStatus.info = "No Detection";
            jsnStore->currentStatus.state = jsnStore->statuslst.at(jsnStore->INDEX_NODETECTION).toStdString();
            jsnStore->struc_2_jsnObj(); // Копируем данные структуры в ordered_json jsnStatus

            jsnStore->returnJsnStatus(); // Берем данные из ordered_json jsnStatus и передаем в QJsonObject::jsnObj

            jsnStore->setActionStart2NoDetection();
            str = jsnStore->returnJsnData();
            Write_2_TcpClient_Signal(str);

//            QString s3 = "NO DETECTION";
            str.insert(0, "NO DETECTION \n");
            GUI_Write_To_Log(value, str);
        }

       std::cout <<  str.toStdString() << std::endl;
       Robot->Write_To_Log(value, str);
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
   //this->update_Servos_from_position(hwr_Start_position);
   memcpy(Servos, hwr_Start_position, 6);
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
    bool matched = false;
    int i = 0;
    QString message, str ;
    message = myCommand;
    int sPosition; // Индекс искомой строки в тексте.
    int value = 0x3355;

    while (!matched and i< theList.size()){
        sPosition = message.indexOf(theList.at(i));
        if (sPosition != -1) {matched = true; qDebug() << "Inside sPosition is " << sPosition;}
        ++i;
    }

    if (!matched) {
        str = "There is now any Matching in command list !!! Unknown command";
        GUI_Write_To_Log(value, str);
        return -1;
    }
    qDebug() << "Index value is" << --i;
    qDebug() << "Matched command sPosition is " << sPosition;
    if (i>=0) {qDebug() << "Matched string is " << theList.at(i);}

    return i;

}// getIndexCommand
//+++++++++++++++++++++++++++++++++++++++++++++++++++
// - обрабатываем текущий статус экшена
// - запускаем экшен, если надо
void MainProcess::ProcessAction(int indexOfCommand, QJsonObject theObj)
{
    int value;
    QString str;

    value = 0x1122;
    str = "I'm in  ProcessAction"; GUI_Write_To_Log(value, str);

    // Пока 2 вида обработки - вернуть статус, выполнить экшен.
    switch (indexOfCommand) {
        case 0: // "status"
                str = "Current JSON data of action_command (STATUS) are as follows ";
                //str += Robot->GetCurrentStatus();
                Robot->Write_To_Log(value, str);
                jsnStore->setCurrentAction(Robot->active_command);

                // Если нет активных экшенов, то надо обнулять action_list

                // Выдаем значение ordered_json jsnStatus в виде QJsonObject, дальше нарастим информативность.
                str = QJsonDocument(jsnStore->returnJsnStatus()).toJson(QJsonDocument::Indented);
                emit Write_2_TcpClient_Signal(str);
                GUI_Write_To_Log(value, str);

// Вот теперь отбираем экшены со статусом "inprogress" из списка QList<QString> statuslst
        break;
//        case 2: //clamp

//        break;

//        case 5: // "start"
//                Robot->SetCurrentStatus ("init"); // Перед запуском распознавания

//                on_trainButton_clicked ();
//                str = "Robot current status is ";
//                str += Robot->GetCurrentStatus();
//                Robot->Write_To_Log(value, str);

//        break;

    default :
        str = "There is no command with such the index ";
        str += QString::number(indexOfCommand);
        GUI_Write_To_Log(value, str);
        break;


    } // switch (indexOfCommand)
//++++++++++++++++++++++++++++++++++++++++++++++
    int returnCode = theObj.value("rc").toInt() ;
    switch (returnCode) {

    case 0: // (уже запущен)=="Is running" -> Выходим, ничего не меняем

        //actionName->rc = -3;
        str = "Action "; str += theObj.value("name").toString();
        str += " is already running. Should wait it to finish";
        // Заносим данные в структуру
        //Robot->getbox_Action = {"get_box", -3, "RC=0, Already In progress"};
        GUI_Write_To_Log(value, str);
        theObj["rc"] = RC_FAIL;
        theObj["state"] = DEV_ACTION_STATE_RUN;
        theObj["info"] = DEV_ACTION_INFO;
        //

    break;

    case -2: // меняем только info
        str = "Serial port is UNAVAILABLE !!! CAN'T move the ARM !!!";

//        theObj.value("rc") = -2;
        theObj["info"] = str;
        GUI_Write_To_Log(value, str);

        break;

    case -3: // (уже запущен) -> Выходим, ничего не меняем
        str = "Action "; str += theObj.value("name").toString();
        str += " is STILL running. Try RESET action or wait for finsih.";
        theObj["info"] = "Already In progress";
        break;

    case -4: // Выполнен ? "waiting" ? тогда запускаем

        // 1. Проверяем, открыт ли SerialPort ? Если нет то
        //    - Пишем в лог.
        //    - Ставим actionName->rc == -2: // (не запустился)
        if (!Robot->SerialIsOpened){
            str = "Serial port is UNAVAILABLE !!! CAN'T move the ARM !!! Action is FAILED !!!";

            theObj["rc"] = RC_FAIL;
            theObj["info"] = str;
            theObj["state"] = DEV_ACTION_STATE_FAIL;
            GUI_Write_To_Log(value, str);
        }
        else { // Все нормально, запускаем манипулятор
            theObj["rc"] = RC_SUCCESS; // Now state "inprogress"
            theObj["state"] = DEV_ACTION_STATE_RUN;

            launchActionAnswer["name"] = theObj.value("name");
            launchActionAnswer["rc"] = RC_SUCCESS;
            launchActionAnswer["info"] = DEV_HEAD_INFO_REQUEST;
            str = QJsonDocument(launchActionAnswer).toJson(QJsonDocument::Indented);
            emit Write_2_TcpClient_Signal(str);

//            str = "Action "; str += theObj.value("name").toString();
//            str += " have rc value equals to ";
//            str += QString::number(theObj.value("rc").toInt());
//            str += " and have started";
//            GUI_Write_To_Log(value, str);

//            str = "Action "; str += theObj.value("name").toString();
//            str += " have index "; str += QString::number(indexOfCommand);
//            GUI_Write_To_Log(value, str);

            // Теперь запускаем манипулятор
            // Определяем команду из списка
            switch (indexOfCommand) {
            case 1: //reset, у текущего (всех) action делаем rc = -4
                jsnStore->setActionDone(theObj);
                break;

            case 2: // clamp
                if(Servos[0]==0) { Servos[0]=90;}
                else {Servos[0]=0;}
                this->send_Data(LASTONE);


                break;
            case 5: //start
                 on_trainButton_clicked ();
                break;
            default: // Нет команды с таким индексом. Дубль из Data_From_TcpClient_Slot, там уже была проверка
                str = "The Command with index ";
                str += QString::number(indexOfCommand);
                str += " is not found";
                GUI_Write_To_Log(value, str);


            }
        } //else
    default:
        str = "The return Code with index ";
        str += QString::number(returnCode);
        str += " is not found";
        GUI_Write_To_Log(value, str);



    } // switch (returnCode)
}// ProcessAction

//+++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++ Получили данные (запрос) от клиента. Парсим.
//void MainProcess::Data_From_TcpClient_Slot(QString message)
//{
//    QString str, substr;
//    int value = 0xf00f;
//    new_get_request = true;
//    //str = "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSimpleServer->Info_2_Log_Signal !!!!!!!!!!!!!!!!!!!";
//    str = "From TCP Get new command : "; str += message;
//    GUI_Write_To_Log(0xf00f, str);

//    substr = message;
//    // Определяем индекс команды в списке MainProcess::tcpCommand
//    int comIndex = getIndexCommand(substr, tcpCommand);
//    if (comIndex < 0) {str = "WRONG DATA !!!"; GUI_Write_To_Log(value, str);return;}
//    str = "Index value is "; str += QString::number(comIndex); str += ",\n";
//    str += "List value on that index is \""; str += tcpCommand.at(comIndex); str += "\"";
//    GUI_Write_To_Log(value, str);

//    // So here we've got the index of elemnt representing the command received by TCP
//    // set value of Robot->active_command
//    Robot->active_command = tcpCommand.at(comIndex);
//    str = "Current active command is ";
//    str += Robot->active_command;
//    GUI_Write_To_Log(value, str);

//    jsnStore->action_command = tcpCommand.at(comIndex); // сигнал updateAction_List_Signal


//        if (substr == "start") {
//            Robot->SetCurrentStatus ("init"); // Перед запуском распознавания

//            on_trainButton_clicked ();
//            str = "Robot current status is ";
//            str += Robot->GetCurrentStatus();
//            Robot->Write_To_Log(0xf00F, str);

//         }

//        if (substr == "put_box"){
//            Robot->SetCurrentStatus ("inprogress"); // Перед запуском распознавания
//            put_box();


//        }

//        if (substr == "reset") {
//            if (Robot->GetCurrentStatus () != "wait"){
//                Robot->SetCurrentStatus ("wait");
//                str = "Robot changed status, now it is : ";
//                str += Robot->GetCurrentStatus();

//                GUI_Write_To_Log (value, str);
//                //str = "status_from_robot";
//                str  = "{\n\t\"status\":\"";
//                str += Robot->GetCurrentStatus();
//                str += "\"\n}";
//                emit Write_2_TcpClient_Signal (str);
//            }
//         }
//         ///run?cmd=status&123


//   if (substr == "status") {
//        // response += "{\n\t\"status\":\"";
//        str  = "{\n\t\"status\":\"";
//        str += Robot->GetCurrentStatus();
//        str += "\"\n}";

//        str = "Robot current status is ";
//        str += Robot->GetCurrentStatus();
//        Robot->Write_To_Log(value, str);
//        //+++++++++++++ Now new format of answer +++++++++++++++++++++++++++++++
//       str = Robot->GetCurrentStatus();
//       jsnStore->action_command = Robot->GetCurrentStatus();
//       jsnStore->jsnStatus["state"] = str.toStdString();
//       str = "JSON VALUE OF state is ";
//       std::string s1 = jsnStore->jsnStatus["state"];
//       str += QString::fromStdString(s1);
//       Robot->Write_To_Log(value, str);
//       jsnStore->jsnStatus["rc"] = RC_SUCCESS;
       // Теперь надо данные структуры jsnStore->currentStatus добавить к объекту jsnStore->jsnStatus
       // А значит, надо по значению jsnStore->action_command сформировать соответствующий jsnStore->currentStatus.action_list
       // Значит создаем сигнал/слот для запуска такой процедуры в классе JsonInfo.


//       int indent = 3;
//       std::string s2 = jsnStore->jsnStatus.dump(indent);
//       str = QString::fromStdString(s2);
//       //str = QJsonDocument(jsnStore->jsnStatus).toJson(QJsonDocument::Compact);

//       emit Write_2_TcpClient_Signal (str);
//       QString S3 = "Send JSON data : \n";
//       S3 += str;
//       Robot->Write_To_Log(value, S3);

//       //+++++++++++++++


//   }

//   if (substr == "sit") {
//       // Go to "sit" position
//       QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(sit_down_position), 6);
//       dd.append(0x31); // Движение "Туда"
//       dd.append(LASTONE); // Всегда последнее ?
//       Robot->GoToPosition(dd);//, sit_down_position
//   }//"sit"

//   if (substr == "standup") {
//       // Go to Initial "Start" position
//       QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(hwr_Start_position), 6);
//       dd.append(0x30); // Движение "Обратно"
//       dd.append(LASTONE); // Всегда последнее ?
//       Robot->GoToPosition(dd);//, hwr_Start_position

//   }

//   if (substr =="clamp") {

//       str = "######## Try to lock/unlock the gripper #########";
//       str += "\n";
//       str += "Current clamper value is ";
//       str += QString::number(Servos[0]);
//       if(Servos[0]==0) { Servos[0]=90;}
//       else {Servos[0]=0;}
//       this->send_Data(LASTONE);
//   }//"clamp"

//   if (substr =="lock") {

//       str = "######## Try to lock the gripper ######### ";
//       str += "Current clamper value is ";
//       str += QString::number(Servos[0]);
//       Servos[0]=90;
//       this->send_Data(NOT_LAST); //NOT_LAST LASTONE
//   }//"lock"

//   if (substr =="unlock") {

//       str = "######## Try to UNlock the gripper ######### ";
//       str += "Current clamper value is ";
//       str += QString::number(Servos[0]);
//       Servos[0]=0;
//       this->send_Data(NOT_LAST); //NOT_LAST LASTONE
//   }//"unlock"

//   if (substr == "close") {
//       Robot->serial.close();

//   }

//   // Запрашиваем в Arduino значения углов сервоприводов
//   if (substr == "getservos") {
//       QByteArray dd ;
//       dd.resize(parcel_size);
//       memcpy(dd.data(), get_values_position, parcel_size);
//       Robot->GoToPosition(dd);

//   }

//   if (substr == "info") {
//       // Вот тут делаем присвоение статуса.
//       str = Robot->GetCurrentStatus();
//       jsnStore->jsnInfo["state"] = str.toStdString();
//       jsnStore->jsnInfo["rc"] = RC_SUCCESS;
//       //jsnStatus[""]

//       // serialization with pretty printing
//       // pass in the amount of spaces to indent
//       int indent = 3;
//       std::string s2 = jsnStore->jsnInfo.dump(indent);
//       str = QString::fromStdString(s2);

//       GUI_Write_To_Log(value, "!!!!!!!!!!! Current STATUS is ");
//       GUI_Write_To_Log(value, str);

//       //str = QString::fromStdString(s2);
//       //str = QJsonDocument(jsnStatus).toJson(QJsonDocument::Compact);


//       emit Write_2_TcpClient_Signal (str);

//   }
   //++++++++++++++ /service?name=getactions
//      if (substr == "getactions") {
//        jsnStore->jsnGetActionsAnswer["rc"] = RC_SUCCESS;
//          int indent = 3;
//          std::string s2 = jsnStore->jsnGetActionsAnswer.dump(indent);
//          str = QString::fromStdString(s2);
//          GUI_Write_To_Log(value, "!!!!!!!!!!! Current Actions LIST is ");
//          GUI_Write_To_Log(value, str);

//          emit Write_2_TcpClient_Signal (str);

//      }//substr == "getactions"



//}
//++++++++++++++++++++++++++
// Слот сигнала HiWonder::Moving_Done_Signal
// Пишем в лог сообщение, что комплексная команда (например,взять кубик в одной точке и положить в другую)
// А также это индикатор, что команда выполнена и можно, например, отправить эти данные вебсерверу.
void MainProcess::Moving_Done_Slot()
{
    QString str, myname;
    int value = 0xFAAA;
    int result;

    GUI_Write_To_Log(value, "Demo cycle finished !!!");
    // Меняем статус, теперь "done"
    std::cout<<"Set DONE to Robot!" << std::endl;
    Robot->SetCurrentStatus ("done");

    str = "Current json object name is ";
    myname = mainjsnObj.value("name").toString();
    result = QString::compare(myname, Robot->active_command); // 0 -> equals
    str += myname;
    GUI_Write_To_Log(value, str);
    if (result){
        str = "Значения Robot->active_command и action_list['name'] неравны. \n";
        str += Robot->active_command; str += " and "; str += myname;
        GUI_Write_To_Log(value, str);
    }
    else {
    // Вот как бэ не совсем правильно... надо передавать QJsonObject в свой класс и там все менять...
    jsnStore->setActionDone(mainjsnObj);

//    str = "After moving the status is :\n";
    str = jsnStore->returnJsnData();
    GUI_Write_To_Log(value, str);
    emit Write_2_TcpClient_Signal(str);
    }

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
// Пришло из GUI-версии. В console-версии тоже нужно.
void MainProcess::update_Servos_from_position(unsigned char *pos)
{
    for (int i =0; i< DOF; i++) { Servos[i] = pos[i]; }

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
