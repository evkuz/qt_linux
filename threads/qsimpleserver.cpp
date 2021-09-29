#include "qsimpleserver.h"

QSimpleServer::QSimpleServer(QObject *parent) :
    QTcpServer(parent)
{
    //Включает прослушивание сервером 80 порта
    if(listen(QHostAddress::LocalHost, tcpport)){
        qDebug() << "Listening...";
    //current_status = "wait";

    }
}

//++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++  В конце генерирует сигнал newConnection(). Finally newConnection() is emitted
//++++++++++++++ Слот этого сигнала - &MainWindow::newConnection_Slot - дает роботу команду.
void QSimpleServer::incomingConnection(qintptr sDescriptor)
{
    // Создание объекта потока QObject
    thread_A = new QThread;
    //Создание объекта потока для сокета
    QSocketThread* tcpthread = new QSocketThread(sDescriptor);
    //Соединение сигнала завершения потока с слотом отложенного удаления
    connect(tcpthread, SIGNAL(finished()), tcpthread, SLOT(deleteLater()));
    connect(thread_A, SIGNAL(started()), tcpthread, SLOT(process_TheSocket()),Qt::QueuedConnection);
    tcpthread->moveToThread(thread_A);

    connect(tcpthread, &QSocketThread::Command_4_Parsing_Signal, this, &QSimpleServer::Command_4_Parsing_Slot);
    connect(this, &QSimpleServer::Data_2_Client_Signal, tcpthread, &QSocketThread::Data_2_Client_Slot);
    //Запуск потока
    thread_A->start();
    //addPendingConnection(sDescriptor);
}
//++++++++++++++++
// Слот отправки данных от робота клиенту в сокет
void QSimpleServer::Write_2_Client_SLot(QString data)
{
    // Передаем данные дальше вниз в qtcpsocket
    emit Data_2_Client_Signal(data);
}
//++++++++++++
// ОТправляем команду наверх, роботу на выполнение.
void QSimpleServer::Command_4_Parsing_Slot(QString message)
{
  emit Info_2_Log_Signal(message);
}



//Added by Miksarus
void QSimpleServer::SetCurrentState(QString newStatus){
    //this->current_status = newStatus;
    ;
}
