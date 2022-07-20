/*
 * Класс QSocketThread - для объекта сокет. Создает новый поток при каждом новом соединении,
 * далее в этом потоке слушает сокет, принимает/отправляет данные из/в него (сокет)
 *
 *
*/

#ifndef QSOCKETTHREAD_H
#define QSOCKETTHREAD_H

//Подсоединение используемых в коде классов
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTime>
#include <QFile> // Будем записывать данные иконки из файла в строку, которую потом отправим в сокет
#include <QDataStream>
//Пользовательский класс потока
// Только вместо public QThread делаем public QObject
class QSocketThread : public QObject
{
    Q_OBJECT
public:
    //explicit QSocketThread(int descriptror, QObject *parent = nullptr);
    QSocketThread(int descriptror);
    ~QSocketThread();

    QByteArray data2Client; // храним данные на отправку клиенту.
    bool data_ready; // Данные на отправку сформированы, можно отправлять.
    bool toBeClosed; // Сокет без данных закрываем.
    QString current_status;
    QThread *threadID;
    //QFile iconFile; // file of icon
#define ICON_FILE_PATH "../favicon.png" // Путь к файлу иконки

    QByteArray* returnIconChrome(); // Данные из файла помещаем в строку, чтобы отправить в сокет.
    QByteArray* iconBA;
    void favIconAnswer();
    //mySocketError
signals:
    void stopThread_signal();
    void Command_4_Parsing_Signal(QString, int); // Отправляем данные (qstring) и номер сокета (int)

    // Сигнал о том, что сокет создан. Передаем указатель на него. для последующего QTcpServer::addPendingConnection()
    void isCreatedSocket_Signal(QTcpSocket*);
   // void socketErrorToLog_Signal(QString); // write to log socketError message

public slots:
    // Слот, где будет event loop потока, т.е. вся обработка.
    // Это вместо run
    void process_TheSocket();
    //Слот обмена данными с сокетом
    void onReadyRead();
    //Слот закрытия сокета
    void onDisconnected();
    //Слот принятия данных на отправку в сокет.
    void Data_2_TcpClient_Slot(QString, int socketNumber);
    // Слот сигнала изменения состояния сокета
    void onSocketDevState_Changed();

    // slot for QAbstractSocket::errorOccurred() signal
    void displayError(QAbstractSocket::SocketError); //


private:
    //Дескриптор сокета
    int socketDescriptor;
    //Объект сокета
    QTcpSocket* socket;

};

#endif // QSOCKETTHREAD_H
