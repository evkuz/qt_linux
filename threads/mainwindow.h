#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <stdint.h>
#include <QFile>
#include <QList>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include "hiwonder.h"  // hiwonder class
#include "SocketClient.h"
#include "robo_math.h" //Robo_Math class
#include "qsimpleserver.h"

//#include "mcinfer.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort serial;
    QByteArray *buff;

    QString     target_name;
    //char       *servos;    //unsigned char
    QByteArray LineEdits[6];

    QList<QLineEdit*> qle_list;
    QList<QSpinBox*> qspb_list;
    QList<QSlider*> slider_list;

    HiWonder *Robot;
    Robo_Math * RMath;


    bool new_get_request; // Флаг сигнализирует, что есть неотвеченный GET-запрос от webserver.
    //QTcpServer* m_pTCPServer;

    bool newYearMode; // Снимаем НГ-поздравление. Кое-где надо медленнее двигаться.
    int moving_mode;  // Режим движения (быстро/медленно)

    QSimpleServer server;

    //+++++++++++++++++++++++++++++ Threads +++++++++++++++
    int thread_counter ;

    QString rAnswer; // Ответ робота - статус, return_code, etc


#define parcel_size 8


// 7й байт

#define FORWARD_MV    0x31 // 49 // Движение "Туда"
#define BACKWAWARD_MV 0x30 // 48 // Движение "Обратно"
#define NEWYEAR_MV    0x35 // 53 // Движение в режиме "НГ" - медленно, задержка  передается отдельным байтом

// 8й байт
#define NOT_LAST    0xC8 //200  // Не последняя команда
#define LASTONE     0xDE //222  // Последняя команда роботу при комплексном движении
#define BEFORE_LAST 0xE9 //233  // Предпоследняя команда - положить кубик на тележку.
#define AFTER_PUT   0xF4 //244  Кубик на тележку положили, теперь грамотно убираем манипулятор.

    //int parcel_size; // размер посылки в байтах от ПК к роботу

    int X, Y;//Координаты x,y
    bool DETECTED; // Флаг, показывающий, сработал ли захват изображения.

    unsigned char Servos [6] = {93,93,93,93,93,93};

    void update_data_from_sliders(int index, int value);

    void GUI_Write_To_Log (int value, QString log_message); //Пишет в лог-файл номер ошибки value и сообщение message
    void try_mcinfer(int x, int y);
    void update_LineDits_from_servos(void);
    void update_LineDits_from_position(const char *pos);
    void update_LineDits_from_position(unsigned char *pos);
    void update_Servos_from_LineEdits(void);
    void send_Data(unsigned char thelast);
    void ssend_Data(QByteArray position);

    void make_json_answer();   // подготовка json-строки с полями ответа в TCP сокет.


private:


private:
    SocketClient readSocket;

public slots:
void Data_From_Web_SLot(QString message);
void Info_2_Log_Slot(QString);

void newConnection_Slot();
void server_New_Connect_Slot();

private slots:
    void on_openButton_clicked();

    void on_sitButton_clicked();

    void on_stand_upButton_clicked();

    void on_closeButton_clicked();


    void on_set_posButton_clicked();


    void on_socketButton_clicked();

    void on_clampButton_clicked();



    void on_servo_1_spinBox_valueChanged(int arg1);

    void on_servo_2_spinBox_valueChanged(int arg1);

    void on_servo_3_spinBox_valueChanged(int arg1);

    void on_servo_4_spinBox_valueChanged(int arg1);

    void on_servo_5_spinBox_valueChanged(int arg1);

    void on_servo_6_spinBox_valueChanged(int arg1);






    void on_getXYButton_clicked();
    void Return_EL_Slot(float EL);
    void Return_FW_Kinematic_XYZ_Slot(int X, int Y, int Z, float EL);
    void Pass_String_Slot(QString str);

    void on_submitButton_clicked();

    void on_trainButton_clicked();
    void Moving_Done_Slot(); // ОБработка сигнала окончания движения


    void TakeAndPutSlot();


    void on_getBackButton_clicked();

    void on_fixButton_clicked();

    void on_PUTButton_clicked();

    void on_GetBackFromServoButton_clicked();

    void on_fromFileButton_clicked();

signals:
    void Open_Port_Signal(QString portname); // Сигнал даем по нажатию кнопки "OPEN"
    void Pass_XY_Signal(int x_pix, int y_pix); //Сигнал по нажатию кнопки "Get_XY"
    void FW_Kinemaic_Signal(int S3, int S4, int S5, int l1, int l2, int l3); // Углы приводов, длины соответствующих звеньев.
    void Write_2_Client_Signal(QString); // Сигнал вебсерверу, - пересылка данных в сокет на отправку.
//    void StartTakeAndPutSignal();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
