#ifndef QSOCKETTHREAD_H
#define QSOCKETTHREAD_H

//Подсоединение используемых в коде классов
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTime>

//Пользовательский класс потока
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
    void Data_2_Client_Slot(QString);


private:
    //Дескриптор сокета
    int socketDescriptor;
    //Объект сокета
    QTcpSocket* socket;
};

#endif // QSOCKETTHREAD_H
