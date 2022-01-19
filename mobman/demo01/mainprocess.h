/*
 *
 * В GUI-версии это класс MainWindow, или класс основного потока, поэтому в текущей, консольной версии - класс MainProcess
 *
*/

#ifndef MainProcess_H
#define MainProcess_H

#include <QtCore>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <stdint.h>
#include <QFile>
#include <QList>
#include "hiwonder.h"  // hiwonder class
#include "qsimpleserver.h"
#include "cvdevice.h"


//#include "SocketClient.h"
//#include "jsoncpp/json/json.h"
//#include "${EXTLIBS}/nlohmann/json.hpp"
#include "nlohmann/json.hpp"
//#include <QJsonObject>
//#include <QJsonDocument>


#include <QTcpSocket>
//QT_BEGIN_NAMESPACE
//namespace Ui { class MainProcess; }
//QT_END_NAMESPACE


//
//using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

class MainProcess : public QObject
{
    Q_OBJECT

public:
    MainProcess(QObject *parent = nullptr);
    //MainProcess();
    ~MainProcess();
    QSerialPort serial;
    QByteArray *buff;

    QString     target_name;
    //char       *servos;    //unsigned char
    QByteArray LineEdits[6];

    HiWonder *Robot;
//    Robo_Math * RMath;


    bool new_get_request; // Флаг сигнализирует, что есть неотвеченный GET-запрос от webserver.
    //QTcpServer* m_pTCPServer;

    QSimpleServer server;
    QTcpSocket* socketCV;
    CVDevice* CVdevice;

    //+++++++++++++++++++++++++++++ Threads +++++++++++++++
    int thread_counter ;

    QString rAnswer; // Ответ робота - статус, return_code, etc

    ordered_json jsnStatus; // Ответ на запрос статуса в формате json
    ordered_json jsnAction;  // Ответ на команду Action в формате json
    //QJsonObject qjsnStatus;


//#define
#define FORWARD     0X31 //049
#define NOT_LAST    0xC8 //200  // Не последняя команда
#define LASTONE     0xDE //222  // Последняя команда роботу при комплексном движении
#define BEFORE_LAST 0xE9 //233  // Предпоследняя команда - положить кубик на тележку.
#define AFTER_PUT   0xF4 //244  Кубик на тележку положили, теперь грамотно убираем манипулятор.

//++++++++++++++++++++++ JSON data +++++++++++++++++++++++++++++++++++++++++++++

#define DEV_NAME "MOBMAN"   // device name - mobile manipulator
#define RC_SUCCESS 0        // запрос выполнен успешно
#define RC_WRONG_VALUE -1   // неверные параметры
#define RC_UNDEFINED -2     // action с таким именем не найден

#define AC_RUNNING 0        // action запущен
#define AC_WRONG_VALUE -1   // action с таким именем не найден
#define AC_FAILURE -2       // action с таким именем не запустился
#define AC_ALREADY_HAVE -3  // action с таким именем уже запущен

//+++++++++++++++++++++ CV device

#define CVDev_IP     "192.168.1.201"//"192.168.1.201"
#define CVDev_Port   5001

    QString currentTcpdata; //Нужно, чтоб была глобальная.
    QDataStream in; // НА считывание данных из сокета CV

    //int parcel_size; // размер посылки в байтах от ПК к роботу

    int X, Y;//Координаты x,y
    bool DETECTED; // Флаг, показывающий, сработал ли захват изображения.

    int parcel_size ;
    unsigned char Servos [DOF] = {93,93,93,93};

    //void update_data_from_sliders(int index, int value);

    void GUI_Write_To_Log (int value, QString log_message); //Пишет в лог-файл номер ошибки value и сообщение message
    void Servos_To_Log(QString message);
    void try_mcinfer(int x, int y);
//    void update_LineDits_from_servos(void);
//    void update_LineDits_from_position(const char *pos);
//    void update_LineDits_from_position(unsigned char *pos);
//    void update_Servos_from_LineEdits(void);
    void update_Servos_from_position(unsigned char *pos);

    void send_Data(unsigned char thelast);
    void make_json_answer();   // подготовка json-строки с полями ответа в TCP сокет.
    void init_json();
    void request_CV();
    void request_New_CV();
    int my_round(int n); // Округление целого числа до ближайшего кратного 10


private:
//    SocketClient readSocket;

public slots:
void Data_From_Web_SLot(QString message);
void Data_From_TcpClient_Slot(QString);

void newConnection_Slot();
void server_New_Connect_Slot();

void onSocketConnected_Slot(); // Слот обработки сигнала void QAbstractSocket::connected()
void CV_onReadyRead_Slot();    // Слот обработка сигнала readyRead()
void CV_onDisconnected();      // Слот обработки сигнала

void data_from_CVDevice_Slot(QString);

private slots:
    //void on_openButton_clicked();

    void on_sitButton_clicked();

    void on_stand_upButton_clicked();

    void on_closeButton_clicked();


    void on_set_posButton_clicked();


//    void on_socketButton_clicked();

    void on_clampButton_clicked();



    void on_getXYButton_clicked();
    void Return_EL_Slot(float EL);
    void Return_FW_Kinematic_XYZ_Slot(int X, int Y, int Z, float EL);
    void Pass_String_Slot(QString str);

//    void on_submitButton_clicked();

    void on_trainButton_clicked();
    void Moving_Done_Slot(); // ОБработка сигнала окончания движения


    //void TakeAndPutSlot();


//    void on_getBackButton_clicked();

//    void on_fixButton_clicked();

//    void on_PUTButton_clicked();

signals:
    void Open_Port_Signal(QString portname); // Сигнал даем по нажатию кнопки "OPEN"
    void Pass_XY_Signal(int x_pix, int y_pix); //Сигнал по нажатию кнопки "Get_XY"
    void FW_Kinemaic_Signal(int S3, int S4, int S5, int l1, int l2, int l3); // Углы приводов, длины соответствующих звеньев.
    void Write_2_TcpClient_Signal(QString); // Сигнал вебсерверу, - пересылка данных в сокет на отправку.
//    void StartTakeAndPutSignal();

};

#endif // MainProcess_H
