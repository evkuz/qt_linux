#ifndef QSIMPLESERVER_H
#define QSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>
#include <QTime>
#include <QObject>
#include "qsocketthread.h"

class QSimpleServer : public QTcpServer
{
    Q_OBJECT
public:
<<<<<<< HEAD
    //Конструктор
    explicit QSimpleServer(QObject *parent = nullptr);
    QThread   *thread_A;
    //Метод, вызываемый при подключении нового соединения
    void incomingConnection(qintptr sDescriptor) override;
=======

signals:
    void Info_2_Log_Signal(QString);

public slots:
<<<<<<< HEAD
//    void onReadyRead();
//    void onDisconnected();
    void Write_2_Client_SLot(QString);

    //Added by Miksarus
    void SetCurrentState(QString);
};

#endif // QSIMPLESERVER_H
