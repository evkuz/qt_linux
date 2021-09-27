#include "qsimpleserver.h"

QSimpleServer::QSimpleServer(QObject *parent) :
    QTcpServer(parent)
{
    //Включает прослушивание сервером 80 порта
    if(listen(QHostAddress::LocalHost, 8383)){
        qDebug() << "Listening...";
    }
    else {
        qDebug() << "Error while starting: " << errorString();
    }
}

void QSimpleServer::incomingConnection(qintptr handle)
{
    //Создание объекта потока
    QSocketThread* thread = new QSocketThread(handle);
    //Соединение сигнала завершения потока с слотом отложенного удаления
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(started()), thread, SLOT(process_TheSocket()));
    //Запуск потока
    thread->start();
}
