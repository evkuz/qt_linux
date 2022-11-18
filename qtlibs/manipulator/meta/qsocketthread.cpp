#include "qsocketthread.h"
#include <QMultiMap>

QSocketThread::QSocketThread(qintptr descriptror, QObject* ptrMainThread, volatile int _socketsCounter)  :
    socketDescriptor(descriptror)
{
    socketDescriptor = descriptror;
    qRegisterMetaType<qintptr>("qintptr");
    data_ready = false;
    current_status = "wait";
    toBeClosed = false;
    _ptrMainThtread = ptrMainThread;
    pauseTimer = new QTimer(this);
    socketsCounter = _socketsCounter;
//    data2Client = "";
//    data2Client.resize(500);
    qbData="";
    qbData.resize(500);

    //Шапка ответа клиенту в tcp-сокет
    response = "HTTP/1.1 200 OK\r\n";
    response += "content-type: application/json\r\n"; //text/html
    response += "Access-Control-Allow-Origin: *\r\n";
//    response += "connection : close\r\n";
    response += "\r\n";





    //ba.resize(icon)
   // threadID = QThread::currentThread(); // get thread pointer
}
//+++++++++++++++++++++++
QSocketThread::~QSocketThread()
{
    //Удаление объекта сокета
    //delete socket;

}
//+++++++++++++++++++++++++++++++++++++++++++++
// Данные из файла помещаем в строку, чтобы отправить в сокет.
QByteArray* QSocketThread::returnIconChrome()
{
    QString str = "";
    QFile iconFile(ICON_FILE_PATH);

    if (!iconFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Could not open bin file for reading";

            return this->iconBA; //QByteArray("error");
        }

        QDataStream in(&iconFile);
        QByteArray ba(iconFile.size(), 0);

        qint32 bytes = in.readRawData(ba.data(), ba.size());

        qInfo() << "bytes read: " << bytes;
        qInfo() << ba.toHex(); // You read binary data, not a text. Do not try to print it as a text

        //this->iconBA = &ba;
        memcpy(this->iconBA, ba, ba.size());
    return this->iconBA;

}
//+++++++++++++++++++++++++++++++++++++++
// Answer to "/favicon.ico HTTP/1.1" request
void QSocketThread::favIconAnswer()
{
    int value = 0x5544;
    QString response = "HTTP/1.1 200 OK\r\n";
    response += "content-type: text/html\r\n"; //text/html image/png\r\n
    response += "Access-Control-Allow-Origin: *\r\n";
//    response += "Content-Length: ";
//    response += QString::number(228); //228*2 = 456; 318 *2 = 636
//    response += "\r\n";
//    response += "Connection: close";
    response += "\r\n\r\n";
   // QString img = returnIconChrome().toStdString();


//    QByteArray ba = response.toUtf8();
//    ba.append(returnIconChrome()->data());
    response += "<html>\r\n";
    response += "<head>\r\n";

    response += "<link rel=\"icon\" href=\"data:,\">\r\n";
//    response += "<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"http://arduino.cc/en/favicon.png\" />\r\n";
    //response += "<link href=\"data:image/x-icon;base64,AAABAAEAEBAQAAAAAAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAEhEQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD//wAA//8AAP//AAD//wAA//8AAP//AAD//wAA//8AAP7/AAD//wAA//8AAP//AAD//wAA//8AAP//AAD//wAA\" rel=\"icon\" type=\"image/x-icon\" />";

    response += "</head>\r\n";
//    response += "<body>";
//    response += "IT'S   FAVICON FAVICON DATA !";
//    response += "</body>";

    response += "</html>\r\n";

//    response += "<title>MEGATESTING!!!!</title>";
//    response += "<link rel=\"icon\" href=\"data:,\">";

    if (!socket->isValid() || this->socket->state() != QAbstractSocket::ConnectedState){
       qDebug() << "!!!!!!!!!!!!!!!!!!!!! SOCKET IS NOT OPENED";
       int state = socket->state();
       qDebug() << "socket state value is " << state;
       return;

    }



    qDebug() << value << "Favicon Current socket " << this->socketDescriptor << " state is " << this->socket->state();
    qDebug() << "Going to SEND FAVICON FAVICON FAVICON data to socket with an desctiptor " << QString::number(this->socketDescriptor);

    int bytesWritten = socket->write(response.toUtf8());
//    int bytesWritten = socket->write(ba);
    socket->disconnectFromHost();
    emit stopThread_signal();
//    this->deleteLater();
    toBeClosed = true;

    qDebug().noquote() << "The folowing " << bytesWritten << " bytes of data has been written to socket :\n" <<response.toUtf8();

    //    qDebug() << "From faviconAnswer" << returnIconChrome();
} //favIconAnswer()
//++++++++++++++++++++++++++++++++++++++++++++++++++++
// main loop of the QSocketThread
void QSocketThread::theLoop()
{
    this->process_TheSocket();
    uint64_t counter = 0;
    while ((!this->toBeClosed) ) // !!!event-loop!!! && !this->once
        {
            //this->process_TheSocket();
//            qDebug() << "iteration " << counter;
            counter++;
        }
    qDebug() << "^^^^^^^^^^^^^^^^ STOP THREAD  ^^^^^^^^^^";
        emit stopThread_signal();


}
//++++++++++++++++++++++++++++++++++++++++++++++++++++
void QSocketThread::tcpParce(QString tcpData)
{
//    QRegularExpression("^asd$");
    QRegularExpression regexStatusOnly("\\/status$");
    QRegularExpression regexStatusParam("\\/status?.*$");



} // theLoop()
//+++++++++++++++++++++++++++
//++++++++++++++++++ главный event loop потока
// Самого event loop пока не видно, а он нужен.
// Надо в этом же потоке дождаться ответа и отправить его.
void QSocketThread::process_TheSocket()
{
    // eventloop тут не делаем, достаточно 1 сокета.
    //Создание объекта сокета
    socket = new QTcpSocket();
    //Сопоставление объекта сокета с системным сокетом через дескриптор
    socket->setSocketDescriptor(socketDescriptor);
    socket->setSocketOption(QAbstractSocket::LowDelayOption,1);



    //Соединение сигналов со слотами
    // сигнал readyRead и слот onReadyRead живут в одном потоке, поэтому DirectConnection
    connect(socket, &QAbstractSocket::readyRead, this, &QSocketThread::onReadyRead, Qt::DirectConnection); //, Qt::QueuedConnection DirectConnection

    connect(socket, &QAbstractSocket::disconnected, this, &QSocketThread::deleteLater); //, Qt::AutoConnection


    //connect(socket, &QAbstractSocket::disconnected,

    //connect(socket, &QTcpSocket::stateChanged, this, &QSocketThread::onSocketDevState_Changed);

    connect(socket, &QAbstractSocket::errorOccurred , this, &QSocketThread::displayError);




    qDebug() << "$$$$$$$$$ process_TheSocket "<< QThread::currentThread() << " $$$$$$$$$$ new socket $$$ " << QString::number(this->socketDescriptor);

}
//+++++++++++++++++++++++++++++++++++++
// Данные от TCP-client считываем, отправляем на парсинг, готовим ответ.
// - Определяем тип запроса (статус, экшен, сервис)
// - Парсим содержимое запроса.
// - в Зависимости от типа запускаем соовтествующую ф-цию обработки (статус, экшен, сервис), передаем в эту ф-цию
//   как аргумент - содержимое запроса.
// - Запрос favicon игнорируем.

void QSocketThread::onReadyRead()
{
//   QList<QString>  strcommand = { "/run?cmd=", "/service?name=", "/status", "/status?action=", "/action?name=", "/favicon"};
// - Чтение информации из сокета и вывод в консоль
// - Парсинг полученной команды и вызов соответствующей ф-ции, для обработки.
//    qintptr socketNumber = this->socketDescriptor;
//    qDebug() << QThread::currentThread() << "$$$$$$$$$ I'm in ReadyRead_Slot for socket " << socketNumber;
// ...
// вот тут запускаем таймер, чтобы дождаться favicon.

   // int value = 0xfafa;
    QString nextTcpdata;
    nextTcpdata = "";

    int befbytes = socket->bytesAvailable();
    if (befbytes < 20) {// "HTTP/1.0 200 OK" == 17 байт занимает такая строка , обрабатывать не нужно, выходим.
                        // Так делает CV-камера. Шлёт ОК, и через 10мс остальное.
        qDebug() << "######### onReadyRead thread ID " <<  QThread::currentThread() << "!!!!!!!!! Too few bytes readed " << befbytes;
        return;
        // Мы не считывали буфер сейчас, когда в сокет еще "нальют" данныех, тогда и считаем всё целиком.
    }

    QByteArray httpHeaders = socket->readAll();
    int realbytes = httpHeaders.size();

    qDebug() << "######### onReadyRead thread ID " <<  QThread::currentThread() << "!!!!!!!!! Have got " << realbytes << " bytes of Data FROM TCP SOCKET !!!!!!!!!!!!!!!!!!!";//<< QString::number(socketNumber);

    //Парсим команду.
    QString message, substr, searchstr;
    message = QString(httpHeaders);

    int sPosition, ePosition; // Индекс строки run в запросе.

//  Вот тут обходим запрос "GET /favicon.ico HTTP/1.1"
    QString  wrong_mess = "favicon.ico";//"Referer:";//"status";//"favicon.ico";
    if (message.contains(wrong_mess))
        {
           qDebug() << "\r\nHave got favicon request !\r\n";
//           this->favIconAnswer();
           socket->disconnectFromHost();
           emit stopThread_signal();

           return;
       }
//     else // so it's not favicon request
//     {
        bool matched = false;
        int i = 0; sPosition = 0;
        // Перебираем массив strcommand, определяем тип запроса (разные Виды команд)
        while (!matched and i< strcommand.size()){
            sPosition = message.indexOf(strcommand.at(i));
            if (sPosition != -1) {
                 matched = true;
    //             qDebug() << "Inside sPosition is " << sPosition;
    //             qDebug() << "Inside Index is " << i;
            }
            ++i;
        } // while
        i--;

        if (!matched) {return;} // Выходим, если нет правильных строк. Пока "молча", клиенту не отвечаем.

//    qDebug() << "Index value is" << i;
//    qDebug() << "Matched command sPosition is " << sPosition;
//    if (i>=0) {
//        qDebug() << "Matched string is " << strcommand.at(i);
//    }


//    if (i<0) { //Строка не найдена
//        qDebug() << "No MATCHING in onReadyRead";
//        QString myresponse;
//        myresponse = "{\r\n";
//        myresponse += "{\"info\" : \"There is now action with such a name\"}";
//        myresponse += "{\"rc\" : \"-100\"}";
//        myresponse += "}";
//        socket->write(myresponse.toUtf8());
//        socket->disconnectFromHost();
//        socket->waitForDisconnected();
//        this->deleteLater();
//        return;
//    }

    qDebug() << "Type of command is " << strcommand.at(i);
    int commandType; // Тип зароса , статус, экшен, сервис ?
    commandType = i;

    // вид определили, пишем в переменную
    searchstr = strcommand.at(i);

    // Take the rest of the string, matched with strcommand list. I.e. parameters (for "setservos=")
    // or action/service name
    if (matched)
    {
        sPosition += searchstr.size();
        // Это пробел, который идет перед "the HTTP version that defines the structure of the remaining message"
        // В нашем случае - "HTTP/1.1"
        ePosition = message.indexOf(" ", sPosition);
        substr = message.mid(sPosition, (ePosition - sPosition)); // Вся интересная нам часть tcp-данных,
        if(substr == "") substr = searchstr;                      // это остаток get-запроса, того, что можно руками в браузере вбить

        // Получили команду. Передаем её наверх
        qDebug() << "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSocketThread::onReadyRead(), i.e. from TCP SOCKET " << QString::number(this->socketDescriptor);
        qDebug() << "!!!!!!!!!!!!!!!!!!!!! QSocketThread::onReadyRead()" << QThread::currentThread() << substr;

//      qDebug() << "Socket descriptor " << QString::number(this->socketDescriptor);


// Now choose the right method to invoke, don't use socketNumber anymore
        QString statParam; // prameter of "status" request, usualy == "action"
        switch (i){

        case 0: //"/run?cmd="
            // пока только для статуса, экшены и прочее в других типах
            if (substr == "status")
            {
                QMetaObject::invokeMethod(this->_ptrMainThtread, "StatusRequest_From_TcpClient",
                                          Qt::QueuedConnection,
                                          Q_ARG(QObject*, this));
             }

            if (substr == "ready")
            {
                QMetaObject::invokeMethod(_ptrMainThtread, "ActionLaunch_From_TcpClient",
                                          Qt::QueuedConnection,
                                          Q_ARG(QObject*, this),
                                          Q_ARG(QString, substr));
             }

            if (substr == "formoving")
            {
                QMetaObject::invokeMethod(_ptrMainThtread, "ActionLaunch_From_TcpClient",
                                          Qt::QueuedConnection,
                                          Q_ARG(QObject*, this),
                                          Q_ARG(QString, substr));
             }
        break;

        case 1: // "/service?name="

            // Пока только getactions
            QMetaObject::invokeMethod(_ptrMainThtread, "ServiceLaunch_From_TcpClient",
                                      Qt::QueuedConnection,
                                      Q_ARG(QObject*, this),
                                      Q_ARG(QString, substr));


            break;


        case 2: // "/status" or "/status?action="
            if (substr == "/status"){
                qDebug() << "onReadyRead() just ONLY status case 2";

                QMetaObject::invokeMethod(_ptrMainThtread, "StatusRequest_From_TcpClient",
                                          Qt::QueuedConnection,
                                          Q_ARG(QObject*, this));

            }

             //QRegularExpression regexStatusOnly("\\/status$");
            if (substr.contains("?action=")){


                // И дальше надо выцеплять имя экшена...
//                sPosition = 8; //next after "/status?"
//                sPosition = 15; //next after "/status?action="
                sPosition = 8; //next after "?action="
                statParam = substr.mid(sPosition); // from sPosition all characters available
                qDebug() << "onReadyRead() status WITH PARAMS case 2" << "paramName is " << statParam;

                QMetaObject::invokeMethod(_ptrMainThtread, "StatusParamRequest_From_TcpClient",
                                          Qt::QueuedConnection,
                                          Q_ARG(QObject*, this),
                                          Q_ARG(QString, statParam));

            }



            break;
        case 4: // "/action?name="
            QMetaObject::invokeMethod(_ptrMainThtread, "ActionLaunch_From_TcpClient",
                                      Qt::QueuedConnection,
                                      Q_ARG(QObject*, this),
                                      Q_ARG(QString, substr));


            break;

        } //switch (i)



// И здесь же надо отправлять указатель this, чтобы из MainProcess вызвать Data_2_TcpClient_Slot

  } // if (matched)

// } //else // so it's not favicon request

// А если !matched, ...  закрываем сокет

// Надо добавить ответ на запрос "/favicon.ico HTTP/1.1"

} // onReadyRead()

//++++++++++++++++++
// Слот сигнала &QAbstractSocket::disconnected
// Дождались отправки всех данных.
// Закрваем сокет, завершаем поток.
void QSocketThread::onDisconnected()
{
    //Закрытие сокета
  //  socket->deleteLater();
    //Завершение потока
//    emit this->finished();
 ;
    //this->deleteLater();

   //this->quit();
}
//+++++++++++++++++
// this is Q_INVOKABLE
// ПРишли данные от робота на отправку в сокет.
// Данные отправляем, сокет закрываем
// Тут важно попасть в свой поток...
void QSocketThread::Data_2_TcpClient_Slot(QString data)
{
    // Готовим ответ.
    //socket->write(response.arg(QTime::currentTime().toString()).toLatin1());
//    int sockNumber = this->socketDescriptor;
//    if (sockNumber != socketNumber) {
//        qDebug()<< "thread ID" << QThread::currentThread() << "Current this->socketDescriptor is" << sockNumber << "and it's state is " << socket->state();
//        qDebug()<< "thread ID" << QThread::currentThread() << "Also socket->socketDescriptor() is " << socket->socketDescriptor();
//        qDebug()<< "thread ID" << QThread::currentThread() << "But socketNumber from MainProcess is " << socketNumber;
//        this->socket->readAll();
//        this->socket->abort();
//        return;
//    }
    qDebug() << "%%%%%%%%%%%%%%%% Data for TCP-CLIENT %%%%%%%%%%%%%";
//    qDebug() << "%%%%%%%%%%%%%%%% QByteArray          %%%%%%%%%%%%%";
    //data2Client.resize(data.size());

//    data2Client = data.toUtf8(); // have got data from MainProcess as QByteArray
//    QString response = "HTTP/1.1 200 OK\r\n";

//++++++++++++++++++++++++++ FAVICON ++++++++++++++++++++++++++++++++++++
//    response += "content-type: application/json\r\n"; //text/html
//    response += "Access-Control-Allow-Origin: *\r\n";
//    response += "connection : close\r\n";
//    response += "\r\n";

// Добавляем данные "сверху" к шапке из конструктора
    response += data;
//        response += "<html>";
//        response += "<head>";

//        response += "<title>MEGATESTING!!!!</title>";
//        response += "<link rel=\"icon\" href=\"data:,\">";
//        response += "\r\n";

//        response += "</head>";
    //    //response += "{\n\t\"status\":\"";
//        response += "<body>";
//        response += "IT'S HTML DATA !";

    qbData = response.toUtf8();
//    qbData.resize(300);
//    qbData += data2Client;
//    data2Client += response.toUtf8();
//    qDebug() << qbData;
    qDebug() << "The packet size in bytes is " << QString::number(qbData.size());

//        response += "</body>";
//        response += "</html>";
//++++++++++++++++++++++++++ END OF FAVICON ++++++++++++++++++++++++++++++++++++



//    response += "content-type: application/json\r\n";
//    response += "Access-Control-Allow-Origin: *\r\n";
//    response += "\r\n";

//    response += data2Client;

    if (!socket->isValid() || this->socket->state() != QAbstractSocket::ConnectedState){
       qDebug() << "!!!!!!!!!!!!!!!!!!!!! SOCKET with descriptor " << this->socketDescriptor <<  "IS NOT VALID"; // socketNumber
       int state = socket->state();
       QString str;

    //   QAbstractSocket::SocketState sockstate;

       switch (state){
       case QAbstractSocket::UnconnectedState:
           qDebug() << QThread::currentThread()  << "Socket destroyed from client" << "\n";
           str = "<The socket is not connected>";
           break;

      case QAbstractSocket::HostLookupState:
           str = "<The socket is performing a host name lookup>";
           break;
      case QAbstractSocket::ConnectingState:
           str = "<The socket has started establishing a connection>";
           break;
      case QAbstractSocket::ConnectedState: // ==3
           str = "<A connection is established.>";
           break;
      case QAbstractSocket::BoundState: // ==4
           str = "<The socket is bound to an address and port.>";
           break;
      case QAbstractSocket::ListeningState: // ==5
           str = "<For internal use only.>";
           break;
      case QAbstractSocket::ClosingState: // ==6
           str = "<The socket is about to close (data may still be waiting to be written).>";
           break;

      default:
           str = "UNKNOWN SOCKET STATE !";
      } //switch

    qDebug() << "socket state value is " <<state << str << "\n";


    //emit stopThread_signal();
//    toBeClosed = true; // eventLoop
    //this->deleteLater();

    this->socket->disconnectFromHost();
    //socket->deleteLater();
    emit stopThread_signal();
    //      QThread::currentThread()->terminate();

    return;

    } //if (!socket->isValid()
// Вот тут возникает разница между socketNumber и this->socketDescriptor, т.е. они разные.
//

//    qDebug() << QThread::currentThread()  << "Current socket " << socketNumber << " state is " << this->socket->state();
//    qDebug() << QThread::currentThread()  << "Going to SEND SEND SEND data to socket with an desctiptor " << QString::number(this->socketDescriptor);

    //qint64 bufbytes = socket->write(qbData.constData());
    socket->write(qbData.constData());
    socket->waitForBytesWritten();
    // socket->write(response.toUtf8());
    // ВОт тут (дождаться отправки) происходит отсоединение сокета

    //Отсоединение от удаленнного сокета
    this->socket->disconnectFromHost();
//    qDebug() << QThread::currentThread()  << "The folowing data has been written to socket : \n" << qbData;
    emit stopThread_signal();
    // Поток завершили. Объект QSocketThread удалили
    qDebug() << QThread::currentThread()  << "Finished" << "\n";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
void QSocketThread::onSocketDevState_Changed()
{
 //   QString str = "";
 //   int value = 0x9999;
    qDebug() << "SocketDev state changed \n";
    qDebug() << "Current socketDev state is " << socket->state() << " ID " << this->socketDescriptor;
//    GUI_Write_To_Log(value,str);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
// slot for QAbstractSocket::errorOccurred() signal
void QSocketThread::displayError(QAbstractSocket::SocketError socketError)
{
    QString str;
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError: // 0
        str = "The connection was refused by the peer. ";
        str += "Make sure the "; str += "TARGET server is running,\n";
        str += "and check that the host name and port settings are correct.";
        break;

    case QAbstractSocket::RemoteHostClosedError: // 1
        str = "socketError : The remote host closed the connection.";
        break;
    case QAbstractSocket::HostNotFoundError: // 2
             str = "The host was not found. Please check the ";
             str += "host name and port settings.";
        break;

    case QAbstractSocket::SocketAccessError: // 3
        str = "The socket operation failed because the application lacked the required privileges.";
        break;
    case QAbstractSocket::SocketResourceError: // 4
        str = "The local system ran out of resources (e.g., too many sockets).";
        break;
    case QAbstractSocket::SocketTimeoutError: // 5
        str = "The socket operation timed out.";
        break;
    case QAbstractSocket::DatagramTooLargeError: // 6
        str = "The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).";
        break;
    case QAbstractSocket::NetworkError: // 7
        str = "An error occurred with the network (maybe the network cable was accidentally plugged out).";
        break;

    default:
        str = "The following error occurred: \n";
        str += socket->errorString();
    }
// от
    qDebug() << "Socket ERROR !!! " << str ;
//  Делаем запись в лог через Q_INVOKABLE
//  emit Command_4_Parsing_Signal(str, socketDescriptor);

    //QMetaObject::invokeMethod()


} //nSocketDevState_Changed()
//++++++++++++++++++++++++++
// slot for signal void QObject::destroyed(QObject *obj = nullptr)
// while *obj is the QThread object, i.e. thread pointer
void QSocketThread::onDestroyedThread(QObject *threadDestroyed)
{
    QString str;
    str = "Thread destroyed ! ";
    str += QString("0x%1").arg((quintptr)threadDestroyed,
                               QT_POINTER_SIZE * 2, 16, QChar('0'));
    qDebug() << str ;
} // onDestroyedThread
//++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++
