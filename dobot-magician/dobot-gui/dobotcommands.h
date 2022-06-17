#ifndef DOBOTCOMMANDS_H
#define DOBOTCOMMANDS_H

#include <QObject>
#include <numeric>

#include <iostream>
using namespace std;

// Класс для работы с данными по протоколу Dobot-Communication-Protocol-V1.1.5
class dobotCommands : public QObject
{
    Q_OBJECT
public:
    explicit dobotCommands(QObject *parent = nullptr);
    float X, Y, Z, R; // Data from GetPose command
    float *XX;
    float *pX;
    uint64_t queuedCmdIndex;
    void parseData(QByteArray &data); // Парсим данные, полученные от робота
    float QByteArrayToFloat(QByteArray &arr);
    void floatToQByteArray(QByteArray *arr, float *val);

    unsigned char getCRC(unsigned char *arr, int size);
signals:
    void getCooridnates_Signal(); //Обновились данные координат xyzr - сигнал, что можно их считать из public-переменных.
};

#endif // DOBOTCOMMANDS_H
