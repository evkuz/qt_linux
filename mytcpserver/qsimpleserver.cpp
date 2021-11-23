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

void QSimpleServer::incomingConnection(qintptr sDescriptor)
{
    // Создание объекта потока QObject
    thread_A = new QThread;
    //Создание объекта потока для сокета
    tcpthread = new QSocketThread(sDescriptor);

    //Соединение сигнала завершения потока со слотом отложенного удаления
    connect(tcpthread, SIGNAL(finished()), thread_A, SLOT(quit()));
    connect(tcpthread, SIGNAL(finished()), tcpthread, SLOT(deleteLater()));
    connect(thread_A, SIGNAL(started()), tcpthread, SLOT(process_TheSocket()),Qt::QueuedConnection);
    connect(thread_A, SIGNAL(finished()), thread_A, SLOT(deleteLater()));

    tcpthread->moveToThread(thread_A);
    //Запуск потока
    thread_A->start();
}
