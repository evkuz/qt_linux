#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setlocale(LC_NUMERIC,"C"); //Нужно, иначе проблема с числами.
    MainWindow w;
    w.show();
    return a.exec();
}
