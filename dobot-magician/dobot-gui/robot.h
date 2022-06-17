#ifndef ROBOT_H
#define ROBOT_H


#include "serialdevice.h"

class Robot : public SerialDevice
{
public:
    explicit Robot(QObject *parent = nullptr);
    ~Robot();
    int robot_buffer_SIZE ;  // Размер буфера Serial порта
    int DOF;   //6           // Число звеньев манипулятора
    int szData;// 8          // Размер посылки Serial порта в байтах
    int devID;

    bool MOVEMENT_DONE;  // Флаг показывает, что получено сообщение от робота о заврешении цикла движения
    void GoToPosition(QByteArray &position); // Оправляет данные для новой позиции приводов в порт (Роботу)
    // В данном роботе более точна ф-ция SendData, т.к. GoToPosition была расчитана на углы в манипуляторе HiWonder,
    // а в роботе Magician другая система координат. Либо сделаем virtual overload...
    void SendData(QByteArray &position);

    struct dbtCommand {
        int header;
        int len;
        int payload_ID;
        int payload_RW;
        int payload_isQueued;
        int checksum;


    }; // dbtCommand

    int getCRC(dbtCommand*); //вычисляет CRC по данным структуры команды.

};

#endif // ROBOT_H
