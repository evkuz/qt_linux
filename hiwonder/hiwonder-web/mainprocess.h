/*
 *
 * В GUI-версии это класс MainWindow, или класс основного потока, поэтому в текущей, консольной версии - класс MainProcess
 *
*/

#ifndef MainProcess_H
#define MainProcess_H


#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <stdint.h>
#include <QFile>
#include <QList>
#include "hiwonder.h"  // hiwonder class
#include "qsimpleserver.h"
#include "SocketClient.h"
#include "jsoninfo.h"
#include "protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainProcess; }
QT_END_NAMESPACE

class MainProcess : public QObject
{
    Q_OBJECT

public:
    MainProcess(QObject *parent = nullptr);
    ~MainProcess();
    //QSerialPort serial;
    QByteArray *buff;
    QString     target_name;
    HiWonder *Robot;

    bool new_get_request; // Флаг сигнализирует, что есть неотвеченный GET-запрос от webserver.

    QSimpleServer server;

    //+++++++++++++++++++++++++++++ Threads +++++++++++++++
    int thread_counter ; // Было нужно при отладке старт/останов потоков

    QString rAnswer; // Ответ робота - статус, return_code, etc
    JsonInfo *jsnStore;
    QJsonObject mainjsnObj;


#define parcel_size 8           // размер посылки в байтах от ПК к роботу
#define NOT_LAST    0xC8 //200  // Не последняя команда
#define LASTONE     0xDE //222  // Последняя команда роботу при комплексном движении
#define BEFORE_LAST 0xE9 //233  // Предпоследняя команда - положить кубик на тележку.
#define AFTER_PUT   0xF4 //244  Кубик на тележку положили, теперь грамотно убираем манипулятор.

    float X, Y;//Координаты x,y центра объекта при распознавании
    bool DETECTED; // Флаг, показывающий, сработал ли захват изображения.

    unsigned char Servos [6] = {93,93,93,93,93,93};


    void GUI_Write_To_Log (int value, QString log_message); //Пишет в лог-файл (тот же, что и в классе HiWonder) номер ошибки value и сообщение message
    void try_mcinfer(float x, float y);
    void update_Servos_from_position(unsigned char *pos); // Фактически - это копирование { Servos[i] = pos[i]; }, либо memcpy(Servos, hwr_Start_position, DOF);

    void send_Data(unsigned char thelast);
    void make_json_answer();   // подготовка json-строки с полями ответа в TCP сокет.
    void init_json(); // Подготовка общего ответа на status
    void put_box();  //Положить кубик на пол
    void traversJson(QJsonObject json_obj); // Рекурсивный Парсинг JSON
    int getIndexCommand(QString myCommand, QList<QString> theList);  // Определяем индекс команды в списке MainProcess::tcpcommand

private:
    SocketClient readSocket;

public slots:
void Data_From_TcpClient_Slot(QString);


private slots:
    void on_clampButton_clicked(); // Пока используется
    void on_trainButton_clicked(); // Обработка команды "Start", т.е. взять кубик с одного места, положить в другое.
    void Moving_Done_Slot(); // ОБработка сигнала окончания движения

signals:
    void Open_Port_Signal(QString portname); // Сигнал даем по нажатию кнопки "OPEN"
    void Pass_XY_Signal(int x_pix, int y_pix); //Сигнал по нажатию кнопки "Get_XY"
    void FW_Kinemaic_Signal(int S3, int S4, int S5, int l1, int l2, int l3); // Углы приводов, длины соответствующих звеньев.
    void Write_2_TcpClient_Signal(QString); // Сигнал вебсерверу, - пересылка данных в сокет на отправку.
//    void StartTakeAndPutSignal();

};

#endif // MainProcess_H
