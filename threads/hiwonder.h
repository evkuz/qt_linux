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
#define Log_File_Name        "./hiwonder.log"
#define SOURCE_POINTS_FILE  "../source_points.xls"
#define COMMAND_LIST_FILE   "./command.lst"
//unsigned int  DOF;
//unsigned int  szData; //Размер посылки в байтах

static const int robot_buffer_SIZE = 32;
static const int DOF = 6;
static const int szData = 8;
    unsigned char outputData [szData];
    QSerialPort serial;
    char byOutputBuffer[robot_buffer_SIZE];
    QFile       LogFile, SourceFile, CommandFile;

    QByteArray qbuf;
    bool MOVEMENT_DONE;  // Флаг показывает, что получено сообщение от робота о заврешении цикла движения



    QList<QString> statuslst = { "wait", "init", "inprogress", "done" };
    // Ответ робота состоит из 4 полей
    QString current_status; // Текущий статус
    QString return_code;    // результат обработки запроса - понял/не понял
    QString active_command; // команда, которая сейчас исполняется
    QString comment;        // любые дополнительные данные

    int writeTo(char *OutBuffer, int numbytes); // Запись данных из ПК в порт (роботу)
    int readFrom(char *buf_data, int buf_size); // Считывает данные из порта в ПК (от робота)

    void Log_File_Open(QString lname);
    void Source_Points_File_Open (QString fname); // Файл точек, для ускорения набора, при создании выборки
    void Command_List_File_Open (QString lstname);// Файл списка позиций,
    // включая служебные байты, куда нужно попасть при комплексном движении

    void Write_To_Log (int value, QString log_message);
    void Write_To_Source(QString points_data); // Запись в файл обучающей выборки координат кубика и 6 приводов в виде строки. Для ускорения процесса.

    void GoToPosition(QByteArray &position); //, const char *servo Оправляет данные для новой позиции приводов в порт (Роботу)

    void Write_Status(QByteArray &status);


public:
    QString GetCurrentStatus() { return this->current_status; }
    void SetCurrentStatus(QString);

signals:
    void Moving_Done_Signal();


public slots:
    void Open_Port_Slot(QString portname);
    void ReadFromSerial_Slot(); // Слот сигнала QSerialPort::readyRead()
};

#endif // HIWONDER_H
