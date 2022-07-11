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
#include <QMutex>
#include <QMutexLocker>



class HiWonder : public QObject
{
    Q_OBJECT
public:
    //explicit HiWonder(QObject *parent = nullptr);
    HiWonder();
    ~HiWonder();

    #define serial_speed Baud115200
    #define robot_buffer_SIZE 512
    #define Log_File_Name        "./hiwonder.log"
    #define Serial_Output_File   "./serialData.log"
    #define SOURCE_POINTS_FILE  "../source_points.xls"
    #define DOF 6
    #define szData 8 //Размер посылки в байтах
    unsigned char outputData [szData];
    QSerialPort serial;
    QByteArray byInputBuffer[robot_buffer_SIZE];
    char byOutputBuffer[robot_buffer_SIZE];
    QFile       LogFile, SourceFile;

    QByteArray qbuf;
    bool MOVEMENT_DONE;  // Флаг показывает, что получено сообщение от робота о заврешении цикла движения

    bool SerialIsOpened; //Флаг показывает, открыт ли Serial port

    QList<QString> statuslst = { "wait", "init", "inprogress", "done", "NoDetection" };
    // Ответ робота состоит из 4 полей
//    QString current_status; // Текущий статус
    int current_st_index;   // Индекс текущего статуса в списке statuslst
    QString return_code;    // результат обработки запроса - понял/не понял
    QString active_command; // команда, которая сейчас исполняется
    QString comment;        // любые дополнительные данные

    QMutex hwMutex;         // mutex for HiWonder class

//    int writeTo(char *OutBuffer, int numbytes); // Запись данных из ПК в порт (роботу)
//    int readFrom(char *buf_data, int buf_size); // Считывает данные из порта в ПК (от робота)

    void Log_File_Open(QString lname);            // Открыть лог-файл
    void Source_Points_File_Open (QString fname); // Открыть файл для записи точек - это точно можно перенести в основной класс.

    void Write_To_Log (int value, QString log_message);
    void Write_To_Source(QString points_data); // Запись в файл координат кубика и 6 приводов в виде строки. Используется при создании ОС.

    void GoToPosition(QByteArray &position); //, const char *servo Оправляет данные для новой позиции приводов в порт (Роботу), уже чисто для робота ф-ция

    void Write_Status(QByteArray &status);  // Тоже надстройка над QSerialPort


private:
    QString current_status; // Текущий статус

public:
    QString GetCurrentStatus();
    void SetCurrentStatus(QString);

signals:
    void Moving_Done_Signal(); // Тоже надстройка над QSerialPort

//    void StatusChangedSignal(QString);


public slots:
    int Open_Port_Slot(QString portname); // https://doc.qt.io/qt-5/qserialport.html#SerialPortError-enum - список ошибок при открытии порта.
    void ReadFromSerial_Slot(); // Слот обработки сигнала readyRead()
    void SerialError_slot(QSerialPort::SerialPortError error); // Слот обработки сигнала
/*
 *     enum SerialPortError {
        NoError,
        DeviceNotFoundError,
        PermissionError,
        OpenError,
        ParityError,
        FramingError,
        BreakConditionError,
        WriteError,
        ReadError,
        ResourceError,
        UnsupportedOperationError,
        UnknownError,
        TimeoutError,
        NotOpenError
    };
*/
};

#endif // HIWONDER_H
