#include <QCoreApplication>
//#include "qsimpleserver.h"
#include "manipulator/meta/qsimpleserver.h"
#include "mainprocess.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    // Запускаем основной поток, где обмен с TCP-сервером, роботом, запись в лог и т.д.
//    MainProcess processor;


//    // Запускаем TCP-сервер
//    QSimpleServer server;

    setlocale(LC_NUMERIC,"C"); //Нужно, иначе проблема с числами.

    // Запускаем основной поток, где обмен с TCP-сервером, роботом, запись в лог и т.д.
    QObject* ptrMainProcess = new MainProcess();

//    // Запускаем TCP-сервер
    QSimpleServer server(ptrMainProcess);

    return a.exec();
}

