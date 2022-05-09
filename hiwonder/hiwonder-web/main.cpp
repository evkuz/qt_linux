#include <QCoreApplication>
//#include "qsimpleserver.h"
#include "manipulator/qsimpleserver.h"
#include "mainprocess.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_NUMERIC,"C");

    // Запускаем основной поток, где обмен с TCP-сервером, роботом, запись в лог и т.д.
    MainProcess processor;


    // Запускаем TCP-сервер
    QSimpleServer server;
    return a.exec();
}
