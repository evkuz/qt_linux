#include "cvdevice.h"
// Class for "Computer Vision Device" objects
// Создаем объект CVDevice - внешний сервер, т.е. сначала сокет
// А потом подключаемся к серверу CVDevice
//CVDevice::CVDevice(QObject *parent) : QObject(parent)
//  //, CVDtcpSocket(new QTcpSocket(this))
//  , hostaddress(new QString)
//  , portnumber(new uint16_t)
CVDevice::CVDevice(const char* hostaddress, int portnumber)
{
    currentTcpData = "";
    CVDtcpSocket = new QTcpSocket(this);
    connect (CVDtcpSocket, &QIODevice::readyRead, this, &CVDevice::readyRead_Slot);
    connect (this->CVDtcpSocket, &QTcpSocket::connected, this, &CVDevice::socketConnected_Slot);
    CVDtcpSocket->connectToHost(hostaddress, portnumber);

    CVDhostaddress = hostaddress;
    CVDportnumber = portnumber;
}

CVDevice::~CVDevice()
{
    ;
}
//++++++++++++++++++++++++++++++++++++++++
// Посылаем запрос в устройство CV
void CVDevice::askCVDevice()
{
    // А вот теперь готовим команду "/service?name=getposition"
    QString request = "GET ";
    request += "/service?name=getposition";
    request += " HTTP/1.1";
    request += "\r\nHost: ";
    request += "192.168.1.201:5001\r\n";
    request += "Accept: */*\r\n";
   // request += "Access-Control-Allow-Origin: *\r\n";

    request += "content-type: application/json\r\n";
    request += "Access-Control-Allow-Origin: *\r\n";
    request += "\r\n";

   // request += "";

//    GUI_Write_To_Log(0xfefe, "The following Data is going to be sent to CV :");
//    GUI_Write_To_Log(0xfefe, request.toUtf8());
    CVDtcpSocket->write(request.toUtf8());
}
//+++++++++++++++++++++++++++++++++++++++
// Слот обработки сигнала readyRead
void CVDevice::readyRead_Slot()
{
    QString nextTcpData;
    nextTcpData = CVDtcpSocket->readAll();

    //Либо можно сравнивать число байт...
    if (currentTcpData == nextTcpData)
    { // значить все данные получены
      // Пишем их в лог, передаем на обработку.
      emit data_from_CVDevice_Signal(currentTcpData);
    }
    else
    {
        currentTcpData = nextTcpData;
    }
}
//+++++++++++++++++++++++++++++++++++++++
// Слот обработки сигнала QTcpSocket::connected
void CVDevice::socketConnected_Slot()
{
    // Обычно тут пишем что-то в Лог. Т.к. это отдельный класс
    // ТО передаем "наверх"
    QString str;
    str = "Connection to CV device established !";
    str += "Host "; str += this->CVDhostaddress;
    str += " port "; str += QString::number(CVDportnumber);
    emit data_from_CVDevice_Signal(str);

}
