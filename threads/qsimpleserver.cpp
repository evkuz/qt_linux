#include "qsimpleserver.h"

QSimpleServer::QSimpleServer(QObject *parent) :
    QTcpServer(parent)
{
    //Включает прослушивание сервером 8383 порта
    // server.listen(QHostAddress("127.0.0.1"),8888);
    // {
    // QHostAddress adr;
    // adr.setAddress("192.168.1.175");
    // if(listen(adr, tcpport)){
    // Так не слушает даже localhost, а только указанный ip
    // if(listen(QHostAddress("192.168.1.175"), tcpport))
    if(listen(QHostAddress::AnyIPv4, tcpport)){
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

    tcpthread->moveToThread(thread_A);

    //Соединение сигнала завершения потока со слотом отложенного удаления
    connect(tcpthread, SIGNAL(finished()), thread_A, SLOT(quit()));
    connect(tcpthread, SIGNAL(finished()), tcpthread, SLOT(deleteLater()));
    connect(thread_A, SIGNAL(started()), tcpthread, SLOT(process_TheSocket()),Qt::QueuedConnection);
    connect(thread_A, SIGNAL(finished()), thread_A, SLOT(deleteLater()));


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
  emit Data_From_TcpClient_Signal(message);
}



//Added by Miksarus
//void QSimpleServer::SetCurrentState(QString newStatus){
//    //this->current_status = newStatus;
//    ;
//}
