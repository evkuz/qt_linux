/*
 * Класс hiwonder - для объекта робот. Написан под робота LeArm или, в более поздней версии, - Hiwonder.
 * В этом проекте - Плата Arduino Mega - управление сервоприводами
*/

#ifndef HIWONDER_H
#define HIWONDER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>



class HiWonder : public QObject
{
    Q_OBJECT
public:
    //explicit HiWonder(QObject *parent = nullptr);
    HiWonder();
    ~HiWonder();

#define serial_speed Baud115200
#define robot_buffer_SIZE 32
#define Log_File_Name       "./hiwonder.log"
#define SOURCE_POINTS_FILE  "../source_points.xls"
#define DOF 4     // mobman
//#define szData 6 //Размер посылки в байтах
    int szData = 6;
    unsigned char outputData [6];
    QSerialPort serial;
    QByteArray byInputBuffer[robot_buffer_SIZE];
    char byOutputBuffer[robot_buffer_SIZE];
    QFile       LogFile, SourceFile;

    QByteArray qbuf;
    bool MOVEMENT_DONE;  // Флаг показывает, что получено сообщение от робота о заврешении цикла движения

    bool SerialIsOpened; //Флаг показывает, открыт ли Serial port


    QList<QString> statuslst = { "init", "wait", "inprogress", "done", "manipulator malfunction" }; //"manipulator malfunction" - SerialPort
    //    if (!SerialIsOpened) {str = "WARNING !!!! Serial port is NOT opened ! The data has NOT been sent."; this->Write_To_Log(0xF001, str); return;}

    // В этом классс свой список                1                       3
    QList<QString> actionlst = {"get_box", "nothing", "getposition", "ready", "parking", "formoving"};
    // Ответ робота состоит из 4 полей
    QString current_status; // Текущий статус
    int current_st_index;   // Индекс текущего статуса в списке statuslst
    QString return_code;    // результат обработки запроса - понял/не понял
    QString active_command; // команда, которая сейчас исполняется
    QString comment;        // любые дополнительные данные

    //QString CurrentState;

    int writeTo(char *OutBuffer, int numbytes); // Запись данных из ПК в порт (роботу)
    int readFrom(char *buf_data, int buf_size); // Считывает данные из порта в ПК (от робота)

    void Log_File_Open(QString lname);
    void Source_Points_File_Open (QString fname);

    void Write_To_Log (int value, QString log_message);
    void Write_To_Source(int value, QString points_data); // Запись в файл координат кубика и 6 приводов в виде строки.

    void GoToPosition(QByteArray &position); //, const char *serhttps://www.motoman.com/en-us/products/robots/educationvo Оправляет данные для новой позиции приводов в порт (Роботу)

    void Write_Status(QByteArray &status);

    struct ActionState {
        QString name;
        int     rc;    // return code
        QString info;  // text interpretation of return code
//         0 - action запущен -  "Is running"
//        -1 - action с таким именем не найден
//        -2 - action с таким именем не запустился, т.е. ошибка ?
//        -3 - action с таким именем уже запущен
//        -4 - action с таким именем не запущен (ожидание)

    };



    // НА каждый экшен свой - экземпляр ActionState
    ActionState getbox_Action;
    ActionState STAT_getbox_Action;
    ActionState parking_Action;
    ActionState ready_Action;
    ActionState forMoving_Action; //Get ready for moving accross

//Global for the whole robot
    struct CurrentState {
        QString state;
        QString action;

    };

CurrentState ManipulatorState;
private:
    //QString current_status;

// это ответ для "status?action=get_box"
struct StatusActionState {
    QString name;
    int     rc;    // "int - request result code",
    QString info;  // text interpretation of return code
    QString state; // "str - global device status: init | run | fail",
    QString action_list; // Список активных на данных моментэкшенов. И вот тут вопрос :
                         // Или как jsnDocument или как nlohmann::ordered_json;



//        - 0 - action запущен -  "Is running"
//        -1 - action с таким именем не найден
//        -2 - action с таким именем не запустился, т.е. ошибка ?
//        -3 - action с таким именем уже запущен
//        -4 - action с таким именем не запущен (ожидание)

};



public:
    QString GetCurrentStatus() { return this->current_status; }
    void SetCurrentStatus(QString);
    QString ActiveAction;

signals:
    void Moving_Done_Signal();
//    void StatusChangedSignal(QString);


public slots:
    void Open_Port_Slot(QString portname);
    void ReadFromSerial_Slot();
};

#endif // HIWONDER_H
