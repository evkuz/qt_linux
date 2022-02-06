#include "clientsocket.h"

ClientSocket::ClientSocket(QString Hostip, int portnumber, QObject *parent) : QObject(parent)
{
    socketStatus = "UNDEFINED";
    this->serverIP = Hostip;
    this->serverPort = portnumber;


}

ClientSocket::~ClientSocket()
{
 ;
}
//++++++++++++++++++++++++++ Слот сигнала start для потока
// Основной код потока - создаем сокет, подключаемся к хосту. Далее все события сокета обрабатываются соответствующими слотами.
void ClientSocket::process_TheSocket()
{

    QString str = "Going to create socket for CVDevice";
   // GUI_Write_To_Log(0xC1C1, str);
    qDebug() << str;
    socketCV = new QTcpSocket(this);
  //  socketCV->setSocketOption(QAbstractSocket::KeepAliveOption, true);
    //in.setDevice(socketCV);

    //Соединение сигналов со слотами                        было  CV_onReadyRead_Slot
    connect(socketCV, &QIODevice::readyRead, this, &ClientSocket::onReadyRead_Slot);//, Qt::QueuedConnection);
    connect(socketCV, SIGNAL(disconnected()), this, SLOT(CV_onDisconnected()),Qt::AutoConnection);

    connect (this->socketCV, &QTcpSocket::connected, this, &ClientSocket::onSocketConnected_Slot);
    socketCV->connectToHost(serverIP, serverPort);


}
//+++++++++++++++++++++++++++++++++++
// Слоте обработки сигнала "connected" для сокета.
// Формируем запрос в формате JSONю Отправляем запрос на сервер.
void ClientSocket::onSocketConnected_Slot()
{
    QString str = "CV socket connected !!!";
    qDebug() << str;


    str = "Current socket state is ";

    if (socketCV->state() == QTcpSocket::ConnectedState){str += " Connected State";}
    else {str += " Some OTHER than Connected State !!!!";}
    qDebug() << str;

    // А вот теперь готовим команду "/service?name=getposition" - единственное, что спрашиваем у CV
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

    qDebug() << "The following Data is going to be sent to CV :";
    qDebug() << request.toUtf8();
    socketCV->write(request.toUtf8());

    // Запрос серверу отправили.
    // Ответ от сервера в слоте onReadyRead_Slot()

}

void ClientSocket::Data_2_TcpServer_Slot(QString tcpData)
{
  ;
}
//++++++++++++++++++++++++++++++++++++++
//++++++++++++ Слот сигнала readyRead(), получаем данные из сокета, направляем на обработку.
void ClientSocket::onReadyRead_Slot()
{
    int value = 0xeeee;


    QString nextTcpdata, str;

int befbytes = socketCV->bytesAvailable();
    nextTcpdata = socketCV->readAll();
int afterbytes = socketCV->bytesAvailable();

str = "Bytes before reading "; str += QString::number(befbytes); qDebug() << str; //GUI_Write_To_Log(value, str);

str = "Bytes after reading  "; str += QString::number(afterbytes); qDebug() << str; //GUI_Write_To_Log(value, str);




}

void ClientSocket::onDisconnected()
{
    ;
}
