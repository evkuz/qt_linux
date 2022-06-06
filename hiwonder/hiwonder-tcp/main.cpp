#include <QCoreApplication>
#include "mainprocess.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Запускаем основной поток, где обмен с TCP-сервером, роботом через Serial port, запись в лог и т.д.
    MainProcess processor;


    return a.exec();
}
