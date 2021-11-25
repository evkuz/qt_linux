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
//#include "SocketClient.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainProcess; }
QT_END_NAMESPACE

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

    //+++++++++++++++++++++++++++++ Threads +++++++++++++++
    int thread_counter ;

    QString rAnswer; // Ответ робота - статус, return_code, etc


#define parcel_size 8
#define FORWARD     0X31 //049
#define NOT_LAST    0xC8 //200  // Не последняя команда
#define LASTONE     0xDE //222  // Последняя команда роботу при комплексном движении
#define BEFORE_LAST 0xE9 //233  // Предпоследняя команда - положить кубик на тележку.
#define AFTER_PUT   0xF4 //244  Кубик на тележку положили, теперь грамотно убираем манипулятор.

    //int parcel_size; // размер посылки в байтах от ПК к роботу

    int X, Y;//Координаты x,y
    bool DETECTED; // Флаг, показывающий, сработал ли захват изображения.

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


private:
//    SocketClient readSocket;

public slots:
void Data_From_Web_SLot(QString message);
void Data_From_TcpClient_Slot(QString);

void newConnection_Slot();
void server_New_Connect_Slot();

private slots:
    void on_openButton_clicked();

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
