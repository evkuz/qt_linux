#include "qsimpleserver.h"

QSimpleServer::QSimpleServer(QObject *parent) :
    QTcpServer(parent)
{

    LISTENING = false;

}
//+++++++++++++++++++++++++++++++++++++++++++++
void QSimpleServer::startTCP()
{
    if (listen(QHostAddress::LocalHost, 8383)){
 //       qDebug() << "Listening...";
 //       emit Info_2_Log_Signal("Listening...");
        LISTENING = true;
    }
    else {
//        qDebug() << "Error while starting: " << errorString();
//        emit Info_2_Log_Signal("Error while starting: ...");
        LISTENING = false;
    }


}
//++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++  В конце генерирует сигнал newConnection(). Finally newConnection() is emitted
//++++++++++++++ Слот этого сигнала - &MainWindow::newConnection_Slot - дает роботу команду.
void QSimpleServer::incomingConnection(qintptr handle)
{
    // Тут надо передать дескриптор сокета в поток. Запустить поток.
    // Создавать сокет тут не надо.
    // Делаем это через сигнал/слот.
    emit Info_2_Log_Signal("I am entered incomingConnection");
    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);

    connect(socket, &QTcpSocket::readyRead, this, &QSimpleServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &QSimpleServer::onDisconnected);
}
//+++++++++++++++++++
void QSimpleServer::onReadyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender()); // Получили указатель на сокет
    //qDebug() << socket->readAll();
    str = QString(socket->readAll());

    emit Info_2_Log_Signal(str); // ОТправляем порцию данных в слот Info_2_Log_Slot

    QString response = "HTTP/1.1 200 OK\r\n\r\n%1";
    socket->write(response.arg(QTime::currentTime().toString()).toUtf8());
    socket->disconnectFromHost();
}
//+++++++++++++++++++++++++
void QSimpleServer::onDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender()); // Получили указатель на сокет
    socket->close();
    socket->deleteLater();
}
