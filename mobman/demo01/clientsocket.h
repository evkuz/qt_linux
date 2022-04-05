#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QString Hostip, int portnumber, QObject *parent = nullptr);
    ~ClientSocket();
    QString socketStatus;


signals:
    void Data_2_TcpServer_Signal(QString tcpData);

        void finished();

public slots:
    // Слот, где будет event loop потока, т.е. вся обработка.
    // Это вместо run
    void process_TheSocket();
    //Слот обмена данными с сокетом
    void onReadyRead_Slot();
    //Слот закрытия сокета
    void onDisconnected();
    void onSocketConnected_Slot(); // Слот обработки сигнала void QAbstractSocket::connected()

    void Data_2_TcpServer_Slot(QString tcpData);
private:
    QTcpSocket* socketCV; // Socket for data exchange with computer vision device (web camera)
    QString serverIP;
    int serverPort;
};


#endif // CLIENTSOCKET_H
