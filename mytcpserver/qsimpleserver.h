#ifndef QSIMPLESERVER_H
#define QSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>
#include "qsocketthread.h"


//Класс сервера
class QSimpleServer : public QTcpServer
{
    Q_OBJECT
public:
    //Конструктор
    explicit QSimpleServer(QObject *parent = nullptr);
    //Метод, вызываемый при подключении нового соединения
    void incomingConnection(qintptr handle) override;
signals:

public slots:

};

#endif // QSIMPLESERVER_H
