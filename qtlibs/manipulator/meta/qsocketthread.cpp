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



    qDebug() << value << " Favicon Current socket " << this->socketDescriptor << " state is " << this->socket->state();
    qDebug() << "Going to SEND FAVICON FAVICON FAVICON data to socket with an desctiptor " << QString::number(this->socketDescriptor);

    int bytesWritten = socket->write(response.toUtf8());
//    int bytesWritten = socket->write(ba);
    socket->disconnectFromHost();
//    emit stopThread_signal();
    this->deleteLater();
    toBeClosed = true;

    qDebug().noquote() << "The folowing " << bytesWritten << " bytes of data has been written to socket :\n" << response.toUtf8();

    //    qDebug() << "From faviconAnswer" << returnIconChrome();
}
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


}//favIconAnswer()
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
    connect(socket, &QAbstractSocket::readyRead, this, &QSocketThread::onReadyRead, Qt::QueuedConnection); //, Qt::QueuedConnection DirectConnection
    connect(socket, &QAbstractSocket::disconnected, this, &QSocketThread::deleteLater); //, Qt::AutoConnection


    //connect(socket, &QAbstractSocket::disconnected,

    //connect(socket, &QTcpSocket::stateChanged, this, &QSocketThread::onSocketDevState_Changed);

    connect(socket, &QAbstractSocket::errorOccurred , this, &QSocketThread::displayError);




    qDebug() << "$$$$$$$$$ process_TheSocket "<< QThread::currentThread() << " $$$$$$$$$$ new socket $$$ " << QString::number(this->socketDescriptor);

}
//+++++++++++++++++++++++++++++++++++++
//Данные считываем, отправляем на парсинг, готовим ответ.
void QSocketThread::onReadyRead()
{
 //   QList<QString>  strcommand = { "/run?cmd=", "/service?name=", "/status", "/status?action=", "/action?name=", "/favicon"};
    //Чтение информации из сокета и вывод в консоль
    qintptr socketNumber = this->socketDescriptor;
//    qDebug() << QThread::currentThread() << "$$$$$$$$$ I'm in ReadyRead_Slot for socket " << socketNumber;
// вот тут запускаем таймер, чтобы дождаться favicon.

   // int value = 0xfafa;
    QString nextTcpdata;
    nextTcpdata = "";
//    int befbytes = socket->bytesAvailable();
//    if (befbytes < 400) {
//        qDebug() << "There are less than 400 bytes of available data !!!";
//        nextTcpdata = socket->readAll();
//        this->socket->abort();
//        this->socket->close();
//        this->toBeClosed = true;
//        //this->socket->waitForDisconnected();
//        //this->socket->disconnectFromHost();

//        return;
//    }
//    nextTcpdata += socket->readAll();
    // Output http-headers being received
    QByteArray httpHeaders = socket->readAll();
//    emit Command_4_Parsing_Signal(httpHeaders, socketNumber); // works !

//    QString theData = QString(httpHeaders);
    int realbytes = httpHeaders.size();

    qDebug() << "######### onReadyRead thread ID " <<  QThread::currentThread() << "!!!!!!!!! Have got " << realbytes << " bytes of Data FROM TCP SOCKET !!!!!!!!!!!!!!!!!!!";//<< QString::number(socketNumber);
//    qDebug() << QString::number(befbytes) << " bytes was available before reading";
//    qDebug() << httpHeaders.size() << " bytes of data heas been read";
//    qDebug() << theData.size() << " bytes of data as QString heas been read";

//    qDebug() << httpHeaders;// nextTcpdata;  httpHeaders;


//    QMap<QByteArray, QByteArray> headers;

    // Discard the first line ... Why ?
    //httpHeaders = httpHeaders.mid(httpHeaders.indexOf('\n') + 1).trimmed();
//    foreach(QByteArray line, httpHeaders.split('\n')) {
//        int colon = line.indexOf(':');
//        QByteArray headerName = line.left(colon).trimmed();
//        QByteArray headerValue = line.mid(colon + 1).trimmed();

//        headers.insert(headerName, headerValue);
//    }

//    QMap<QByteArray, QByteArray>::const_iterator ii = headers.constBegin();
//    while (ii != headers.constEnd()) {
//        qDebug() << ii.key() << ": " << ii.value() << Qt::endl;
//        ++ii;
//    }


//    int realbytes = nextTcpdata.size();
//    int afterbytes = socket->bytesAvailable();

//    qDebug() << "Bytes before reading from socket " << socketNumber << "available : " << QString::number(befbytes);
//    str = QString::number(realbytes); str += " bytes has been readed";

//    str = "Bytes after reading  "; str += QString::number(afterbytes);
//    qDebug() << str;


//    QByteArray qbmessage;
//    qbmessage = socket->readAll();
//    qDebug() << qbmessage;
//    qDebug() << "!!!!!!!!!!!!!!!!!!!!! Get Data FROM TCP SOCKET !!!!!!!!!!!!!!!!!!!"<< QString::number(socketNumber);

//    qDebug() << nextTcpdata;// nextTcpdata;  httpHeaders;
    //Парсим команду.
    QString message, substr, searchstr;
    //message = QString(qbmessage);
   // message = nextTcpdata;
    message = QString(httpHeaders);// nextTcpdata;
    int sPosition, ePosition; // Индекс строки run в запросе.
    //sPosition = message.indexOf("/run?cmd=");
    //   // Вот тут обходим запрос "GET /favicon.ico HTTP/1.1"
    QString  wrong_mess = "favicon.ico";//"Referer:";//"status";//"favicon.ico";
    if (message.contains(wrong_mess))
        {
           qDebug() << "\r\nHave got favicon request !\r\n";
           this->favIconAnswer();
           return;
       }
    bool matched = false;
    int i = 0; sPosition = 0;
    while (!matched and i< strcommand.size()){
        sPosition = message.indexOf(strcommand.at(i));
        if (sPosition != -1) {
             matched = true;
//             qDebug() << "Inside sPosition is " << sPosition;
//             qDebug() << "Inside Index is " << i;
        }
        ++i;
    }
    i--;

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

    searchstr = strcommand.at(i);


// Вот тут надо решать с "/action?name="
    if (matched)
    {
        sPosition += searchstr.size();
        ePosition = message.indexOf("&", sPosition);
        substr = message.mid(sPosition, (ePosition - sPosition));
        if(substr == "") substr = searchstr;

        if ((substr == "start")){ // || (substr == "get_box") Долгая операция, ставим на паузу прием новых соединений
            emit makePause_Signal(); // put on pause incomming connections
            pauseTimer->start(50); // 20msec should be enough
            //...
            // when the 20ms is being elapsed the &QTimer::timeout signal will be emitted and the slot
            // for this signal - &QSimpleServer::resumePause_Slot - will be invoked
            qDebug() << "&&&&&&&&&&&&&&&&& Timer STARTED &&&&&&&&&&&&&&&&&&&&&&&";
        }
        // Получили команду. Передаем её наверх
        qDebug() << "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSocketThread::onReadyRead(), i.e. from TCP SOCKET " << QString::number(this->socketDescriptor);
        qDebug() << "!!!!!!!!!!!!!!!!!!!!! " << QThread::currentThread() << substr;

        //        qDebug() << "Socket descriptor " << QString::number(this->socketDescriptor);

//        emit Command_4_Parsing_Signal(substr, socketNumber); // works !

//        QMetaObject::invokeMethod(_ptrMainThtread, "Data_FromTcp_Slot",
//                                  Qt::QueuedConnection,
//                                  Q_ARG(QString, substr));

        QMetaObject::invokeMethod(_ptrMainThtread, "Data_From_TcpClient_Slot",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, substr),
                                  Q_ARG(qintptr, socketNumber),
                                  Q_ARG(QObject*, this));

// И здесь же надо отправлять указатель this, чтобы из MainProcess вызвать Data_2_TcpClient_Slot

  }

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
void QSocketThread::Data_2_TcpClient_Slot(QString data, qintptr socketNumber)
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
    qDebug() << "%%%%%%%%%%%%%%%% QByteArray          %%%%%%%%%%%%%";
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
    qDebug() << "The packet size in bytes is " << QString::number(qbData.size());

//        response += "</body>";
//        response += "</html>";
//++++++++++++++++++++++++++ END OF FAVICON ++++++++++++++++++++++++++++++++++++



//    response += "content-type: application/json\r\n";
//    response += "Access-Control-Allow-Origin: *\r\n";
//    response += "\r\n";

//    response += data2Client;

    if (!socket->isValid() || this->socket->state() != QAbstractSocket::ConnectedState){
       qDebug() << "!!!!!!!!!!!!!!!!!!!!! SOCKET IS NOT VALID";
       int state = socket->state();
       qDebug() << "socket state value is " << state;

       //emit stopThread_signal();
       toBeClosed = true;
       //this->deleteLater();

       return;

    }
// Вот тут возникает разница между socketNumber и this->socketDescriptor, т.е. они разные.
//

//    qDebug() << QThread::currentThread()  << "Current socket " << socketNumber << " state is " << this->socket->state();
//    qDebug() << QThread::currentThread()  << "Going to SEND SEND SEND data to socket with an desctiptor " << QString::number(this->socketDescriptor);

    qint64 bufbytes = socket->write(qbData.constData());
    socket->waitForBytesWritten();
    // socket->write(response.toUtf8());
    // ВОт тут (дождаться отправки) происходит отсоединение сокета

    //Отсоединение от удаленнного сокета
    this->socket->disconnectFromHost();
    socket->deleteLater();
//    socketsCounter--;
//    emit decSocketCounter_Signal();
////    qDebug() << QThread::currentThread()  << "The folowing data has been written to socket : \n" << qbData;
    emit stopThread_signal();
    //this->deleteLater();
    toBeClosed = true;

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

//+++++++++++++++++++++++++++++++
