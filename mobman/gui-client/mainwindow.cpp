#include "mainwindow.h"
#include "ui_mainwindow.h"

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

}

MainWindow::~MainWindow()
{
    delete ui;
    LogFile.close();
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
void MainWindow::makeSocket(QString ipaddress, quint16 port)
{
    socketDEV = new QTcpSocket(this);
    socketDEV->setSocketOption(QAbstractSocket::KeepAliveOption, true);

    //Соединение сигналов со слотами
    connect(socketDEV, &QIODevice::readyRead, this, &MainWindow::onDEVSocketReadyRead_Slot);//, Qt::QueuedConnection);
    connect(socketDEV, SIGNAL(disconnected()), this, SLOT(socketDEV_onDisconnected_Slot()),Qt::AutoConnection);

    connect (this->socketDEV, &QTcpSocket::connected, this, &MainWindow::onDEVSocketConnected_Slot); // Send "status" command

    socketDEV->connectToHost(ipaddress, port);


}
//+++++++++++++++++++++++++++++++++++++++++
// Парсим JSON-ответ от девайсов
void MainWindow::parseJSON(QString jsnData)
{
    int value = 0xC7C7;
    QString str;

  str = jsnData; // Но тут еще надо обрезать HTTP-заголовки
  str = "{\" rc\": 0, \"info\": \"success\",\"name\": \"getposition\", \"data\": {\"detected\": true, \"x\": -15.0, \"y\": -60.0, \"width\": 113, \"height\": 108, \"err_angle\": -1.38117702629722, \"distance\": 209.21150512634233}}";
  //Assign the json text to a JSON object
  jsnDoc = QJsonDocument::fromJson(str.toUtf8());
  if(jsnDoc.isObject() == false) GUI_Write_To_Log(value,"It is not a JSON object");

  //Get the main JSON object and get the datas in it
  jsnObj = jsnDoc.object();


//  str = "JSON data :\n";
//  QJsonValue name= jsnObj.value("name");
  str = "";
  foreach(const QString& key, jsnObj.keys()) {
      QJsonValue jvalue = jsnObj.value(key);
      //qDebug() << "Key = " << key << ", Value = " << jvalue.toString();
      str +=  "Key = "; str += key; str += ", Value = "; str += jvalue.toString();
      GUI_Write_To_Log(value, str);
      str = "";
  }





}
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

}
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
    parseJSON(nextTcpdata);

}
//+++++++++++++++++++++++++++++++++++++++
// Закрытие соединения производится со стороны отправителя. Сюда попадаем, когда приходит сигнал "disconnected"
void MainWindow::socketDEV_onDisconnected_Slot()
{
  int value = 0xf1f1;
  GUI_Write_To_Log(value, "!!!!!!!!!!!!!!!!! Connection closed to SOCKET device !!!!!!!!!!!!!!!!!!!!");
}

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
     request += "192.168.1.201:8383\r\n";
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

