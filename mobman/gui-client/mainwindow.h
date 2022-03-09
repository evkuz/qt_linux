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

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "nlohmann/json.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
#define Log_File_Name    "./cv-checker.log"
#define CVDev_IP         "192.168.1.201"  // "159.93.44.146" - роутер в 240
#define CVDev_Port       5001             // 7575 - проброс
#define ARM_Port         8383


    QString     target_name;
    QTcpSocket *socketCV = nullptr;
    QTcpSocket *socketARM = nullptr;
    QTcpSocket *socketDEV = nullptr;


    QFile       LogFile;

    QString request; // GET request via socket

    QString currentTcpdata; //Нужно, чтоб была глобальная.
    QDataStream in; // НА считывание данных из сокета CV

    QJsonDocument jsnDoc;    // json-данные, полученные по tcp
    QJsonObject   jsnObj;    // ОБъект, хранящий весь JSON ответ от девайса
    QJsonObject   jsndataObj;// ОБъект, хранящий вложенный JSON-объект (вложенный в весь ответ) \
                             // \ Тут как раз данные о distance
    QJsonParseError jsonError; // ОШибка, если полученные данные - не JSON-объект

    ordered_json jsnAnswer;  // Ответ от девайса

    void Log_File_Open(QString lname);
    void GUI_Write_To_Log (int value, QString log_message);
    void makeSocket(QString ipaddress, quint16 port);
    void parseJSON(QString jsnData); // Парсинг JSON из HTTP
    void traversJson(QJsonObject json_obj); // Рекурсивный Парсинг JSON

public slots:
    void onSocketConnected_Slot();
    void onSocketReadyRead_Slot();

    void onARMSocketConnected_Slot();
    void onDEVSocketConnected_Slot();
    void onARMSocketReadyRead_Slot();
    void onDEVSocketReadyRead_Slot();

    void socketDEV_onDisconnected_Slot();

private slots:
    void on_GetDistanceButton_clicked();

    void on_GetStatusButton_clicked();

    void on_GetReadyButton_clicked();

    void on_GetParkingButton_clicked();

    void on_GetBoxButton_clicked();

    void on_GetServicesButton_clicked();

    void on_GetActionsButton_clicked();

    void on_NEW_GetStatusButton_clicked();

    void on_BeforeMovingButton_clicked();

    void on_PutBoxButton_clicked();

    void on_ClampButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
