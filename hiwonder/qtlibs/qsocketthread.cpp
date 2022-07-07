#include "qsocketthread.h"
#include <QMultiMap>

QSocketThread::QSocketThread(int descriptror) :
    socketDescriptor(descriptror)
{
    socketDescriptor = descriptror;
    data_ready = false;
    current_status = "wait";
    toBeClosed = false;
    //ba.resize(icon)
}
//+++++++++++++++++++++++
QSocketThread::~QSocketThread()
{
    //Удаление объекта сокета
    delete socket;
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

        this->iconBA = &ba;
    return this->iconBA;

}
//+++++++++++++++++++++++++++++++++++++++
// Answer to "/favicon.ico HTTP/1.1" request
void QSocketThread::favIconAnswer()
{
    int value = 0x5544;
    QString response = "HTTP/1.1 200 OK\r\n";
    response += "content-type: image/png\r\n"; //text/html
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Content-Length: ";
    response += QString::number(228); //228*2 = 456; 318 *2 = 636
    response += "\r\n";
    response += "Connection: close";
    response += "\r\n\r\n";
   // QString img = returnIconChrome().toStdString();


    QByteArray ba = response.toUtf8();
    ba.append(returnIconChrome()->data());
//    response += "<html>";
//    response += "<head>";

//    response += "<link rel=\"icon\" href=\"data:,\">";

    //response += "<link href=\"data:image/x-icon;base64,AAABAAEAEBAQAAAAAAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAEhEQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD//wAA//8AAP//AAD//wAA//8AAP//AAD//wAA//8AAP7/AAD//wAA//8AAP//AAD//wAA//8AAP//AAD//wAA\" rel=\"icon\" type=\"image/x-icon\" />";

//    response += "</head>";
//    response += "<body>";
//    response += "IT'S   FAVICON FAVICON DATA !";
//    response += "</body>";

//    response += "</html>";
//    response += "\r\n";

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

    //int bytesWritten = socket->write(response.toUtf8());
    int bytesWritten = socket->write(ba);
    socket->disconnectFromHost();
    qDebug() << "The folowing " << bytesWritten << " bytes of data has been written to socket : \n" << response.toUtf8();

//    qDebug() << "From faviconAnswer" << returnIconChrome();
}//favIconAnswer()
//+++++++++++++++++++++++++++
////++++++++++++++++++ главный event loop потока
void QSocketThread::process_TheSocket()
{
    // eventloop тут не делаем, достаточно 1 сокета.
    //Создание объекта сокета
    socket = new QTcpSocket();
    //Сопоставление объекта сокета с системным сокетом через дескриптор
    socket->setSocketDescriptor(socketDescriptor);
    socket->setSocketOption(QAbstractSocket::LowDelayOption,1);

    emit this->isCreatedSocket_Signal(socket);

    //socket //keepalive
    //Соединение сигналов со слотами
    connect(socket, &QAbstractSocket::readyRead, this, &QSocketThread::onReadyRead, Qt::DirectConnection); //, Qt::QueuedConnection DirectConnection
    connect(socket, &QAbstractSocket::disconnected, this, &QSocketThread::onDisconnected, Qt::AutoConnection); //, Qt::AutoConnection

    //connect(socket, &QTcpSocket::stateChanged, this, &QSocketThread::onSocketDevState_Changed);

    connect(socket, &QAbstractSocket::errorOccurred , this, &QSocketThread::displayError);


    qDebug() << "$$$$$$$$$ "<< QThread::currentThread() << " $$$$$$$$$$ new socket $$$ " << QString::number(this->socketDescriptor);
     //Остановка потока на 1 сек (для иммитации долгого выполнения запроса)
    //msleep(200);
    // Вот тут ждем, пока "сверху" из MainProcess придет ответ, и этот ответ будет отправлен в по tcp
  //  while(isNotFinishedTCP){;}
    //Закрываем поток.
   //emit QThread::finished();

}
//+++++++++++++++++++++++++++++++++++++
//Данные считываем, отправляем на парсинг, готовим ответ.
void QSocketThread::onReadyRead()
{
    QList<QString>  strcommand = { "/run?cmd=", "/service?name=", "/status", "/status?action=", "/action?name="};
    //Чтение информации из сокета и вывод в консоль
    int socketNumber = this->socketDescriptor;


   // int value = 0xfafa;
    QString nextTcpdata, str;
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
//    QString theData = QString(httpHeaders);
    int realbytes = httpHeaders.size();

    qDebug() << "thread ID " <<  QThread::currentThread() << "!!!!!!!!!!!!!!!!!!!!! Have got " << realbytes << " bytes of Data FROM TCP SOCKET !!!!!!!!!!!!!!!!!!!"<< QString::number(socketNumber);
//    qDebug() << httpHeaders.size() << " bytes of data heas been read";
//    qDebug() << theData.size() << " bytes of data as QString heas been read";

    qDebug() << httpHeaders;// nextTcpdata;  httpHeaders;


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
    QString  wrong_mess = "/favicon.ico HTTP/1.1";
    if (message.contains(wrong_mess))
        {
           qDebug() << "Have got favicon request !";
           this->favIconAnswer();
           return;
       }
    bool matched = false;
    int i = 0;
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
//    qDebug() << "Now current Index value is" << i;
//    qDebug() << "Matched command sPosition is " << sPosition;
    if (i>=0) {
        qDebug() << "Matched string is " << strcommand.at(i);
    }
    else { //Строка не найдена
        qDebug() << "No MATCHING in onReadyRead";
        QString myresponse;
        myresponse = "{\r\n";
        myresponse += "{\"info\" : \"There is now action with such a name\"}";
        myresponse += "{\"rc\" : \"-100\"}";
        myresponse += "}";
//        socket->write(myresponse.toUtf8());
//        socket->disconnectFromHost();
        return;
    }

    searchstr = strcommand.at(i);



    if (matched)
    {
        sPosition += searchstr.size();
        ePosition = message.indexOf("&", sPosition);
        substr = message.mid(sPosition, (ePosition - sPosition));
        if(substr == "") substr = searchstr;

        // Получили команду. Передаем её наверх
        qDebug() << "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSocketThread::onReadyRead(), i.e. from TCP SOCKET " << QString::number(this->socketDescriptor);
        qDebug() << "************" << QThread::currentThread() << substr;
//        qDebug() << "Socket descriptor " << QString::number(this->socketDescriptor);

     emit Command_4_Parsing_Signal(substr, socketNumber); // works !

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
    emit this->finished();

    //this->deleteLater();

   //this->quit();
}
//+++++++++++++++++
// ПРишли данные от робота на отправку в сокет.
// Данные отправляем, сокет закрываем
// Тут важно попасть в свой поток...
void QSocketThread::Data_2_TcpClient_Slot(QString data, int socketNumber)
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
    data2Client = data.toUtf8(); // have got data from MainProcess as QByteArray
    QString response = "HTTP/1.1 200 OK\r\n";

//++++++++++++++++++++++++++ FAVICON ++++++++++++++++++++++++++++++++++++
    response += "content-type: text/html application/json\r\n"; //text/html
    response += "Access-Control-Allow-Origin: *\r\n";
//    response += "connection : close\r\n";
    response += "\r\n";
//        response += "<html>";
//        response += "<head>";

//        response += "<title>MEGATESTING!!!!</title>";
        response += "<link rel=\"icon\" href=\"data:,\">";
        response += "\r\n";

//        response += "</head>";
    //    //response += "{\n\t\"status\":\"";
//        response += "<body>";
//        response += "IT'S HTML DATA !";

        QByteArray qbData = response.toUtf8();
        qbData += data2Client;
//        response += "</body>";
//        response += "</html>";
//++++++++++++++++++++++++++ END OF FAVICON ++++++++++++++++++++++++++++++++++++



//    response += "content-type: application/json\r\n";
//    response += "Access-Control-Allow-Origin: *\r\n";
//    response += "\r\n";

//    response += data2Client;

    if (!socket->isValid() || this->socket->state() != QAbstractSocket::ConnectedState){
       qDebug() << "!!!!!!!!!!!!!!!!!!!!! SOCKET IS NOT OPENED";
       int state = socket->state();
       qDebug() << "socket state value is " << state;
       return;

    }

    qDebug() << QThread::currentThread()  << "Current socket " << this->socketDescriptor << " state is " << this->socket->state();
    qDebug() << QThread::currentThread()  << "Going to SEND SEND SEND data to socket with an desctiptor " << QString::number(this->socketDescriptor);

    socket->write(qbData);
    // socket->write(response.toUtf8());
    // ВОт тут (дождаться отправки) происходит отсоединение сокета

    //Отсоединение от удаленнного сокета
    socket->disconnectFromHost();
    socket->waitForDisconnected();

    qDebug() << QThread::currentThread()  << "The folowing data has been written to socket : \n" << qbData;

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
        str = "The remote host closed the connection.";
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

    emit Command_4_Parsing_Signal(str, socketDescriptor);



} //nSocketDevState_Changed()
//++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++
