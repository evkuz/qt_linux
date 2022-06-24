#include "mythread.h"

//myThread::myThread(QObject *parent) : QObject(parent)
myThread::myThread()
{
    // Вот тут можно сигналы/слоты прописать для обмена данными, ну, пока оставим.
    finthread = false;
    pause_thread = false;
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

    // Не совсем красиво, нужен таймер.
    while ((!this->finthread)) { // !!!event-loop!!!
        QThread::msleep(50);
        // Ждем 50мс, отправляем сигнал на передачу данных по tcp
        // Точнее, создаем объект класса clientSocket
        if (!this->pause_thread) emit SendToTcp_Signal();


    }
 emit finished();
} // myThread::A_SLOT()
