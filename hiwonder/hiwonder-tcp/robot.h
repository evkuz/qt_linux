#ifndef ROBOT_H
#define ROBOT_H


#include "serialdevice.h"

class Robot : public SerialDevice
{
public:
    explicit Robot(QObject *parent = nullptr);
    ~Robot();
    int robot_buffer_SIZE ;  // Размер буфера Serial порта
    int DOF;   //6              // Число звеньев манипулятора
    int szData;// 8              //Размер посылки Serial порта в байтах

    bool MOVEMENT_DONE;  // Флаг показывает, что получено сообщение от робота о заврешении цикла движения
    void GoToPosition(QByteArray &position); // Оправляет данные для новой позиции приводов в порт (Роботу)

};

#endif // ROBOT_H
