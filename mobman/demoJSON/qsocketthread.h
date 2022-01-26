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
#include <QDateTime>

// Пользовательский класс потока - создает поток на каждый входящий запрос на порт 8383 (сервер) от клиента и обслуживает этот запрос во вновь созданном потоке.
// Т.е. посылает "наверх" полученные данные для парсинга
// Только вместо public QThread делаем public QObject
class QSocketThread : public QObject
{
    Q_OBJECT
public:
    explicit QSocketThread(int descriptror, QObject *parent = nullptr);
    ~QSocketThread();

    QByteArray data2Client; // храним данные на отправку клиенту.
    bool data_ready; // Данные на отправку сформированы, можно отправлять.
    QString current_status;
signals:
    void finished();
    void Command_4_Parsing_Signal(QString);

public slots:
    // Слот, где будет event loop потока, т.е. вся обработка.
    // Это вместо run
    void process_TheSocket();
    //Слот обмена данными с сокетом
    void onReadyRead();
    //Слот закрытия сокета
    void onDisconnected();
    //Слот принятия данных на отправку в сокет.
    void Data_2_TcpClient_Slot(QString);


private:
    //Дескриптор сокета
    int socketDescriptor;
    //Объект сокета
    QTcpSocket* socket;
    //QTcpSocket* socketCV; // Socket for data exchange with computer vision device (web camera)
};

#endif // QSOCKETTHREAD_H
