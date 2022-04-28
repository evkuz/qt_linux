#ifndef MAINPROCESS_H
#define MAINPROCESS_H

#include <QObject>
#include <QFile>
#include <QtCore>
#include "serialdevice.h"
#include "robot.h"
#include "SocketClient.h"

class MainProcess : public QObject
{
    Q_OBJECT
public:
    explicit MainProcess(QObject *parent = nullptr);
    ~MainProcess();
//    SerialDevice *Nano;
    QString str;
    QString     target_name; // Current Application name
    Robot *HiWonder;
    unsigned char Servos [6] = {93,93,93,93,93,93};
    QByteArray dbuf;
#define parcel_size 8           // размер посылки в байтах от ПК к роботу
#define NOT_LAST    0xC8 //200  // Не последняя команда
#define LASTONE     0xDE //222  // Последняя команда роботу при комплексном движении
#define BEFORE_LAST 0xE9 //233  // Предпоследняя команда - положить кубик на тележку.
#define AFTER_PUT   0xF4 //244  Кубик на тележку положили, теперь грамотно убираем манипулятор.

signals:

public slots:
    void GetData_Slot(QByteArray dbuf);
};

#endif // MAINPROCESS_H
