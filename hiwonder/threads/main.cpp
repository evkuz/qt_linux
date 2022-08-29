#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setlocale(LC_NUMERIC,"C");
    MainWindow w;
    w.show();
//    QSimpleServer server;
    return a.exec();
}
