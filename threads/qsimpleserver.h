#ifndef QSIMPLESERVER_H
#define QSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>
#include <QTime>
#include <QObject>
#include "qsocketthread.h"

//#define tcpport 8383;  // Порт, который слушает QTcpServer

class QSimpleServer : public QTcpServer
{
    Q_OBJECT
public:
    //Конструктор
    explicit QSimpleServer(QObject *parent = nullptr);
    QThread   *thread_A;

    quint16 tcpport = 8383;  // Порт, который слушает QTcpServer
    //Метод, вызываемый при подключении нового соединения
    void incomingConnection(qintptr sDescriptor) override;

signals:
    void Info_2_Log_Signal(QString);

public slots:
//    void onReadyRead();
//    void onDisconnected();
    void Write_2_Client_SLot(QString);

    //Added by Miksarus
    void SetCurrentState(QString);
};

#endif // QSIMPLESERVER_H
