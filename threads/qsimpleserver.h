#ifndef QSIMPLESERVER_H
#define QSIMPLESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>
#include <QTime>
#include <QObject>

class QSimpleServer : public QTcpServer
{
    Q_OBJECT
public:
     QSimpleServer(QObject *parent = nullptr); //explicit
//    QSimpleServer();
//    ~QSimpleServer() override;
    QString str;
    bool LISTENING;
    void startTCP();

    QString current_status;

protected:
    void incomingConnection(qintptr handle) override;

signals:
    void Info_2_Log_Signal(QString);

public slots:
    void onReadyRead();
    void onDisconnected();
    void Write_2_Client_SLot(QString);

    //Added by Miksarus
    void SetCurrentState(QString);




};

#endif // QSIMPLESERVER_H
