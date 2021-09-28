#include "qsimpleserver.h"

QSimpleServer::QSimpleServer(QObject *parent) :
    QTcpServer(parent)
{
    //Включает прослушивание сервером 80 порта
    if(listen(QHostAddress::LocalHost, tcpport)){
        qDebug() << "Listening...";
    //current_status = "wait";

    }
//+++++++++++++++++++++++++++++++++++++++++++++
//void QSimpleServer::startTCP()
//{
//    if (listen(QHostAddress::LocalHost, 8383)){
 //       qDebug() << "Listening...";
 //       emit Info_2_Log_Signal("Listening...");
//    }
//    else {
//        qDebug() << "Error while starting: " << errorString();
 //   }
;
}
//+++++++++++++++++++++++++++++++++++++++++++++
//void QSimpleServer::startTCP()
//{
//    if (listen(QHostAddress::LocalHost, 8383)){
// //       qDebug() << "Listening...";
// //       emit Info_2_Log_Signal("Listening...");
//        LISTENING = true;
//    }
//    else {
////        qDebug() << "Error while starting: " << errorString();
////        emit Info_2_Log_Signal("Error while starting: ...");
//        LISTENING = false;
//    }


//}
//++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++  В конце генерирует сигнал newConnection(). Finally newConnection() is emitted
//++++++++++++++ Слот этого сигнала - &MainWindow::newConnection_Slot - дает роботу команду.
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
}//+++++++++++++++++++
//void QSimpleServer::onReadyRead()
//{
//    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender()); // Получили указатель на сокет
//    //qDebug() << socket->readAll();
//    str = QString(socket->readAll());

//    emit Info_2_Log_Signal(str); // ОТправляем порцию данных в слот Info_2_Log_Slot

//    QString response = "HTTP/1.1 200 OK\r\n\r\n%1";
//    socket->write(response.arg(QTime::currentTime().toString()).toUtf8());
//    socket->disconnectFromHost();
//}
//+++++++++++++++++++++++++
//void QSimpleServer::onDisconnected()
//{
//    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender()); // Получили указатель на сокет
//    socket->close();
//    socket->deleteLater();
//}
//    QString response = "HTTP/1.1 200 OK\r\n"; //"%1";//%1";  application/json
//    response += "content-type: application/json\r\n"; //text/html
//   // response += "Content-Length: %d\r\n";
//    response += "Access-Control-Allow-Origin: *\r\n";
//   // response += "\r\n\r\n";
//    response += "\r\n";
//    response += "{\n\t\"status\":\"";
//    response += current_status;
//    response += "\"\n}";
//    //response += "%1";

//   // socket->write(response.arg(QTime::currentTime().toString()).toUtf8());
//    socket->write (response.toUtf8 ());
//    socket->disconnectFromHost();
//}
//+++++++++++++++++++++++++
//void QSimpleServer::onDisconnected()
//{
//    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender()); // Получили указатель на сокет
//    socket->close();
//    socket->deleteLater();
//}
//+++++++++++++++++++++++++
// Слот отправки данных клиенту. Срабатыает по сигналу onWrite_2_Client_Signal
//
void QSimpleServer::Write_2_Client_SLot(QString message)
{
     QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender()); // Получили указатель на сокет
     QString response = "HTTP/1.1 200 OK\r\n\r\n%1";

     QString str = response;
   //  response +


 //        socket->write(response.arg(QTime::currentTime().toString()).toUtf8());
 //        socket->disconnectFromHost();



//     response += message;
//     const QByteArray sData;
//     //const QByteArray &sData
//     socket->write (response.toUtf8 ()); // QString QBYtEaRRAY
//     socket->disconnectFromHost();



}


//Added by Miksarus
void QSimpleServer::SetCurrentState(QString newStatus){
    //this->current_status = newStatus;
    ;
}
