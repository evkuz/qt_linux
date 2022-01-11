#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <iostream>
#include <QDataStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
#define Log_File_Name    "./cv-checker.log"
#define CVDev_IP         "192.168.1.201"  //"192.168.1.201"
#define CVDev_Port       5001
#define ARM_Port         8383


    QString     target_name;
    QTcpSocket *socketCV = nullptr;
    QTcpSocket *socketARM = nullptr;

    QFile       LogFile;

    QString currentTcpdata; //Нужно, чтоб была глобальная.
    QDataStream in; // НА считывание данных из сокета CV


    void Log_File_Open(QString lname);
    void GUI_Write_To_Log (int value, QString log_message);

public slots:
    void onSocketConnected_Slot();
    void onSocketReadyRead_Slot();

    void onARMSocketConnected_Slot();
    void onARMSocketReadyRead_Slot();


private slots:
    void on_GetDistanceButton_clicked();

    void on_GetStatusButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
