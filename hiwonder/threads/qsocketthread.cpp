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
void QSocketThread::onReadyRead()
{
    //Чтение информации из сокета и вывод в консоль
    QByteArray qbmessage;
    qbmessage = socket->readAll();
    qDebug() << qbmessage;

    //Парсим команду.
    QString message, substr;
    message = QString(qbmessage);
    int sPosition, ePosition; // Индекс строки run в запросе.
    sPosition = message.indexOf("/run?cmd=");

   QString  wrong_mess = "/favicon.ico HTTP/1.1";

    if (!message.contains (wrong_mess))
    {
        sPosition += 9;
        ePosition = message.indexOf("&", sPosition);
        substr = message.mid(sPosition, (ePosition - sPosition));

        // Получили команду. Передаем её наверх

     emit Command_4_Parsing_Signal(substr);

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

void QSocketThread::Data_2_Client_Slot(QString data)
{
    // Готовим ответ.
    //socket->write(response.arg(QTime::currentTime().toString()).toLatin1());
    data2Client = data.toUtf8();
    QString response = "HTTP/1.1 200 OK\r\n";
    response += "content-type: application/json\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "\r\n";
    response += "{\n\t\"status\":\"";
    response += data2Client;
    response += "\"\n}";

    socket->write(response.toUtf8());

    //Отсоединение от удаленнного сокета
    socket->disconnectFromHost();

}
//++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++
