#include "robot.h"

Robot::Robot(QObject *parent) : SerialDevice(parent)
{
    MOVEMENT_DONE = true;
    //Log_File_Name = "mydevice.log";
    SerialDevice::qbuf.resize(robot_buffer_SIZE);
    devID = 0x1234;
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
    //position.resize (szData);
    serial.write(position);
    serial.waitForBytesWritten();

//  Данные роботу отправили, теперь запись в лог об этом

    str = "To Robot: ";
    str += QString();
    for (int i=0; i< position.size(); i++){
       str+= QString::number(position.at(i));
       str+= ", ";

    }
    str.remove(str.length()-2,2);
    this->Write_To_Log(0xF001, str);

}
//++++++++++++++++++++++++++++++++++++++++++++++
void Robot::SendData(QByteArray &position)
{
    serial.write(position);
    serial.waitForBytesWritten();

    QString str;
    str = "To Robot: ";
    str += QString();
    for (int i=0; i< position.size(); i++){
       str+= QString::number(position.at(i));
       str+= ", ";

    }
    str.remove(str.length()-2,2);
    this->Write_To_Log(0xF001, str);


}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int Robot::getCRC(dbtCommand *)
{
    return 0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

