#include "mythread.h"

//myThread::myThread(QObject *parent) : QObject(parent)
myThread::myThread()
{
    // Вот тут можно сигналы/слоты прописать для обмена данными, ну, пока оставим.
}
//+++++++++++++++++++++++++++++++
myThread::~myThread()
{
 ;
}
//+++++++++++++++++++++++++++++++++++
// ОБработчик для thread_A::start()
void myThread::A_SLOT()
{
    while (true) {
        QThread::msleep(200);
        // Ждем 500мс, отправляем сигнал на передачу данных по tcp
        // Точнее, создаем объект класса clientSocket
        emit SendToTcp_Signal();


    }

//    QThread::msleep(500);
     // Ждем 500мс, отправляем сигнал на передачу данных по tcp
     // Точнее, создаем объект класса clientSocket
//     emit SendToTcp_Signal();

}
