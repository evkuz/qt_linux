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

    isCreatedSocket = false;
    // Запускаем слушание порта
    if(listen(QHostAddress::AnyIPv4, tcpport)){
        qDebug() << "Listening port " << tcpport;
    //current_status = "wait";
    QTcpServer::setMaxPendingConnections(150);
    counterConnections = 0;


    }
}

//++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++  В конце генерирует сигнал newConnection(). Finally newConnection() is emitted
//++++++++++++++ Слот этого сигнала - &MainWindow::newConnection_Slot - дает роботу команду.
void QSimpleServer::incomingConnection(qintptr sDescriptor)
{

    this->counterConnections++;
    qDebug() << this->counterConnections;
    // А еще проверяем не превышен ли MaxPendingConnections...
//    if (this->counterConnections > this->maxPendingConnections()){
//        qDebug() << "The number of " << this->maxPendingConnections() << "TCP-connections exceeded";
//    }
    isCreatedSocket = false;
    // Создание объекта потока QObject
    thread_A = new QThread();
    //Создание объекта потока для сокета
    QSocketThread* tcpthread = new QSocketThread(sDescriptor);

    // move QObject to the thread
    tcpthread->moveToThread(thread_A);


    //Соединение сигнала завершения потока со слотом отложенного удаления
//    connect(tcpthread, &QSocketThread::finished, tcpthread, &QSocketThread::deleteLater);
    connect(tcpthread, &QSocketThread::stopThread_signal, thread_A, &QThread::quit);
    connect(tcpthread, &QSocketThread::stopThread_signal, tcpthread, &QSocketThread::deleteLater);
    connect(thread_A, &QThread::started, tcpthread, &QSocketThread::process_TheSocket, Qt::QueuedConnection); //, Qt::QueuedConnection)
    connect(thread_A, &QThread::finished, thread_A,  &QThread::deleteLater);


    connect(tcpthread, &QSocketThread::Command_4_Parsing_Signal, this, &QSimpleServer::Command_4_Parsing_Slot);
    connect(this, &QSimpleServer::Data_2_TcpClient_Signal, tcpthread, &QSocketThread::Data_2_TcpClient_Slot);

    connect(thread_A, &QThread::finished, this, &QSimpleServer::finshQThread_Slot, Qt::QueuedConnection);
 // Используем осторожно ! Потенциальный источник проблем...
 //   connect(tcpthread, &QSocketThread::isCreatedSocket_Signal, this, &QSimpleServer::isCreatedSocket_Slot); // get QTcpSocket *socket from tcpthread


    //Запуск потока
    thread_A->start();
//    this->dumpObjectTree();
//    this->dumpObjectInfo();
    // Нужно получить из tcpthread значение QTcpSocket *socket, чтобы вызвать ф-цию addPendingConnection(QTcpSocket *socket);
//    while (!isCreatedSocket) {
//    ;
//    }
//    this->addPendingConnection(sDescriptor);
}
//++++++++++++++++
// Слот отправки данных от робота клиенту в сокет (в ЦУП)
void QSimpleServer::Write_2_TcpClient_SLot(QString data, int socketNumber)
{
    // Передаем данные дальше вниз в qtcpsocket
    emit Data_2_TcpClient_Signal(data, socketNumber);
}
//++++++++++++
// ОТправляем команду наверх, роботу на выполнение.
void QSimpleServer::Command_4_Parsing_Slot(QString message, int socketNumber)
{
  emit Data_From_TcpClient_Signal(message, socketNumber); // works ???
//    qDebug() << "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSimpleServer->Command_4_Parsing_Slot !!!!!!!!!!!!!!!!!!!";
//    qDebug() << message;

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
void QSimpleServer::isCreatedSocket_Slot(QTcpSocket *theSocket)
{
    this->createdSocket = theSocket;
    this->isCreatedSocket = true;
    this->addPendingConnection(theSocket);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
void QSimpleServer::finshQThread_Slot()
{
    qDebug() << QThread::currentThread()  << "Is finished ";
}

//++++++++++++
//Added by Miksarus
//void QSimpleServer::SetCurrentState(QString newStatus){
//    //this->current_status = newStatus;
//    ;
//}
