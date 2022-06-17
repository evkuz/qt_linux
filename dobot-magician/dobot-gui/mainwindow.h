#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QFile>
#include <QtCore>
#include "serialdevice.h"
#include "robot.h"
//#include "../src/DobotDll.h"
#include "dobotcommands.h" // Communication Protocol

#include <QMainWindow>
#include <QtNumeric>
#include <numeric>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString str;
    QString     target_name; // Current Application name
    Robot *Dobot;
    unsigned char Servos [6] = {93,93,93,93,93,93};
    QByteArray dbuf;
    int dbtID; //

//    Pose mypose;
    float QByteArrayToFloat(QByteArray arr);

    dobotCommands *protocolDobot;
    int waitingCommandCode; // Код команды, которая ждет ответ робота.

#define parcel_size 8           // размер посылки в байтах от ПК к роботу

signals:

public slots:
    void GetData_Slot(QByteArray dbuf);  // Slot for Robot::GetData_Signal
    void getCooridnates_Slot(); //Slot for dobotCommands::getCooridnates_Signal;


private slots:
    void on_homePushButton_clicked();

    void on_SetNameButton_clicked();

    void on_GetPoseButton_clicked();

    void on_ReadDevNameButton_clicked();

    void on_GetPoseButton_2_clicked();

    void on_SpecPoseButton_clicked();

    void on_XAdd10Button_clicked();

    void on_GetHOMeParamsButton_clicked();

    void on_ToHOMeButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
