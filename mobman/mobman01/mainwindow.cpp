#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "Started";
}

MainWindow::~MainWindow()
{
    qDebug() << "The program is going to be closed";
    delete ui;
}

void MainWindow::on_mainButton_clicked()
{
    qDebug() << "Button clicked";
}
