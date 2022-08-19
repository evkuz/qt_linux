#include <QCoreApplication>
//#include "qsimpleserver.h"
#include "manipulator/qsimpleserver.h"
#include "mainprocess.h"
#include <QMetaObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_NUMERIC,"C"); //Нужно, иначе проблема с числами.

    // Запускаем основной поток, где обмен с TCP-сервером, роботом, запись в лог и т.д.
    QObject* ptrMainProcess = new MainProcess();

   // const QMetaObject* ptrMainProcess = processor.metaObject();

//    // Запускаем TCP-сервер
    QSimpleServer server(ptrMainProcess);

    // Отправка данных от сервера клиенту (в  ЦУП),
    // Чтение данных от клиента серверу (из ЦУП)
    // Все через iinvokeMethod

//    connect(processor, &MainProcess::Write_2_TcpClient_Signal, &server, &QSimpleServer::Write_2_TcpClient_SLot, Qt::QueuedConnection); // works ?

//    connect(&server, &QSimpleServer::Data_From_TcpClient_Signal, processor, &MainProcess::Data_From_TcpClient_Slot, Qt::QueuedConnection);



    return a.exec();
}
