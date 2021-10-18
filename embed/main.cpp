#include <QCoreApplication>
#include "qsimpleserver.h"
#include "mainprocess.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // Запускаем TCP-сервер
    QSimpleServer server;
    // Запускаем основной поток, где обмен с TCP-сервером, роботом, запись в лог и т.д.
    MainProcess processor;
    return a.exec();
}
