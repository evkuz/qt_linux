#include "dobotcommands.h"
/*
 * Класс для работы с командами робота в соответствии с :
 * Communication Protocol
 * Issue: V1.1.5
 * Date: 2019-08-05
 *
 *
 * */


dobotCommands::dobotCommands(QObject *parent) : QObject(parent)
{
    X = 0; Y = 0; Z = 0, R = 90;

}
//++++++++++++++++++++++++++++++++++
// Парсим пришедшие данные.
void dobotCommands::parseData(QByteArray &data)
{
    int sz = data.size();
    uint8_t payloadValue; // Число payload байт в посылке от робота.
    uint8_t crcByte; // Байт со значением crc
    uint8_t ID;   // Значение ID
    uint8_t Ctrl; // Ctrl

    QByteArray xyData;

    crcByte = data.at(sz-1);
    payloadValue = data.at(2)-2; // Получили количество байт в посылке от робота
    char Params [payloadValue];
    // Вот тут надо правильно скопировать данные из data в Params

    // Далее, по номеру команды извлекаем данные из Params в нужные переменные

    char code = data.at(3);     //Params[0]; // код команды
    //xyData.resize(4);

 unsigned char myData[4] = {0x43, 0x16, 0x35, 0x83};

quint32 temp, mytemp;
    switch (code) {
    case 0x0A: //10 GetPose
        // Вспоминаем, что данные внутри посылки содержатся в виде LittleEndian.
        // Для X==150,209 приходят LE-данные 0x83351643
        // Пока для тестов записываем в массив как BE
//      xyData.append(0x43);
//      xyData.append(0x16);
//      xyData.append(0x35);
//      xyData.append(0x83); // 0x83

//      xyData.append(0x83);
//      xyData.append(0x35);
//      xyData.append(0x16);
//      xyData.append(0x43); // 0x83


       temp = 0x43163583;

      //XX = reinterpret_cast<float*>(&temp);
      //XX = QByteArrayToFloat(xyData);
      xyData = data.mid(5,4);
      // НАдо в BigEndian Переделать.
      X = QByteArrayToFloat(xyData);
      xyData = data.mid(9,4);
      Y = QByteArrayToFloat(xyData);
      xyData = data.mid(13,4);
      Z = QByteArrayToFloat(xyData);
      xyData = data.mid(17,4);
      R = QByteArrayToFloat(xyData);
      //static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");
      //mytemp = xyData.at(0); mytemp <<= 24; mytemp = mytemp & 0xff000000;


      //        |((char)xyData[1] << 16)|((char)xyData[2] << 8)|(char)xyData[3];
      //pX = reinterpret_cast<float*>(&mytemp);
      emit getCooridnates_Signal();
     break;
     case 0x1E:// GetHOMEParams
        // Записываем параметры в массив/структуру
        // Нам пришло 16 байт для float значений X,Y,Z,R
        xyData = data.mid(5,4);
        // НАдо в BigEndian Переделать.
        X = QByteArrayToFloat(xyData);
        xyData = data.mid(9,4);
        Y = QByteArrayToFloat(xyData);
        xyData = data.mid(13,4);
        Z = QByteArrayToFloat(xyData);
        xyData = data.mid(17,4);
        R = QByteArrayToFloat(xyData);
        emit getCooridnates_Signal();

    break;
    case 0x54: // SetPTP command
        // 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // Надо бы распарсить, что там пришло.
        queuedCmdIndex = 0x04;
        break;
    default:
        X = 777.999;
       // emit getCooridnates_Signal();
    }

   // emit getCooridnates_Signal();
}
//++++++++++++++++++++++++++++++++++
// Возвращаем число, а не указатель на него.
// Если возвращать указатель происходит segmentation fault
// Массив приходит в формате Little Endian, поэтому сдвигаем данные начиная с конца массива.
float dobotCommands::QByteArrayToFloat(QByteArray &arr)
{ // Так не работает, не знаю, почему в stackoverflow лежит этот ответ.
  //  quint32 temp = ((char)arr[0] << 24)|((char)arr[1] << 16)|((char)arr[2] << 8)|(char)arr[3]; // Big endian
  //  quint32 temp = ((char)arr[0] << 24) + ((char)arr[1] << 16) + ((char)arr[2] << 8) + (char)arr[3]; // arr == Big Endian
    quint32 temp = ((char)arr[3] << 24) + ((char)arr[2] << 16) + ((char)arr[1] << 8) + (char)arr[0]; // arr == Little Endian

    //float* out = reinterpret_cast<float*>(&temp);

    return *reinterpret_cast<float*>(&temp);
}
//++++++++++++++++++++++++++++++++++
// Раскладыаем float число val как Little Endian значение в массив arr.
// Так требует Dobot-Communication-Protocol-V1.1.5
void dobotCommands::floatToQByteArray(QByteArray *arr, float *val)
{
    quint32 temp;
    char dt;

    // float value 0x437a0000 to QByteArray value AND pack the value as Little Endian
    temp = *reinterpret_cast<quint32*>(val);

    dt = (char)temp&0x000000ff;
    arr->append(dt);
    dt = (char)((temp>>8)&0x000000ff);
    arr->append(dt);
    dt = (char)((temp>>16)&0x000000ff);
    arr->append(dt);
    dt = (char)((temp>>24)&0x000000ff);
    arr->append(dt);

}
//++++++++++++++++++++++++++++++++++

unsigned char dobotCommands::getCRC(unsigned char *arr, int size)
{
    int sum=0;
    for(int i =0; i< size; i++){
        sum += arr[i];

    }
    unsigned char crc = 256 - (sum & 0x000000ff);
    return crc;
}
