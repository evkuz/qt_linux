#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>
#include <QObject>
#include <QString>
#include <QFile>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(QObject *parent = nullptr);
    ~SerialDevice();
    //QString  Log_File_Name; // = "./srdevice.log";
    #define Log_File_Name "./srdevice.log"

    bool SerialIsOpened; //Флаг показывает, открыт ли Serial port
    QSerialPort serial;
    QFile       LogFile;
    QByteArray  qbuf;

    void Log_File_Open(QString lname);            // Открыть лог-файл
    void Write_To_Log (int value, QString log_message);

signals:
    void GetData_Signal(QByteArray  dbuf); // Передаем через сигнал указатель на массив
public slots:
    int Open_Port_Slot(QString portname); // https://doc.qt.io/qt-5/qserialport.html#SerialPortError-enum - список ошибок при открытии порта.
    void ReadFromSerial_Slot(); // Слот обработки сигнала readyRead()



};

#endif // SERIALDEVICE_H
