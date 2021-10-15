#include <QCoreApplication>
#include "qsimpleserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // Запускаем TCP-сервер

    return a.exec();
}
