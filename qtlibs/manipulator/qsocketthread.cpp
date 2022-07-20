#include "qsocketthread.h"

QSocketThread::QSocketThread(int descriptror) :
    socketDescriptor(descriptror)
{
    data_ready = false;
    current_status = "wait";
}
//+++++++++++++++++++++++
QSocketThread::~QSocketThread()
{
    //Удаление объекта сокета
    delete socket;
}
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
    //socket //keepalive
    //Соединение сигналов со слотами
    connect(socket, &QAbstractSocket::readyRead, this, &QSocketThread::onReadyRead, Qt::DirectConnection); //, Qt::QueuedConnection
    connect(socket, &QAbstractSocket::disconnected, this, &QSocketThread::onDisconnected, Qt::AutoConnection); //, Qt::AutoConnection
    connect(socket, &QTcpSocket::stateChanged, this, &QSocketThread::onSocketDevState_Changed);

    connect(socket, &QAbstractSocket::errorOccurred , this, &QSocketThread::displayError);
     //Остановка потока на 1 сек (для иммитации долгого выполнения запроса)
    //msleep(200);
  //  while(true){;}
    //Запуск цикла обработки событий
   //emit finished();

}
//+++++++++++++++++++++++++++++++++++++
//Данные считываем, отправляем на парсинг, готовим ответ.
void QSocketThread::onReadyRead()
{
    QList<QString>  strcommand = { "/run?cmd=", "/service?name=", "/status", "/status?action="};
    //Чтение информации из сокета и вывод в консоль

    QByteArray qbmessage;
    qbmessage = socket->readAll();
    qDebug() << qbmessage;
   // qDebug() << "!!!!!!!!!!!!!!!!!!!!!11 Get Data FROM TCP SOCKET !!!!!!!!!!!!!!!!!!!1";

    //Парсим команду.
    QString message, substr, searchstr;
    message = QString(qbmessage);
    int sPosition, ePosition; // Индекс строки run в запросе.
    //sPosition = message.indexOf("/run?cmd=");
    bool matched = false;
    int i = 0;
    while (!matched and i< strcommand.size()){
        sPosition = message.indexOf(strcommand.at(i));
        if (sPosition != -1) {
             matched = true; qDebug() << "Inside sPosition is " << sPosition;
             qDebug() << "Inside Index is " << i;
        }
        ++i;
    }
    qDebug() << "Index value is" << i--;
    qDebug() << "Now current Index value is" << i;
    qDebug() << "Matched command sPosition is " << sPosition;
    if (i>=0) {qDebug() << "Matched string is " << strcommand.at(i);}
    else return;

    searchstr = strcommand.at(i);

   QString  wrong_mess = "/favicon.ico HTTP/1.1";

    if (!message.contains (wrong_mess))
    {
        sPosition += searchstr.size();
        ePosition = message.indexOf("&", sPosition);
        substr = message.mid(sPosition, (ePosition - sPosition));
        if(substr == "") substr = searchstr;

        // Получили команду. Передаем её наверх
        qDebug() << "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSocketThread::onReadyRead(), i.e. from TCP SOCKET !!!!!!!!!!!!!!!!!!!";
        qDebug() << substr;

     emit Command_4_Parsing_Signal(substr); // works !

  }
}

//++++++++++++++++++
// Слот сигнала &QAbstractSocket::disconnected
// Дождались отправки всех данных.
// Закрваем сокет, завершаем поток.
void QSocketThread::onDisconnected()
{
    //Закрытие сокета
    socket->close();
    //Завершение потока
    emit this->finished();
    //this->deleteLater();

   //this->quit();
}
//+++++++++++++++++
// ПРишли данные от робота на отправку в сокет.
// Данные отправляем, сокет закрываем
void QSocketThread::Data_2_TcpClient_Slot(QString data)
{
    // Готовим ответ.
    //socket->write(response.arg(QTime::currentTime().toString()).toLatin1());
    data2Client = data.toUtf8();
    QString response = "HTTP/1.1 200 OK\r\n";
    response += "content-type: application/json\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "\r\n";
    //response += "{\n\t\"status\":\"";
    response += data2Client;
    //response += "\"\n}";

    qDebug() << "Going to send data to socket device";
    if (!socket->isOpen()){
       qDebug() << "!!!!!!!!!!!!!!!!!!!!! SOCKET IS NOT OPENED";
       int state = socket->state();
       qDebug() << "socket state value is " << state;

    }
    socket->write(response.toUtf8());
    // ВОт тут (дождаться отправки) происходит отсоединение сокета

    //Отсоединение от удаленнного сокета
    socket->disconnectFromHost();

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
void QSocketThread::onSocketDevState_Changed()
{
    QString str = "sdfsfg";
 //   int value = 0x9999;
    qDebug() << "SocketDev state changed \n";
    qDebug() << "Current socketDev state is " << socket->state();
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

    emit Command_4_Parsing_Signal(str);



} //nSocketDevState_Changed()
//++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++
