#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>

class clientSocket : public QObject
{
    Q_OBJECT
public:
//    explicit clientSocket(QObject *parent = nullptr);
    clientSocket(QString ipaddress, quint16 port, QString tcprequest);
    ~clientSocket();

    QTcpSocket *socketDEV;
    QString request; // GET request via socket. GLOBAL.
    QString myip;
    quint16 myport;


signals:
    void Write_2_TcpClient_Signal(QString);      // Дублирование в лог данных, ОТПРАВЛЕННЫХ в сокет
    void Read_From_TcpClient_Signal(QString);    // Дублирование в лог данных, ПОЛУЧЕННЫХ из сокета
    void socketErrorToLog_Signal(QString); // write to log socketError message

public slots:

    //Слот обмена данными с сокетом
    void onReadyRead();

    // Слот сигнала &QTcpSocket::connected
    void onDEVSocketConnected_Slot();

    // Слот сигнала &QAbstractSocket::disconnected
    void socketDEV_onDisconnected_Slot();

    // Слот сигнала myThread::SendToTcp_Signal()
    void SendToTcp_Slot();
    // slot for QAbstractSocket::errorOccurred() signal
    void displayError(QAbstractSocket::SocketError); //


};

#endif // CLIENTSOCKET_H
