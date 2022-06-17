#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "positions.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int value = 0x0000;
    ui->setupUi(this);

    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    Dobot = new Robot();
    Dobot->Log_File_Open(Log_File_Name);
    QString str = "The application \"";  str +=target_name; str += "\"";
    Dobot->Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));

//    mypose.x = 123.6789;
//    mypose.y = 0.0;
//    mypose.z = 0.0;
//    mypose.r = 0.0;

    //mypose.jointAngle = new float[4];

    connect(Dobot, &Robot::GetData_Signal, this, &MainWindow::GetData_Slot);

    protocolDobot = new dobotCommands();

    connect(protocolDobot, &dobotCommands::getCooridnates_Signal, this, &MainWindow::getCooridnates_Slot);

    //Инициализируем параметры конкретного робота. Основной класс не трогаем.

    Dobot->robot_buffer_SIZE = 75;
//    Dobot->DOF = 4;
//    Dobot->szData = 7;

    //++++++++++++++++ Открываем порт
    int OKay = Dobot->Open_Port_Slot("ttyUSB0");
    if (!OKay) { //!Robot->SerialIsOpened
        //Robot->current_st_index = 4;

       OKay = Dobot->Open_Port_Slot("ttyUSB1");

    } // Robot->current_status = statuslst.at(4)

    if (!OKay){
                Dobot->Write_To_Log(value, "SerialPort  PROBLEM !!!");
                // ТОгда таймер пускаем ???
    };
    //+++++++++++++++++++++++++++++

    Dobot->Write_To_Log(value, "Waiting for command");
//    int dbtID=234;

    QString myList = "ttyUSB0 ttyUSB1 ttyUSB2";
//    std:string cstr = myList.toStdString();

//    char *dobotList = "ttyUSB0 ttyUSB1 ttyUSB2";//

    // Вот так получаем Dobot id
//    dbtID = SearchDobot(myList.toLatin1().data(), 80);

//    int result = ConnectDobot("ttyUSB1", 115200, "Dobot", "V1.2", &dbtID);
//    str = "Now the Dobot ID value is ";
//    str += QString::number(dbtID);
//    Dobot->Write_To_Log(value, str);

//    int serNum;
//    char dSN[75] = {};
//    char *deviceSN = dSN;
//    serNum = GetDeviceSN(deviceSN, 255);
//    str = "Dobot device Serial Number is ";
//    str += QString::number(serNum);
//    Dobot->Write_To_Log(value, str);

    int RC;
    //QString devName  = "IQR LIT JINR Dobot Magician";
//    const char *dvname = "IQR LIT JINR Dobot Magician";
    //const char *dvname = devName.toLatin1().constData();

//QByteArray qbData(0xaa, 0xaa);

//    qbData.append(0x1D);        // Добавили длину 2+27 ==29
//    qbData.append(0x01);        // Код команды
//    qbData.append(0x01);        // rw
//    qbData.append((char)0x00);  // isQueued
//    qbData.append(dvName);      // char[n] DeviceName
//    qbData.append(0xe1);        // Payload checksum


//    RC = SetDeviceName(dbtID, dvName); //dvname
//    if (RC == DobotCommunicate_NoError){
//        Dobot->Write_To_Log(value, "Device name has setted SUCCESSFULY !");
//    }
//    else {
//        Dobot->Write_To_Log(value, "There is a error with setting Device name !");
//    }

//    char p[] = {'1', '2', '3', '1', '2', '3', '8', '\0'}; // Создаем пустой массив
//    char *deviceName;
//    deviceName = p;
    // Если (deviceName == NULL то будет ошибка, поэтому инициализируем.
//    RC = GetDeviceName(deviceName, 8);
//    if (RC == DobotCommunicate_NoError){
//        Dobot->Write_To_Log(value, "Device name Getting passed SUCCESSFULY !");
//    }
//    else if (RC == DobotCommunicate_InvalidParams) {
//        Dobot->Write_To_Log(value, "There are invalid params with GETting Device name !");
//    }
//    else if (RC == DobotCommunicate_Timeout) {
//        Dobot->Write_To_Log(value, "There is a error with GETting Device name ! TimeOUT");

//    }
//    else if (RC == DobotCommunicate_BufferFull) {
//        Dobot->Write_To_Log(value, "There is Buffer FULL with GETting Device name !");
//    }


//    str = "Dobot device Name is as follows : ";
//    str += QString(*deviceName).toUtf8();
//    Dobot->Write_To_Log(value, str);


    QByteArray xyData;
    xyData.append(0x43);
    xyData.append(0x16);
    xyData.append(0x35);
    xyData.append(0x83); // 0x83



    quint32 temp, mytemp;
    //static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

    temp = 0;

    temp = ((char)xyData[0] << 24) + ((char)xyData[1] << 16) + ((char)xyData[2] << 8) + (char)xyData[3]; // Big endian
////    temp = temp & 0xff000000;
//    mytemp = temp;
//    temp = ((char)xyData[1] << 16);
////    temp = temp & 0x00ff0000;
//    mytemp += temp;
//    temp = ((char)xyData[2] << 8);
////    temp = temp & 0x0000ff00;
//    mytemp += temp;
//    //temp = 0;
//    temp = (char)xyData[3];
////    temp = temp & 0x000000ff;
//    mytemp += temp;

    //temp = 0x43163583;
    float* out = reinterpret_cast<float*>(&temp);
    float* mydata = reinterpret_cast<float*>(&xyData);


    str = "Hex value of temp is 0x";
    str += QString::number(temp, 16);

    int abc = 25;
    int *dtemp = &abc;
    memcpy(dtemp, xyData, 4);

     Dobot->Write_To_Log(value, str);
   qDebug() <<  "Hex value of temp is 0x" << QString::number(mytemp, 16) << " " << QString::number(*out) << " " << QString::number(*mydata);
   qDebug() << "QByteArray Data are : " << xyData;

//   QByteArray xyzData;
//   //xyData.resize(4);
//   xyzData.append(0x43);
//   xyzData.append(0x16);
//   xyzData.append(0x35);
//   xyzData.append(0x83); // 0x83


//   char pdata[4] = {'\x43', '\x16', '\x35', '\x83'};
//   xyzData.append(pdata);
//   quint32 *dddata;

//   dddata = reinterpret_cast<quint32*>(&xyzData);

//   memcpy(dddata, xyzData.constData(), 4);
//   quint32 *pdata = reinterpret_cast<quint32*>(xyzData);
// //  memcpy(mydata, pdata, 4);
 //  mydata = reinterpret_cast<float*>(dddata);
//   //float fdata = *mydata;
//   qDebug() << "Float from char data are : " << QString::number(*mydata);

//   qDebug() << "Int from QByteArray is : " << QString::number(*dddata, 16);

   waitingCommandCode = 0x00;
}

MainWindow::~MainWindow()
{
    Dobot->Write_To_Log(0x0000, " The program is going to be closed\n");
    Dobot->deleteLater();

    delete ui;
}
//++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::GetData_Slot(QByteArray dbuf)
{
    // Делаем обработку данных, полученных по Serial Port от робота

    // Запускаем парсинг, по результатам которого будет сигнал dobotCommands::getCooridnates_Signal
    protocolDobot->parseData(dbuf);

}
//++++++++++++++++++++++++++++++++++++++++++++++++
// Слот сигнала dobotCommands::getCooridnates_Signal при получении ответа на GetPose
// Обновились данные координат xyzr - можно их считать из public-переменных.
//

void MainWindow::getCooridnates_Slot()
{

    int value = 0x1331;
    str = "From Parser XX = ";
    str += QString::number(protocolDobot->X);
//    str += "      ";
//    str += "From Parser X = ";
//    str += QString::number(*protocolDobot->pX);

    Dobot->Write_To_Log(value, str);

    this->ui->FloatXYZRlineEdit->setText(QString::number(protocolDobot->X)); //QString::number(protocolDobot->X)  "Float data expected"

    this->ui->FloatXYZRlineEdit_Y->setText(QString::number(protocolDobot->Y));
    this->ui->FloatXYZRlineEdit_Z->setText(QString::number(protocolDobot->Z));
    this->ui->FloatXYZRlineEdit_R->setText(QString::number(protocolDobot->R));

    if (waitingCommandCode == 0x2288) { // значит была нажата кнопка "XAdd10Button"
        // Вот тут теперь делаем +10 и отправляем новую команду
        protocolDobot->X += 10.0;
        protocolDobot->Y += 10.0;
        //protocolDobot->Z += 10.0;
//        protocolDobot->R = 0.0;
        this->ui->FloatXYZRlineEdit->setText(QString::number(protocolDobot->X));
        this->ui->FloatXYZRlineEdit->setText(QString::number(protocolDobot->Y));
        this->ui->FloatXYZRlineEdit->setText(QString::number(protocolDobot->Z));
        this->ui->FloatXYZRlineEdit->setText(QString::number(protocolDobot->R));

        // Теперь обратно их в массив на отправку отправляем роботу
        QByteArray dd ;
        dd.resize(6);
        memcpy(dd.data(), setPTPHeaderINC, 6); // HEader of SetPTP Command WITH INCREMENT mode
        // Data converted and appended to QByteArray dd
        protocolDobot->floatToQByteArray(&dd, &protocolDobot->X);
        protocolDobot->floatToQByteArray(&dd, &protocolDobot->Y);
        protocolDobot->floatToQByteArray(&dd, &protocolDobot->Z);
        protocolDobot->floatToQByteArray(&dd, &protocolDobot->R);
        QByteArray ddTemp = dd.mid(3); // from 4th position till the end
        quint8 crcByte = protocolDobot->getCRC(reinterpret_cast<unsigned char*>(ddTemp.data()), ddTemp.size());
        dd.append(crcByte);
        Dobot->SendData(dd);

        Dobot->Write_To_Log(value, "<X + 10> Command has been sent");
        waitingCommandCode = 0;
     } // if (waitingCommandCode == 0x2288)



//        quint32 temp;
//        temp = *reinterpret_cast<int*>(&protocolDobot->X);
//        // Теперь надо обратно в LittleEndian
//        dd.append((char)temp >> 24);
//        dd.append((char)temp >> 16);
//        dd.append((char)temp >> 16);
//        dd.append((char)temp >> 8);
    //}

}

//++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_homePushButton_clicked()
{
    int value = 0x2222;
    QString str;
    QByteArray dd ;

    int sz = sizeof (home_position);
    dd.resize(sz);
    memcpy(dd.data(), home_position, sz);
    Dobot->GoToPosition(dd);
    Dobot->Write_To_Log(value, "<Home> command has been sent");
}
//++++++++++++++++++++++++++++++++++++++++++++++++

//"IQR LIT JINR Dobot Magician";
void MainWindow::on_SetNameButton_clicked()
{
    int value = 0x2233;
//    QByteArray dd ;

//    int sz = sizeof (qbData);
//    dd.resize(sz);
//    memcpy(dd.data(), home_position, sz);
    QByteArray qbData;
    qbData.append(0xaa);
    qbData.append(0xaa);
    qbData.append(0x09);        // Добавили длину 2+7 ==9
    qbData.append(0x01);        // Код команды
    qbData.append(0x01);        // rw + isQueued
    qbData.append(dvName);      // char[n] DeviceName
    qbData.append(0xDF);        // Payload checksum = 256 - 238 = 18


    Dobot->GoToPosition(qbData);
    Dobot->Write_To_Log(value, "<Set Device Name> command has been sent");


}

//++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_GetPoseButton_clicked()
{
    int value = 0x2244;
    QString str;
    QByteArray dd ;

//    int sz = sizeof (ask_pos);
//    dd.resize(sz);
//    memcpy(dd.data(), ask_pos, sz);

//    int RC = GetPose(&mypose);
//    if (RC == DobotCommunicate_NoError){
//        Dobot->Write_To_Log(value, "Device POSE Getting passed SUCCESSFULY !");
//    }
//    else {
//        Dobot->Write_To_Log(value, "There are some errors with GETting Device POSE !");
//    }

    Dobot->Write_To_Log(value, "<GetPose> command has been sent");

    // Извлекаем данные из структуры mypose

   str = "The x = ";
   str += QString::number(999.456);//(mypose.x);
   Dobot->Write_To_Log(value,str);
}

//++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_ReadDevNameButton_clicked()
{
    int value = 0x2255;

    QByteArray qbData;
    QByteArray rdBuf;
    rdBuf.resize(160);
    rdBuf.fill((char)0x00);
    qbData.append(0xaa);
    qbData.append(0xaa);
    qbData.append(0xA2);        // Добавили длину 2+160
    qbData.append(0x01);        // ID - Код команды
    qbData.append((char)0x00);  // rw + isQueued
    qbData.append(rdBuf);       // read buffer 160 bytes of zeroes
    qbData.append(0xff);        // Payload checksum = 256 - 238 = 18


    Dobot->GoToPosition(qbData);
    Dobot->Write_To_Log(value, "<Read Device Name> command has been sent");
/*
 *
 * Далее парсим в соответствии с checksum и можно вывести имя в текстовом виде.
 * НО, пока не актуально
*/
}

//++++++++++++++++++++++++++++++++++++++++++++++++
//
void MainWindow::on_GetPoseButton_2_clicked()
{
    int value = 0x2266;

    QByteArray qbData;
    QByteArray rdBuf;
    rdBuf.resize(160);
    rdBuf.fill((char)0x00);
    qbData.append(0xaa);
    qbData.append(0xaa);
    qbData.append(0x02);        // Добавили длину 2+0
    qbData.append(0x0A);        // ID = 10 - Код команды
    qbData.append((char)0x00);  // rw + isQueued
    //qbData.append(rdBuf);       // read buffer 160 bytes of zeroes
    qbData.append(0xf6);        // Payload checksum = (256 - 10) = 246 = 0xF6.


    Dobot->GoToPosition(qbData);
    Dobot->Write_To_Log(value, "<Get Pose> command has been sent");

}

//++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_SpecPoseButton_clicked()
{
    int value = 0x2277;

    //QString str;
    QByteArray dd ;

    int sz = sizeof (spec_pos);
    dd.resize(sz);
    memcpy(dd.data(), spec_pos, sz);
    Dobot->SendData(dd);
    Dobot->Write_To_Log(value, "<Go to position X: 250, Y: 150, Z: 50, R: 150> command has been sent");


}
//++++++++++++++
// This converter is for BigEndian data
// Так не работает.
float MainWindow::QByteArrayToFloat(QByteArray arr)
{
    static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

    quint32 temp = ((char)arr[0] << 24)|((char)arr[1] << 16)|((char)arr[2] << 8)|(char)arr[3]; // Big endian
    float* out = reinterpret_cast<float*>(&temp);

    return *out;
}

//++++++++++++++++++++++++++++++++++++++++++++++++
// X + 10 к текущей позиции.
void MainWindow::on_XAdd10Button_clicked()
{
    // 1. Считываем позицию через GetPose
    // 2. Раскладываем значение X в QByteArray -> float -> делаем +10 -> QByteArray
    // 3. Задаем новую позицию SetPTPCmd (ID 84)
/*
 * Или... В другом потоке отправляем, ждем когда там придут данные в ответ
 * продолжаем тут.
 *
 *
 */

    int value = 0x2288;

    QByteArray qbData;
//    QByteArray rdBuf;
//    rdBuf.resize(160);
//    rdBuf.fill((char)0x00);
    qbData.append(0xaa);
    qbData.append(0xaa);
    qbData.append(0x02);        // Добавили длину 2+0
    qbData.append(0x0A);        // ID = 10 - Код команды
    qbData.append((char)0x00);  // rw + isQueued
    //qbData.append(rdBuf);       // read buffer 160 bytes of zeroes
    qbData.append(0xf6);        // Payload checksum = (256 - 10) = 246 = 0xF6.

    // mutex.lock();
    waitingCommandCode = 0x2288; // Код ф-ции, которая вызвала.
    // mutex.unlock();

    Dobot->SendData(qbData);
    //QThread::msleep(100);

    // Данные отправили. Теперь по сигналу readyRead надо как-то их считать.
    // Отдельный поток.



}

//++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_GetHOMeParamsButton_clicked()
{
    int value = 0x2299;

    QString str;
    QByteArray dd ;

    int sz = sizeof (getHOMeParams);
    dd.resize(sz);
    memcpy(dd.data(), getHOMeParams, sz);
    Dobot->SendData(dd);
    Dobot->Write_To_Log(value, "<GetHOMEParams> command has been sent");

}

//++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_ToHOMeButton_clicked()
{
    int value = 0x22AA;

    QString str;
    QByteArray dd ;

    int sz = sizeof (toHOMePosition);
    dd.resize(sz);
    memcpy(dd.data(), toHOMePosition, sz);
    // Get data without crc, calculate crc, add crc to data
    dd.append(protocolDobot->getCRC(crcSource, sizeof (crcSource)));

    //Dobot->GoToPosition(dd);
    Dobot->SendData(dd);
    Dobot->Write_To_Log(value, "<Go to position ToHOME> command has been sent");

}

