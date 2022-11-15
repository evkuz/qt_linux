#ifndef WORKFLOW_H
#define WORKFLOW_H


/*
 *
 *
 *   Data_From_TcpClient_Slot -> "get_box" ->
 *
Видим полностью цикл обмена:
- Запрос пришел в потоке объекта QSimpleServer и MainProcess -  QThread(0x3f0980)
- Создаем новый объект класса QThread - это новый поток - thread_A ptr QThread(0xb3a04630)
- Создаем новый объект класса QSocketThread
- Переводим объект класса QSocketThread в поток QThread и далее там вся обработка
- Проводим обработку в потоке QThread - process_TheSocket  QThread(0xb3a04630)
  т.е.
       - создаем новый объект класса QTcpSocket : public QAbstractSocket;
       - соединяем сигналы класса QAbstractSocket со слотами.
       - По сигналу &QAbstractSocket::readyRead попадаем в слот QSocketThread::onReadyRead()
         onReadyRead thread ID  QThread(0xb3a04630) !!!!!!!!! Have got  163  bytes ...
         считываем команду, отправляем её на обработку
       - отправляем ответ Tcp-клиенту из ф-ции Data_2_TcpClient_Slot()
         %% Data for TCP-CLIENT %%%%%%%%%%%%%
         пишем в сокет.
         выводим в консоль QThread(0xb3a04630) Finished


QDateTime(2022-11-11 16:34:34.137 MSK Qt::LocalTime) New Incoming 2
************* QSimpleServer::incomingConnection QThread(0x3f0980)
********************** thread_A ptr QThread(0xb3a04630)
************ Now tcpthread lives in QThread(0xb3a04630)
$$$$$$$$$ process_TheSocket  QThread(0xb3a04630)  $$$$$$$$$$ new socket $$$  "9"
######### onReadyRead thread ID  QThread(0xb3a04630) !!!!!!!!! Have got  163  bytes of Data FROM TCP SOCKET !!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSocketThread::onReadyRead(), i.e. from TCP SOCKET  "9"
!!!!!!!!!!!!!!!!!!!!! QSocketThread::onReadyRead() QThread(0xb3a04630) "status"
%%%%%%%%%%%%%%%% Data for TCP-CLIENT %%%%%%%%%%%%%
The packet size in bytes is  "153"
QThread(0xb3a04630) Finished

НО после, программа еще раз заходит в слот отправки ответа Tcp-клиенту...

%%%%%%%%%%%%%%%% Data for TCP-CLIENT %%%%%%%%%%%%%
The packet size in bytes is  "262"
!!!!!!!!!!!!!!!!!!!!! SOCKET IS NOT VALID
QThread(0xb3a04630) Socket destroyed from client

socket state value is  0 "<The socket is not connected>"

Ведь поток QThread(0xb3a04630) уже Finished ...
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
Будем разбираться с потоками.
Наблюдаем явное дублирование завершения потока при создании коннектов сигнал/слот

1.    connect(tcpthread, &QSocketThread::stopThread_signal, tcpthread, &QSocketThread::deleteLater);
2. Закомментирован Вызов emit stopThread_signal(); - убрал
3. Была попытка организовать собственный eventLoop для потока через ф-цию
   QSocketThread::theLoop() <---> this->toBeClosed

 *
 */
#endif // WORKFLOW_H
