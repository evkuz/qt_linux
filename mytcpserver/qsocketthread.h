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

    //При наследовании от QThread, необходимо переопределить метод run,
    //где и размещается код, выполняемый в потоке
    //void run();
signals:
    void finished();

public slots:
    // Слот, где будет event loop потока, т.е. вся обработка.
    // Это вместо run
    void process_TheSocket();
    //Слот обмена данными с сокетом
    void onReadyRead();
    //Слот закрытия сокета
    void onDisconnected();
private:
    //Дескриптор сокета
    int socketDescriptor;
    //Объект сокета
    QTcpSocket* socket;
};

#endif // QSOCKETTHREAD_H
