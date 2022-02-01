#include "qsocketthread.h"

QSocketThread::QSocketThread(int descriptror, QObject *parent) :
    QObject(parent), socketDescriptor(descriptror)
{
    data_ready = false;
    current_status = "wait";
}
//+++++++++++++++++++++++
QSocketThread::~QSocketThread()
{
    //Удаление объекта сокета
    delete socket;
}
//+++++++++++++++++++++++++++
////++++++++++++++++++ главный event looop потока
void QSocketThread::process_TheSocket()
{
    //Создание объекта сокета
    socket = new QTcpSocket();
    //Сопоставление объекта сокета с системным сокетом через дескриптор
    socket->setSocketDescriptor(socketDescriptor);

    //Соединение сигналов со слотами
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()),Qt::QueuedConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()),Qt::AutoConnection);

     //Остановка потока на 1 сек (для иммитации долгого выполнения запроса)
    //msleep(200);
  //  while(true){;}
    //Запуск цикла обработки событий
   //emit finished();

}
//+++++++++++++++++++++++++++++++++++++
//Данные считываем, готовим ответ.
// Слот сигнала readyRead
void QSocketThread::onReadyRead()
{
    QList<QString>  strcommand = { "/run?cmd=", "/service?name=", "/status?action="};

    //Чтение информации из сокета и вывод в консоль
    QByteArray qbmessage;
    qbmessage = socket->readAll();
    qDebug() << qbmessage;
   // qDebug() << "!!!!!!!!!!!!!!!!!!!!!!! Get Data FROM TCP SOCKET !!!!!!!!!!!!!!!!!!!!";

    //Парсим команду.
    QString message, substr, searchstr;
    message = QString(qbmessage);
    int sPosition, ePosition; // Индекс строки run в запросе.

    bool matched = false;
    int i = 0;
    while (!matched and i< strcommand.size()){
        sPosition = message.indexOf(strcommand.at(i));
        if (sPosition != -1) {matched = true; qDebug() << "Inside sPosition is " << sPosition;
             qDebug() << "Inside Index is " << i;
        }
        ++i;
    }
    qDebug() << "Index value is" << i--;
    qDebug() << "Matched command sPosition is " << sPosition;
    if (i>=0) {qDebug() << "Matched string is " << strcommand.at(i);}
    // Теперь идем по индексам strcommand, перебираем все подряд, пока не найдем совпадение.

//    for ( int i =0; i< strcommand.size();  ++i)
//    {
//         sPosition = message.indexOf(strcommand.at(i));
//         if (sPosition != -1) {return;} //Нашли, выходим.

//    }

//    searchstr = "/run?cmd=";
//    sPosition = message.indexOf(searchstr);

//    if (sPosition == -1) {// нет строки "/run?cmd=", ищем другую - про сервисы

//        searchstr = "/service?name=";

//        sPosition = message.indexOf(searchstr);

//    }
   QString  wrong_mess = "/favicon.ico HTTP/1.1";

    if (!message.contains (wrong_mess))
    {
        sPosition += searchstr.size();
        ePosition = message.indexOf("&", sPosition);
        substr = message.mid(sPosition, (ePosition - sPosition));

        // Получили команду. Передаем её наверх
        qDebug() << "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSocketThread::onReadyRead(), i.e. from TCP SOCKET !!!!!!!!!!!!!!!!!!!";
        qDebug() << substr;

     emit Command_4_Parsing_Signal(substr); // works !

  }
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
//+++++++++++++++++
// ПРишли данные от робота на отправку в сокет.
// Слот сигнала Data_2_TcpClient_Signal
void QSocketThread::Data_2_TcpClient_Slot(QString data)
{
    // Готовим ответ.
    //socket->write(response.arg(QTime::currentTime().toString()).toLatin1());
    //QDateTime dt(QDateTime::currentDateTime());
   // qint64 tval;
    data2Client = data.toUtf8();
    QString response = "HTTP/1.1 200 OK\r\n";
    response += "content-type: application/json\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "\r\n";
    response += data;
    socket->write(response.toUtf8());

    //Отсоединение от удаленнного сокета
    socket->disconnectFromHost();

}
//++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++
