#include "clientsocket.h"


// ОБъект Создается в потоке.
// создаем сокет, соединяемся с хостом.
clientSocket::clientSocket(QString ipaddress, quint16 port, QString tcprequest)
{
    myip = ipaddress;
    myport = port;
    request = tcprequest;
//    socketDEV = new QTcpSocket(this);
//    socketDEV->setSocketOption(QAbstractSocket::KeepAliveOption, true);

//    //Соединение сигналов со слотами
//    connect(socketDEV, &QIODevice::readyRead, this, &clientSocket::onReadyRead);//, Qt::QueuedConnection);
//    connect(socketDEV, &QAbstractSocket::disconnected, this, &clientSocket::socketDEV_onDisconnected_Slot,Qt::AutoConnection);

//    connect (this->socketDEV, &QTcpSocket::connected, this, &clientSocket::onDEVSocketConnected_Slot); // Send "status" command
//    //connect (this->socketDEV, &QTcpSocket::stateChanged, this, &MainWindow::onSocketDevState_Changed);

//    this->request = tcprequest;
//    socketDEV->connectToHost(ipaddress, port);


}
//+++++++++++++++++++++++++++++++++++++++++++++++
clientSocket::~clientSocket()
{
    ;
}
//+++++++++++++++++++++++++++++++++
// Слот сигнала &QIODevice::readyRead, т.е. пришли данные.
// Считываем ответ от сервера, пишем в лог.
void clientSocket::onReadyRead()
{
    QString nextTcpdata, str;
    int value = 0xf9f9;
    int befbytes = socketDEV->bytesAvailable();
        nextTcpdata = socketDEV->readAll();
    int realbytes = nextTcpdata.size();
    int afterbytes = socketDEV->bytesAvailable();

//    str = "Bytes before reading "; str += QString::number(befbytes); GUI_Write_To_Log(value, str);
//    str = QString::number(realbytes); str += " bytes has been readed"; GUI_Write_To_Log(value, str);

//    str = "Bytes after reading  "; str += QString::number(afterbytes); GUI_Write_To_Log(value, str);

    str = "!!!!!!!!!!!!!!!!! There are some  data from SOCKET device !!!!!!!!!!!!!!!!!!!!\n";
    str += nextTcpdata;

    emit Read_From_TcpClient_Signal(str);

}
//+++++++++++++++++++++++++++++++++
// Слот сигнала &QTcpSocket::connected
// So now we have connected, and the next is to send data to TCP-server
// The data for GET request are already in "tcprequest" variable
void clientSocket::onDEVSocketConnected_Slot()
{
    socketDEV->write(request.toUtf8());
    emit clientSocket::Write_2_TcpClient_Signal(request);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
// Слот сигнала &QAbstractSocket::disconnected
void clientSocket::socketDEV_onDisconnected_Slot()
{
    this->socketDEV->close();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
// Слот сигнала myThread::SendToTcp_Signal()
// Создаем сокет, задаем сигналы/слоты, подключаемся к хосту.
void clientSocket::SendToTcp_Slot()
{

    socketDEV = new QTcpSocket(this);
    socketDEV->setSocketOption(QAbstractSocket::KeepAliveOption, true);

    //Соединение сигналов со слотами
    connect(socketDEV, &QIODevice::readyRead, this, &clientSocket::onReadyRead, Qt::QueuedConnection);//, Qt::QueuedConnection);
    connect(socketDEV, &QAbstractSocket::disconnected, this, &clientSocket::socketDEV_onDisconnected_Slot,Qt::AutoConnection);

    connect (this->socketDEV, &QTcpSocket::connected, this, &clientSocket::onDEVSocketConnected_Slot); // Send "status" command
    //connect (this->socketDEV, &QTcpSocket::stateChanged, this, &MainWindow::onSocketDevState_Changed);

    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError") ;

    connect(socketDEV, &QAbstractSocket::errorOccurred, this, &clientSocket::displayError);

    socketDEV->connectToHost(myip, myport);

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
// Слот сигнала &QAbstractSocket::errorOccurred()
void clientSocket::displayError(QAbstractSocket::SocketError socketError)
{
    QString str;
    str = "Just Initialize";
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
             str = "The host was not found. Please check the ";
             str += "host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError: // 0
        str = "The connection was refused by the peer. ";
        str += "Make sure the fortune server is running,\n";
        str += "and check that the host name and port settings are correct.";
        break;
    default:
        str = "The following error occurred: \n";
        str += socketDEV->errorString();
    }

    emit socketErrorToLog_Signal(str);

}
