/*
 *
 * Класс QSimpleServer - для объекта сервер QTcpServer
 *
*/
#ifndef QSIMPLESERVER_H
#define QSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>
#include <QTime>
#include <QObject>
#include <QString>
#include "qsocketthread.h"

//#define tcpport 8383;  // Порт, который слушает QTcpServer

class QSimpleServer : public QTcpServer
{
    Q_OBJECT
public:
    //Конструктор
    explicit QSimpleServer(QObject *parent = nullptr);

    // http://localhost:8383/run?cmd=status&
    // http://192.168.1.175:8383/run?cmd=status&
#define  wifi_ADDRESS "192.168.1.175"
#define  ipv4_ADDRESS "159.93.84.44"

    QThread   *thread_A;
    QSocketThread *tcpthread;
    quint16 tcpport = 8383;  // Порт, который слушает QTcpServer
    QByteArray tcpdata;
    bool isCreatedSocket; // socket creation flag
    QTcpSocket *createdSocket;
    //Метод, вызываемый при подключении нового соединения.
    virtual void incomingConnection(qintptr sDescriptor) override; // YES !!! override EXACTLY !

signals:
    // Сигнал на отправку данных наверх, уже роботу.
    void Data_From_TcpClient_Signal(QString, int socketNumber);
    // Сигнал на отправку данных вниз, в сокет
    void Data_2_TcpClient_Signal(QString, int socketNumber);


public slots:
    // Слот принятия строки для отправки клиенту в сокет
    // Обработчик сигнала сверху  Write_2_Client_Signal(QString)
    void Write_2_TcpClient_SLot(QString, int socketNumber);
    //Слот отправки наверх команды на выполнение
    // Срабатывает по сигналу ommand_4_Parsing_Signal(QString);
    void Command_4_Parsing_Slot(QString, int);

    //Слот получает указатель на вновь созданный сокет - обект QTcpSocket*
    void isCreatedSocket_Slot(QTcpSocket*); // Слот сигнала QSocketThread::isCreatedSocket_Signal(QTcpSocket);

};

#endif // QSIMPLESERVER_H
