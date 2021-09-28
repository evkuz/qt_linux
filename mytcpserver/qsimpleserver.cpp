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
    QSocketThread* thread = new QSocketThread(sDescriptor);
    //Соединение сигнала завершения потока с слотом отложенного удаления
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread_A, SIGNAL(started()), thread, SLOT(process_TheSocket()),Qt::QueuedConnection);

    thread->moveToThread(thread_A);
    //Запуск потока
    thread_A->start();
    //addPendingConnection(sDescriptor);
}
