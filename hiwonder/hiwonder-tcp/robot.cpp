#include "robot.h"

Robot::Robot(QObject *parent) : SerialDevice(parent)
{
    MOVEMENT_DONE = true;
    //Log_File_Name = "mydevice.log";
    SerialDevice::qbuf.resize(robot_buffer_SIZE);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Robot::~Robot()
{
    ;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Robot::GoToPosition(QByteArray &position)
{
    QString str;
    this->MOVEMENT_DONE = false;
    position.resize (szData);
    serial.write(position);
    serial.waitForBytesWritten();

//    memcpy(&outputData, position,szData);
//// Данные роботу отправили, теперь запись в лог об этом

    str = "To Robot: ";
    str += QString();
    for (int i=0; i< szData; i++){
    //    str += QString::number(sData[i]);
       str+= QString::number(position.at(i));
////        str+= QString::number(outputData[i]);
       str+= ", ";

    }
    this->Write_To_Log(0xF001, str);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

