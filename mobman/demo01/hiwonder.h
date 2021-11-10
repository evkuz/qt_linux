/*
 * Класс hiwonder - для объекта робот. Написан под робота LeArm или, в более поздней версии, - Hiwonder.
 *
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
#define Log_File_Name        "./hiwonder.log"
#define SOURCE_POINTS_FILE  "../source_points.xls"
#define DOF 4     // mobman
#define szData 8 //Размер посылки в байтах
    unsigned char outputData [szData];
    QSerialPort serial;
    QByteArray byInputBuffer[robot_buffer_SIZE];
    char byOutputBuffer[robot_buffer_SIZE];
    QFile       LogFile, SourceFile;

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
    void Source_Points_File_Open (QString fname);

    void Write_To_Log (int value, QString log_message);
    void Write_To_Source(int value, QString points_data); // Запись в файл координат кубика и 6 приводов в виде строки.

    void GoToPosition(QByteArray &position); //, const char *serhttps://www.motoman.com/en-us/products/robots/educationvo Оправляет данные для новой позиции приводов в порт (Роботу)

    void Write_Status(QByteArray &status);

private:
    //QString current_status;

public:
    QString GetCurrentStatus() { return this->current_status; }
    void SetCurrentStatus(QString);

signals:
    void Moving_Done_Signal();
//    void StatusChangedSignal(QString);


public slots:
    void Open_Port_Slot(QString portname);
    void ReadFromSerial_Slot();
};

#endif // HIWONDER_H
