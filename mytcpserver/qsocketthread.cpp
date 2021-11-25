#include "qsocketthread.h"

QSocketThread::QSocketThread(int descriptror, QObject *parent) :
    QObject(parent), socketDescriptor(descriptror)
{
}

QSocketThread::~QSocketThread()
{
    //Удаление объекта сокета
    delete socket;
}
/*
 * If you want to sDescriptor an incoming connection as a
 * new QTcpSocket object in another thread you have to pass
 * the socketDescriptor to the other thread and create the
 * QTcpSocket object there and use its setSocketDescriptor()
 *  method.
*/

//++++++++++++++++++ главный event loop потока
void QSocketThread::process_TheSocket()
{
    //Создание объекта сокета
    socket = new QTcpSocket();
    //Сопоставление объекта сокета с системным сокетом через дескриптор
    socket->setSocketDescriptor(socketDescriptor);

    //Соединение сигналов со слотами
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()),Qt::QueuedConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()),Qt::AutoConnection);

}
//+++++++++++++++++++++++++++++++++++++
void QSocketThread::onReadyRead()
{
    //Чтение информации из сокета и вывод в консоль
    qDebug() << socket->readAll();

    //Шаблон ответа сервера
    QString response = "HTTP/1.1 200 OK\r\n\r\n%1";
    //Запись ответа в сокет
    socket->write(response.arg(QTime::currentTime().toString()).toLatin1());
    //Отсоединение от удаленнного сокета
    socket->disconnectFromHost();

}

//++++++++++++++++++

void QSocketThread::onDisconnected()
{
    //Закрытие сокета
    socket->close();
    //Завершение потока
    emit finished();
   //this->quit();
    // this->deleteLater();
}
