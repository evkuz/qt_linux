#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientsocket.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Log_File_Open(Log_File_Name);
    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    QString str = "The application \"";  str +=target_name; str += "\"";
    GUI_Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));

    QDataStream in;

    currentTcpdata = "";

    request = "GET ";
    request += "/run?cmd=status&";
    request += " HTTP/1.1";
    request += "\r\nHost: ";
    request += HIWONDER_IP; request+=":"; request+=strARM_Port; request+="\r\n";
    request += "Accept: */*\r\n";
    request += "Access-Control-Allow-Origin: *\r\n";
    request += "\r\n";

    mysocketDev = new clientSocket(HIWONDER_IP, ARM_Port, request);

    // Создание объекта потока QObject
    thread_A = new QThread;
    thread_Timer = new myThread();

    connect(thread_A, &QThread::started, thread_Timer, &myThread::A_SLOT); //, Qt::QueuedConnection
    connect(thread_Timer, &myThread::SendToTcp_Signal, mysocketDev, &clientSocket::SendToTcp_Slot, Qt::QueuedConnection);

    // Вспомагательный, запись в лог передаваемых по TCP данных
    connect(mysocketDev, &clientSocket::Write_2_TcpClient_Signal, this, &MainWindow::Write_2_TcpClient_Slot);

    // Вспомагательный, запись в лог полученных по TCP данных
    connect(mysocketDev, &clientSocket::Read_From_TcpClient_Signal, this, &MainWindow::Read_From_TcpClient_Slot);


    // catch socket error if any
    connect(mysocketDev->socketDEV, &QAbstractSocket::errorOccurred, mysocketDev, &clientSocket::displayError);

    //socketError to log
    connect(mysocketDev, &clientSocket::socketErrorToLog_Signal, this, &MainWindow::socketErrorToLog_Slot);

    thread_Timer->moveToThread(thread_A);

//    statusTimer = new QTimer(this);

//    connect(statusTimer, &QTimer::timeout, this, &MainWindow::timerProcessing_Slot);


}

MainWindow::~MainWindow()
{
    int value = 0xffff;
    delete ui;
    LogFile.close();

    delete thread_A;
    delete thread_Timer;
    GUI_Write_To_Log(value, "The program is going to be closed");
}

void MainWindow::Log_File_Open(QString lname)
{
    LogFile.setFileName(lname);
    LogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

}

void MainWindow::GUI_Write_To_Log(int value, QString log_message)
{
    QDateTime curdate ;
    QTextStream uin(&this->LogFile);

    QString str, str2;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_message << "\n";

}
//++++++++++++++++++++++++++++++++++++++++
// Create socket, than connect to host
void MainWindow:: makeSocket(QString ipaddress, quint16 port)
{
    socketDEV = new QTcpSocket(this);
    socketDEV->setSocketOption(QAbstractSocket::KeepAliveOption, true);

    //Соединение сигналов со слотами
    connect(socketDEV, &QIODevice::readyRead, this, &MainWindow::onDEVSocketReadyRead_Slot);//, Qt::QueuedConnection);
    connect(socketDEV, &QAbstractSocket::disconnected, this, &MainWindow::socketDEV_onDisconnected_Slot,Qt::AutoConnection);

    connect (this->socketDEV, &QTcpSocket::connected, this, &MainWindow::onDEVSocketConnected_Slot); // Send "status" command
    connect (this->socketDEV, &QTcpSocket::stateChanged, this, &MainWindow::onSocketDevState_Changed);

    socketDEV->connectToHost(ipaddress, port);

//    QString str;
//    str = "Im in makesocket";


}
//+++++++++++++++++++++++++++++++++++++++++
// Парсим JSON-ответ от девайсов
// 0x20 (пробел), 0x09 (табуляцию), 0x0A (перевод строки) и 0x0D (возврат каретки).
// Пробелы допускаются до и после «структурных символов» (structural characters) []{}:,
// QString (данные от CV-Device) -> QJsonDocument -> QJsonObject и вот дальше надо парсить.

void MainWindow::parseJSON(QString jsnData)
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

  GUI_Write_To_Log(value, "\n");
  GUI_Write_To_Log(value, "Http headers cutted, so data are as follows !");
  GUI_Write_To_Log(value, "\n");

  str = substr; // jsnData; // Но тут еще надо обрезать HTTP-заголовки. ОБрезаем все до первого символа '{'
  GUI_Write_To_Log(value, str);
 // str = "{\" rc\": 0, \"info\": \"success\",\"name\": \"getposition\", \"data\": {\"detected\": true, \"x\": -15.0, \"y\": -60.0, \"width\": 113, \"height\": 108, \"err_angle\": -1.38117702629722, \"distance\": 209.21150512634233}}";
  //Assign the json text to a JSON object
  jsnDoc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
  if(jsnDoc.isObject() == false) GUI_Write_To_Log(value,"It is not a JSON object");
  if(jsonError.error != QJsonParseError::NoError){
          str = "Error: "; str += jsonError.errorString();
          GUI_Write_To_Log(value, str);
   }       //return;

  //Get the main JSON object and get the datas in it
  jsnObj = jsnDoc.object();


//  str = "JSON data :\n";
//  QJsonValue name= jsnObj.value("name");

  GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!!!!! Go to recursive parsing !!!!!!!!!!!!!!!!!!!!");
  traversJson(jsnObj);
  GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!!!!! Get back from recursive parsing !!!!!!!!!!!!!!!!!!!!");

  // Парсинг JSON закончили, получили глобальную переменную  jsndataObj - это объект "data" : {}. Извлекаем из него данные.
  double cvdistance = jsndataObj.value("distance").toDouble();
  str = "Got distance value as double : ";
  str += QString::number(cvdistance);

  GUI_Write_To_Log(value, str);

} // parseJSON()
//+++++++++++++++++++++++++++++++++++++++
// After socket becoming in state "connected", the connected() signal
// is emitted. Here is the slot for that signal.
// Отсылаем данные в сокет - HTTP запрос.
void MainWindow::onSocketConnected_Slot()
{
    // Раз сокет готов отправляем ему запрос, предварительно подготовив.
    // А вот теперь готовим команду "/service?name=getposition"
     QString request = "GET ";
     request += "/service?name=getposition";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     request += "192.168.1.201:5001\r\n";
     request += "Accept: */*\r\n";
    // request += "Access-Control-Allow-Origin: *\r\n";

     request += "content-type: application/json\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

    // request += "";

     GUI_Write_To_Log(0xfefe, "The following Data is going to be sent to CV :");
     GUI_Write_To_Log(0xfefe, request.toUtf8());
     socketCV->write(request.toUtf8());// Тут это QByteArray



}
//+++++++++++++++++++++++++++++++++++++++++++++
// Считываем данные из буфера сокета.
void MainWindow::onSocketReadyRead_Slot()
{

        /*
         * Now, TCP is based on sending a stream of data, so we cannot expect to get the entire fortune in one go. Especially on a slow network,
         * the data can be received in several small fragments. QTcpSocket buffers up all incoming data and emits readyRead() for every new block that arrives,
         * and it is our job to ensure that we have received all the data we need before we start parsing.
         */
        int value = 0xfafa;

//        in.startTransaction();

        QString nextTcpdata, str;

//        in >> nextTcpdata;
//        int status = in.status();
//        str = "Stream status value is ";
//        str += QString::number(status);
//         GUI_Write_To_Log(value, str);
//        GUI_Write_To_Log(value, "Next portion of data is ");
//        GUI_Write_To_Log(value, nextTcpdata);

//        if(!in.commitTransaction()){
//            GUI_Write_To_Log(value, "commitTransaction exit, complete data reading from socket");
//            GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!! That's are all the data from CV device !!!!!!!!!!!!!!!!!!!!");
//            GUI_Write_To_Log(value, nextTcpdata);

//            return;
//           }
    int befbytes = socketCV->bytesAvailable();
        nextTcpdata = socketCV->readAll();
    int realbytes = nextTcpdata.size();
    int afterbytes = socketCV->bytesAvailable();

    str = "Bytes before reading "; str += QString::number(befbytes); GUI_Write_To_Log(value, str);
    str = QString::number(realbytes); str += " bytes has been readed"; GUI_Write_To_Log(value, str);

    str = "Bytes after reading  "; str += QString::number(afterbytes); GUI_Write_To_Log(value, str);



//        if (nextTcpdata.contains("HTTP/1.0 200 OK")) return;

//        if (nextTcpdata == currentTcpdata){

//            GUI_Write_To_Log(value, "Data all readed");
//            socketCV->abort();
//            QTimer::singleShot(0, this, &MainWindow::on_GetDistanceButton_clicked);
//            return;
//        }



//        currentTcpdata = nextTcpdata;

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

//            int rDistance = my_round(cvd);
//            str = "!!!!!!!!!!!!!!!!! The distance as rounded to closest 10x int value : ";
//            substr =  QString::number(rDistance);
//            str += substr;
//            GUI_Write_To_Log(value, str);

//            unsigned char *arrPtr = mob_parking_position;

            // Выбираем массив углов через switch, потом попробуем через словарь, т.е. ключ - значение, где значением будет массив
//            switch (rDistance)
//            {
//           // unsigned char ptr;

//                case 130:

//                    arrPtr = mob_pos_13;
//    //            memcpy(Servos, mob_pos_13, DOF);  this->send_Data(LASTONE);
//    //            GUI_Write_To_Log(value, "!!!!! position 130 !!!!");
//                break;

//                case 140:
//                    arrPtr = mob_pos_14;
//                    memcpy(Servos, mob_pos_14, DOF);  this->send_Data(LASTONE);
//                break;

//                case 150: arrPtr = mob_pos_15; break;
//                case 160: arrPtr = mob_pos_16; break;
//                case 170: arrPtr = mob_pos_17; break;
//                case 180: arrPtr = mob_pos_18; break;
//                case 190: arrPtr = mob_pos_19; break;
//                case 200: arrPtr = mob_pos_20; break;


//              default:
//                GUI_Write_To_Log(value, "!!!!! Unrecognized position, Go to Parking !!!!");
//                arrPtr = mob_parking_position; break;
//              break;

//            }

//            memcpy(Servos, arrPtr, DOF);  this->send_Data(LASTONE);

            str =  "!!!!! position "; str += QString::number(cvd); str += "mm !!!!";

            GUI_Write_To_Log(value, str);


            //ВСе данные получили Отсоединение от удаленнного сокета
            socketCV->disconnectFromHost();

}// onSocketReadyRead_Slot()
//++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::onSocketDevState_Changed()
{
    int value = 0x9999;
    QString str = "SocketDev state changed \n";
    str += "Current socketDev state is "; str += QString::number(socketDEV->state());
    GUI_Write_To_Log(value,str);
} // onSocketDevState_Changed()
//++++++++++++++++++++++++++++
// Пока оставим как есть, для сохранения работоспособности кода.
void MainWindow::onARMSocketConnected_Slot()
{
;
}
//+++++++++++++++++++++++++++++
// slot for "connected" signal for socketDEV
// data for GET request are already in "request" variable
void MainWindow::onDEVSocketConnected_Slot()
{
    GUI_Write_To_Log(0xf7f7, "The following Data is going to be sent to SOCKET Device :");
    GUI_Write_To_Log(0xf7f7, request.toUtf8());
    socketDEV->write(request.toUtf8());

}
//++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::onARMSocketReadyRead_Slot()
{
;

}
//++++++++++++++++++++++++++++++++++++++++++++++
// ОБщий слот на все устройства
void MainWindow::onDEVSocketReadyRead_Slot()
{
    QString nextTcpdata, str;
    int value = 0xf9f9;
    int befbytes = socketDEV->bytesAvailable();
        nextTcpdata = socketDEV->readAll();
    int realbytes = nextTcpdata.size();
    int afterbytes = socketDEV->bytesAvailable();

    str = "Bytes before reading "; str += QString::number(befbytes); GUI_Write_To_Log(value, str);
    str = QString::number(realbytes); str += " bytes has been readed"; GUI_Write_To_Log(value, str);

    str = "Bytes after reading  "; str += QString::number(afterbytes); GUI_Write_To_Log(value, str);

    GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!! There are some  data from SOCKET device !!!!!!!!!!!!!!!!!!!!");
    GUI_Write_To_Log(value, nextTcpdata);
    //ВСе данные получили Отсоединение от удаленнного сокета
   // socketDEV->disconnectFromHost();

    // Запускаю JSON-парсинг
    // Нужно, когда выцепляем данные из посылки, чтобы обработать.
    // Пока закроем, не нужно, нужна скорость примема данных.
    //parseJSON(nextTcpdata);

}
//+++++++++++++++++++++++++++++++++++++++
// Закрытие соединения производится со стороны отправителя. Сюда попадаем, когда приходит сигнал "disconnected"
void MainWindow::socketDEV_onDisconnected_Slot()
{
  int value = 0xf1f1;
  GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!! Connection closed to SOCKET device !!!!!!!!!!!!!!!!!!!!");
}
//+++++++++++++++++++++++++++++++++++++++
// Slot for timer timeout signal
// send "status" request to robot
void MainWindow::timerProcessing_Slot()
{
    QString str;
    int value = 0xf2f2;
    str = "I'm in timerProcessing_Slot";
    GUI_Write_To_Log(value, str);

    request = "GET ";
    request += "/run?cmd=status&";
    request += " HTTP/1.1";
    request += "\r\nHost: ";
    //request += "192.168.1.201:8383\r\n";
    request += HIWONDER_IP; request+=":"; request+=strARM_Port; request+="\r\n";
    request += "Accept: */*\r\n";
    request += "Access-Control-Allow-Origin: *\r\n";
    request += "\r\n";

    QString myipaddress = HIWONDER_IP;
    quint16 myport = ARM_Port;
    makeSocket(myipaddress, myport);

}
// Слот сигнала clientSocket::Write_2_TcpClient_Signal(request)
// Пишем в лог данные, ОТПРАВЛЕННЫЕ по tcp
void MainWindow::Write_2_TcpClient_Slot(QString message)
{
    QString str;

    str = "New TCP command has been sent : "; str += message;
    GUI_Write_To_Log(0xf00f, str);

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Слот сигнала clientSocket::Read_From_TcpClient_Signal(QString)
// write data to log, Received by TCP
void MainWindow::Read_From_TcpClient_Slot(QString message)
{
     int value = 0x1111;
     GUI_Write_To_Log(value, message);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Слот сигнала clientSocket::socketErrorToLog_Signal(QString)
void MainWindow::socketErrorToLog_Slot(QString message)
{
    int value = 0x2222;
    GUI_Write_To_Log(value, message);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++
// - create qtcpsocket with signal/slot configuration
// - connect to host with CV
void MainWindow::on_GetDistanceButton_clicked()
{

    // Формируем запрос, "кнопка Get Distance"
    // готовим команду "/service?name=getposition"
     request = "GET ";
     request += "/service?name=getposition";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     request += "192.168.1.201:5001\r\n";
     request += "Accept: */*\r\n";
    // request += "Access-Control-Allow-Origin: *\r\n";

     request += "content-type: application/json\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

    makeSocket(CVDev_IP, CVDev_Port);





//    socketCV = new QTcpSocket(this);
//    socketCV->setSocketOption(QAbstractSocket::KeepAliveOption, true);

//    //Соединение сигналов со слотами
//    connect(socketCV, &QIODevice::readyRead, this, &MainWindow::onSocketReadyRead_Slot);//, Qt::QueuedConnection);
//    connect(socketCV, SIGNAL(disconnected()), this, SLOT(CV_onDisconnected()),Qt::AutoConnection);

//    connect (this->socketCV, &QTcpSocket::connected, this, &MainWindow::onSocketConnected_Slot);

//    in.setDevice(socketCV);
//    in.setVersion(QDataStream::Qt_5_12);
//    socketCV->connectToHost(CVDev_IP, CVDev_Port);
    // После успешного socketCV->connectToHost() будет сигнал connected

}
//++++++++++++++++++++++++++++++++++++++++
// - prepare HTTP request  http://192.168.1.201:8383/run?cmd=status&
void MainWindow::on_GetStatusButton_clicked()
{
    // Формируем запрос, "кнопка Get Status"
    // А вот теперь готовим команду "/run?cmd=status&"
     request = "GET ";
     request += "/run?cmd=status&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
//     request += "192.168.1.201:8383\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     QString myipaddress = CVDev_IP;
     quint16 myport = ARM_Port;
     makeSocket(myipaddress, myport);

}

//++++++++++++++++++++++++++++++++++++++++++++++
// - prepare HTTP request  http://192.168.1.201:8383/run?cmd=ready&
void MainWindow::on_GetReadyButton_clicked()
{
    // Формируем запрос, "кнопка Get Ready"
    // А вот теперь готовим команду "/run?cmd=ready&"
     request = "GET ";
     request += "/run?cmd=ready&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     request += "192.168.1.201:8383\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     makeSocket(CVDev_IP, ARM_Port);

}

//++++++++++++++++++++++++++++++++++++++++++++++
// - prepare HTTP request  http://192.168.1.201:8383/run?cmd=parking&

void MainWindow::on_GetParkingButton_clicked()
{
    // Формируем запрос, "кнопка Parking"
    // А вот теперь готовим команду "/run?cmd=parking&"
     request = "GET ";
     request += "/run?cmd=parking&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     request += "192.168.1.201:8383\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     makeSocket(CVDev_IP, ARM_Port);

}

//+++++++++++++++++++++++++++++++++++++++
// Формируем запрос, "кнопка Get Box"
// готовим команду "/run?cmd=get_box&"

void MainWindow::on_GetBoxButton_clicked()
{
    request = "GET ";
    request += "/run?cmd=get_box&";
    request += " HTTP/1.1";
    request += "\r\nHost: ";
    request += "192.168.1.201:8383\r\n";
    request += "Accept: */*\r\n";
    request += "Access-Control-Allow-Origin: *\r\n";
    request += "\r\n";

    makeSocket(CVDev_IP, ARM_Port);

}

//+++++++++++++++++++++++++++++++
//Ф-ция рекурсивно парсит JSON в случае вложенных объектов.


void MainWindow::traversJson(QJsonObject json_obj){
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
                      // Не хватает
                      if (value.isArray()) {
                          QJsonArray jsnArray = value.toArray();
                          // А еще желательно проверить, что у этого value соответствующий key ==  "action_list":
                          // Тут выводим элементы массива, точнее только имена экшенов, т.е. значение name
                          if (key == "action_list") {str += "Many actions, look at output above";}
                      }


                     // str += value.toString();
                      GUI_Write_To_Log(0x5555, str);
                      str = "";

          //qDebug() << "Key = " << key << ", Value = " << value;
         }
        else{
            // А теперь определяем тип данных поля.
             //jsndataObj = json_obj["data"].toObject(); // Так тоже работает, но уже есть привязка к конкретному случаю.
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

}
//++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_GetServicesButton_clicked()
{
    request = "GET ";
    request += "/service?name=getservices&";
    request += " HTTP/1.1";
    request += "\r\nHost: ";
    request += "192.168.1.201:8383\r\n";
    request += "Accept: */*\r\n";
    request += "Access-Control-Allow-Origin: *\r\n";
    request += "\r\n";

    makeSocket(CVDev_IP, ARM_Port);

}

//++++++++++++++++++++++++++++++++++++++++++++


void MainWindow::on_GetActionsButton_clicked()
{
    request = "GET ";
    request += "/&";
    request += " HTTP/1.1";
    request += "\r\nHost: ";
    //request += "192.168.1.201:8383\r\n";
    request += HIWONDER_IP; request+=":"; request+=strARM_Port; request+="\r\n";
    request += "Accept: */*\r\n";
    request += "Access-Control-Allow-Origin: *\r\n";
    request += "\r\n";

    makeSocket(HIWONDER_IP, ARM_Port);

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++ Get Status in new format
void MainWindow::on_NEW_GetStatusButton_clicked()
{
    // Формируем запрос, "кнопка Get Status"
    // А вот теперь готовим команду "/run?cmd=status&"
     request = "GET ";
     request += "/status&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     request += "192.168.1.201:8383\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     QString myipaddress = CVDev_IP;
     quint16 myport = ARM_Port;
     makeSocket(myipaddress, myport);

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_BeforeMovingButton_clicked()
{
    // Формируем запрос, "кнопка Before Moving"
    // А вот теперь готовим команду "/run?cmd=formoving&"
     request = "GET ";
     request += "/run?cmd=formoving&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     request += "192.168.1.201:8383\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     QString myipaddress = CVDev_IP;
     quint16 myport = ARM_Port;
     makeSocket(myipaddress, myport);

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_PutBoxButton_clicked()
{
    // Формируем запрос, "кнопка Put Box"
    // А вот теперь готовим команду "/run?cmd=put_box&"
     request = "GET ";
     request += "/run?cmd=put_box&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     request += "192.168.1.201:8383\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     QString myipaddress = CVDev_IP;
     quint16 myport = ARM_Port;
     makeSocket(myipaddress, myport);

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_ClampButton_clicked()
{
    // Формируем запрос, "кнопка Clamp"
    // А вот теперь готовим команду "/run?cmd=clamp&"
     request = "GET ";
     request += "/run?cmd=clamp&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     //request += "192.168.1.201:8383\r\n";
     request += CVDev_IP; request+=":"; request+=strARM_Port; request+="\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     QString myipaddress = CVDev_IP;
     quint16 myport = ARM_Port;
     makeSocket(myipaddress, myport);

}

//++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_SetServosButton_clicked()
{
    // Формируем запрос, "кнопка SetServos"
    // А вот теперь готовим команду "/run?cmd=setservos=&"

//     int num;
//     QString data = ui->Param_lineEdit->text();
//     QRegExp rx("[, ]");// match a comma or a space
//     QStringList list;
//     list = data.split(rx, Qt::SkipEmptyParts);
//     num = list.size (); //Число элементов, начиная с 1

     request = "GET ";
     request += "/run?cmd=setservos="; request+=ui->Param_lineEdit->text(); request+="&"; //
     // Вот тут добавляем значения Servos
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     request += CVDev_IP; request+=":"; request+=strARM_Port; request+="\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     QString myipaddress = CVDev_IP;
     quint16 myport = ARM_Port;
    makeSocket(myipaddress, myport);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_ResetButton_clicked()
{
    // Формируем запрос, "кнопка Reset"
    // А вот теперь готовим команду "/run?cmd=reset&"
     request = "GET ";
     request += "/run?cmd=reset&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     request += CVDev_IP; request+=":"; request+=strARM_Port; request+="\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     QString myipaddress = CVDev_IP;
     quint16 myport = ARM_Port;
    makeSocket(myipaddress, myport);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++
// - Жмем кнопку, отправляем запрос на выполнение команды
// - Запускаем таймер и по нему опрашиваем статус. Опрос уже в слоте таймера.

void MainWindow::on_CollapsButton_clicked()
{
    // Формируем запрос, "кнопка Collaps"
    // А вот теперь готовим команду "/run?cmd=collapse&"
     request = "GET ";
     request += "/run?cmd=collapse&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     //request += "192.168.1.201:8383\r\n";
     request += HIWONDER_IP; request+=":"; request += strARM_Port; request+="\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     QString myipaddress = HIWONDER_IP;
     quint16 myport = ARM_Port;
     makeSocket(myipaddress, myport);


}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_getStatusButton_clicked()
{
//    request = "GET ";
//    request += "/run?cmd=status&";
//    request += " HTTP/1.1";
//    request += "\r\nHost: ";
//    request += HIWONDER_IP; request+=":"; request+=strARM_Port; request+="\r\n";
//    request += "Accept: */*\r\n";
//    request += "Access-Control-Allow-Origin: *\r\n";
//    request += "\r\n";

     //Запускаем опрос статуса
     //statusTimer->start(500);

    thread_A->start();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_StandUpButton_clicked()
{
    // Формируем запрос, "кнопка standup"
    // А вот теперь готовим команду "/run?cmd=standup&"
     request = "GET ";
     request += "/run?cmd=standup&";
     request += " HTTP/1.1";
     request += "\r\nHost: ";
     //request += "192.168.1.201:8383\r\n";
     request += HIWONDER_IP; request+=":"; request+=strARM_Port; request+="\r\n";
     request += "Accept: */*\r\n";
     request += "Access-Control-Allow-Origin: *\r\n";
     request += "\r\n";

     QString myipaddress = HIWONDER_IP;
     quint16 myport = ARM_Port;
     makeSocket(myipaddress, myport);

}

