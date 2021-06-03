#ifndef HIWONDER_H
#define HIWONDER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QDateTime>


class HiWonder : public QObject
{
    Q_OBJECT
public:
    //explicit HiWonder(QObject *parent = nullptr);
    HiWonder();
    ~HiWonder();

#define serial_speed Baud115200
#define accum_buffer_SIZE 64
#define Log_File_Name        "./hiwonder.log"

    char byInputBuffer[accum_buffer_SIZE];
    QFile       LogFile;



    int writeTo(char *OutBuffer, int numbytes); // Запись данных из ПК в порт (роботу)
    int readFrom(char *buf_data, int buf_size); // Считывает данные из порта в ПК (от робота)

    void Log_File_Open(QString lname);
    void Write_To_Log (int value, QString log_message);


signals:

public slots:
};

#endif // HIWONDER_H
