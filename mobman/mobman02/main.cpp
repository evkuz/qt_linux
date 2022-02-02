#include <QCoreApplication>
#include "QDebug"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Started";

    return a.exec();
}

