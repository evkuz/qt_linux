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
#define  wifi_ADDRESS "192.168.1.175" // local PC address
#define  ipv4_ADDRESS "159.93.84.44"  // PC public address

    QThread   *thread_A;
    QSocketThread *tcpthread;
    quint16 tcpport = 8383;  // Порт, который слушает QTcpServer
    quint16 tcpportCV = 5001; // Port for data exchange with CV
    QByteArray tcpdata;
    //Метод, вызываемый при подключении нового соединения
    void incomingConnection(qintptr sDescriptor) override;

signals:
    // Сигнал на отправку данных наверх, уже роботу.
    void Data_From_TcpClient_Signal(QString);
    // Сигнал на отправку данных вниз, в сокет
    void Data_2_TcpClient_Signal(QString);


public slots:
    // Слот принятия строки для отправки клиенту в сокет
    // Обработчик сигнала сверху  Write_2_Client_Signal(QString)
    void Write_2_TcpClient_SLot(QString);
    //Слот отправки наверх команды на выполнение
    // Срабатывает по сигналу ommand_4_Parsing_Signal(QString);
    void Command_4_Parsing_Slot(QString);
    //Added by Miksarus
    void SetCurrentState(QString);

};

#endif // QSIMPLESERVER_H
