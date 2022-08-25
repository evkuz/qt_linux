#include "qsimpleserver.h"

QSimpleServer::QSimpleServer(QObject* ptrMainProcess,  QObject *parent) :
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
//    if(listen(QHostAddress("192.168.1.175"), tcpport)){
        QString str = "Listening address ";
        str += this->serverAddress().toString();
        str += " and port ";
        str += QString::number(tcpport);

        //qDebug() <<  <<  << "and port " << tcpport;
        qDebug() << str;
    //current_status = "wait";
    QTcpServer::setMaxPendingConnections(30);
    counterConnections = 0;
    ptrServerToMainProc = ptrMainProcess;
    clientsCounter = 0;

    QMetaObject::invokeMethod(ptrMainProcess, "GUI_Write_To_Log",
                              Q_ARG(int, 0x1111),
                              Q_ARG(QString, str));
 //   qDebug() << "QSimpleServer thread " << this->QThread::thread();
    }
    else {
        qDebug() << "ERROR WHILE TRY Listening port " << tcpport;

    }
}

//++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++  В конце генерирует сигнал newConnection(). Finally newConnection() is emitted
//++++++++++++++ Слот этого сигнала - &MainWindow::newConnection_Slot - дает роботу команду.
void QSimpleServer::incomingConnection(qintptr sDescriptor)
{

    this->counterConnections++;
    this->clientsCounter++;
    qDebug() << QDateTime::currentDateTime() << "New Incoming" << this->counterConnections;
    qDebug() << QDateTime::currentDateTime() << "Total processing connections" << this->clientsCounter;
    isCreatedSocket = false;
    // Создание объекта потока QObject
    thread_A = new QThread();
    qDebug() << "************* QSimpleServer::incomingConnection" << QThread::currentThread();
    qDebug() << "********************** thread_A ptr" << thread_A ;
    //Создание объекта потока для сокета
    QSocketThread* tcpthread = new QSocketThread(sDescriptor, this->ptrServerToMainProc, clientsCounter);

    // move QObject to the thread
    tcpthread->moveToThread(thread_A);

    qDebug() << "************ Now tcpthread lives in" << tcpthread->thread();


    //Соединение сигнала завершения потока со слотом отложенного удаления
//    connect(tcpthread, &QSocketThread::finished, tcpthread, &QSocketThread::deleteLater);
    connect(tcpthread, &QSocketThread::stopThread_signal, thread_A, &QThread::quit);
    connect(tcpthread, &QSocketThread::stopThread_signal, tcpthread, &QSocketThread::deleteLater);
    connect(thread_A, &QThread::started, tcpthread, &QSocketThread::process_TheSocket, Qt::QueuedConnection); //, Qt::QueuedConnection)
    connect(thread_A, &QThread::finished, thread_A,  &QThread::deleteLater);

    connect(tcpthread, &QSocketThread::makePause_Signal, this, &QSimpleServer::makePause_Slot, Qt::QueuedConnection);
    connect(tcpthread->pauseTimer, &QTimer::timeout, this, &QSimpleServer::resumePause_Slot, Qt::QueuedConnection);

    connect(tcpthread, &QSocketThread::decSocketCounter_Signal, this, &QSimpleServer::decSocketCounter_Slot, Qt::QueuedConnection);


//    connect(tcpthread, &QSocketThread::Command_4_Parsing_Signal, this, &QSimpleServer::Command_4_Parsing_Slot);
//    connect(this, &QSimpleServer::Data_2_TcpClient_Signal, tcpthread, &QSocketThread::Data_2_TcpClient_Slot);

//    connect(thread_A, &QThread::finished, this, &QSimpleServer::finshQThread_Slot, Qt::QueuedConnection);


    //Запуск потока
    thread_A->start();
//    qDebug() << "pointer to the event dispatcher object for the thread : " << thread_A->eventDispatcher();

    // Нужно получить из tcpthread значение QTcpSocket *socket, чтобы вызвать ф-цию addPendingConnection(QTcpSocket *socket);
//    while (!isCreatedSocket) {
//    ;
//    }
//    this->addPendingConnection(sDescriptor);
}
//++++++++++++++++
// Слот отправки данных от робота клиенту в сокет (в ЦУП)
/*
void QSimpleServer::Write_2_TcpClient_SLot(QString data, qintptr socketNumber)
{
    // Передаем данные дальше вниз в qtcpsocket
    emit Data_2_TcpClient_Signal(data, socketNumber);
}
//++++++++++++
// ОТправляем команду наверх, роботу на выполнение.
void QSimpleServer::Command_4_Parsing_Slot(QString message, qintptr socketNumber)
{
  emit Data_From_TcpClient_Signal(message, socketNumber); // works ???
//    qDebug() << "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSimpleServer->Command_4_Parsing_Slot !!!!!!!!!!!!!!!!!!!";
//    qDebug() << message;

}
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
void QSimpleServer::isCreatedSocket_Slot(QTcpSocket *theSocket)
{
//    this->createdSocket = theSocket;
    this->isCreatedSocket = true;
    this->addPendingConnection(theSocket);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
void QSimpleServer::finshQThread_Slot()
{
    qDebug() << QThread::currentThread()  << "Is finished ";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Ставим на паузу прием новых tcp-соединений.
void QSimpleServer::makePause_Slot()
{
    this->pauseAccepting();
    qDebug() << QDateTime::currentDateTime() << "&&&&&&&&&&&&&&&&& Timer Started Pause for Accpting ^^^^^^^^^^^^";

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Убираем паузу наприем новых tcp-соединений.

void QSimpleServer::resumePause_Slot()
{
    this->resumeAccepting();
    qDebug() << QDateTime::currentDateTime() << "&&&&&&&&&&&&&&&&& Timer FINISHED, Accepting resumed !!! &&&&&&&&&&&&&&&&&&&&&&&";
}
//+++++++++++++++++++++++++++++++++++++++++++++
void QSimpleServer::decSocketCounter_Slot()
{
    this->clientsCounter--;
    qDebug() << "Current connected socket counter is " << clientsCounter;
}

//++++++++++++
//Added by Miksarus
//void QSimpleServer::SetCurrentState(QString newStatus){
//    //this->current_status = newStatus;
//    ;
//}
