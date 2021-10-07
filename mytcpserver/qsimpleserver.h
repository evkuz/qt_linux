#ifndef QSIMPLESERVER_H
#define QSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>
#include "qsocketthread.h"


//Класс сервера - это "Controller" в терминах примера.
class QSimpleServer : public QTcpServer
{
    Q_OBJECT
public:
    //Конструктор
    explicit QSimpleServer(QObject *parent = nullptr);
    QThread   *thread_A;
    QSocketThread *tcpthread;

    // Метод, вызываемый при подключении нового соединения
    // Переопределяем его
    void incomingConnection(qintptr sDescriptor) override;

signals:

public slots:

};

#endif // QSIMPLESERVER_H
