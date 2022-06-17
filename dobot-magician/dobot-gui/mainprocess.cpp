#include "mainprocess.h"
#include <thread>
#include "positions.h"

MainProcess::MainProcess(QObject *parent) : QObject(parent)

{
    int value = 0x0000;
    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    HiWonder = new Robot();
    HiWonder->Log_File_Open(Log_File_Name);
    QString str = "The application \"";  str +=target_name; str += "\"";
    HiWonder->Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));


    connect(HiWonder, &Robot::GetData_Signal, this, &MainProcess::GetData_Slot);

    //Инициализируем параметры конкретного робота. Основной класс не трогаем.

    HiWonder->robot_buffer_SIZE = 32;
    HiWonder->DOF = 4;
    HiWonder->szData = 7;

    //++++++++++++++++ Открываем порт
    int OKay = HiWonder->Open_Port_Slot("ttyUSB0");
    if (!OKay) { //!Robot->SerialIsOpened
        //Robot->current_st_index = 4;

       OKay = HiWonder->Open_Port_Slot("ttyUSB1");

    } // Robot->current_status = statuslst.at(4)

    if (!OKay){
                HiWonder->Write_To_Log(value, "SerialPort  PROBLEM !!!");
                // ТОгда таймер пускаем ???
    };
    //+++++++++++++++++++++++++++++

    HiWonder->Write_To_Log(value, "Waiting for command");
    QByteArray dd ;
    dd.resize(HiWonder->szData);
    memcpy(dd.data(), home_position, HiWonder->szData);
//    dd.insert(HiWonder->szData-2, 0x31);
//    dd.insert(HiWonder->szData-1, LASTONE);
//    QThread::sleep(2);
    HiWonder->GoToPosition(dd);
//    QThread::sleep(2);
//    dd.resize(HiWonder->szData);
//    memcpy(dd.data(), hwr_Start_position, HiWonder->DOF);
//    dd.insert(HiWonder->szData-2, 0x31);
//    dd.insert(HiWonder->szData-1, LASTONE);
//    HiWonder->GoToPosition(dd);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MainProcess::~MainProcess()
{
    HiWonder->Write_To_Log(0xf000, " The program is going to be closed\n");
    HiWonder->deleteLater();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Обработка данных, полученных по SerialPort
void MainProcess::GetData_Slot(QByteArray dbuf)
{

//  Этот код дублируется в классе SerialDevice
//    int value = 0xF002;
//    QString str;
//    str = "From HiWonder : ";
//    str += QString(dbuf);
//    HiWonder->Write_To_Log(value, str);

// Вот тут делаем обработку данных, полученных по Serial Port от робота
    str = QString(dbuf);
    QStringList list_str = str.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    str="DONE!";
    if (list_str.contains (str)) {HiWonder->MOVEMENT_DONE = true; HiWonder->Write_To_Log(0xF001, "Robot finished"); }
    //else HiWonder->Write_To_Log(0xF001, "Robot still running");


    str="LAST";
    if (list_str.contains (str)) {
        HiWonder->MOVEMENT_DONE = true;
        HiWonder->Write_To_Log(0xF001, "Robot finished complex command");
//                HiWonder->current_status = "done";
//                emit this->Moving_Done_Signal();
    }

}
