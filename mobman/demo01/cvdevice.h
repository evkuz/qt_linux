#ifndef CVDEVICE_H
#define CVDEVICE_H
/*
 * Класс объекта "Computer Vision Device" - камера и т.д.
 *
 *
 */
#include <QObject>
#include <QTcpSocket>


class CVDevice : public QObject
{
    Q_OBJECT
public:
    //explicit CVDevice(QObject *parent = nullptr);
    CVDevice(const char* hostaddress, int portnumber);
    ~CVDevice();

    void askCVDevice();
    QString currentTcpData;
signals:
// передаем данные "наверх"
void data_from_CVDevice_Signal(QString);

public slots:
    void readyRead_Slot(); // Слот сигнала readyRead()
    void socketConnected_Slot();
private:
    QTcpSocket *CVDtcpSocket = nullptr;
    QString CVDhostaddress;  // ip адрес сервера в виде "192.168.x.x"
    int CVDportnumber;  // Порт сервера

};

#endif // CVDEVICE_H
