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
    QThread   *thread_A;
    QSocketThread *tcpthread;
    quint16 tcpport = 8383;  // Порт, который слушает QTcpServer
    QByteArray tcpdata;
    //Метод, вызываемый при подключении нового соединения
    void incomingConnection(qintptr sDescriptor) override;

signals:
    // Сигнал на отправку данных наверх, уже роботу.
    void Info_2_Log_Signal(QString);
    // Сигнал на отправку данных вниз, в сокет
    void Data_2_Client_Signal(QString);


public slots:
//    void onReadyRead();
//    void onDisconnected();
    // Слот принятия строки для отправки клиенту в сокет
    //сигнал сверху  Write_2_Client_Signal(QString)
    void Write_2_Client_SLot(QString);
    //Слот отправки наверх команды на выполнение
    void Command_4_Parsing_Slot(QString);
    //Added by Miksarus
    void SetCurrentState(QString);

};

#endif // QSIMPLESERVER_H
