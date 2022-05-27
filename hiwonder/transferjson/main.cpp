#include <QCoreApplication>
#include "mainprocess.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

        MainProcess processor;

    return a.exec();
}
