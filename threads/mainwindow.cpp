#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "positions.h"
#include "hiwonder.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <QtGui>
#include <chrono>
#include <thread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , readSocket("../../simpledetector_cpp/iqr.socket")
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QList ports = QSerialPortInfo


    //parcel_size = 8;
    DETECTED = false;
    new_get_request = false;
    thread_counter = 0;
    qspb_list = {ui->servo_1_spinBox, ui->servo_2_spinBox, ui->servo_3_spinBox,
                 ui->servo_4_spinBox, ui->servo_5_spinBox, ui->servo_6_spinBox};

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->comL->addItem(info.portName(),info.portName());
    }

    // Прячем QlineEdit-ы и QSlider-ы
//    for (int i =0; i<= DOF -1; i++)
//    {
//        qle_list[i]->setVisible(false);
//        slider_list[i]->setVisible(false);
//    }


    target_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    //QByteArray ba = target_name.toLocal8Bit();
    //g/const char *c_str = ba.data();
    //printf("Appname : %s", c_str);
    Robot = new HiWonder(); // Без этого будет "The program has unexpectedly finished", хотя в начале нговорила, что это ambiguous

    Robot->Log_File_Open(Log_File_Name);

    QString str = "The application \"";  str +=target_name; str += "\"";
    Robot->Write_To_Log(0xf000, str.append(" is started successfully!!!\n"));

    TheWeb = new WebServer(); //
    GUI_Write_To_Log(0000, "Going to Start QTcpSErver");
    if (server.isListening ()) {str = "Listening on port "; str += QString::number(server.tcpport);
        GUI_Write_To_Log(0000, str);
    }
    //QSimpleServer server;
    //server.startTCP();

//    if (server.listen(QHostAddress::AnyIPv4, 8383))
//       { GUI_Write_To_Log(0000, "Listening...");
//        //server.LISTENING = true;
//       }
//    else {
//          str = "Error while starting: ";
//          str += server.errorString();
//          GUI_Write_To_Log(0000, str);
//    }
//    str = " The value of LISTENING IS ";
//    //str += QVariant(server.LISTENING).toString();
//    GUI_Write_To_Log(0000,str);


    //+++++++++++++++++++++++++++++++++  signal/slot of Get Request to webserver
    //connect( TheWeb, SIGNAL(Data_TO_Log_Signal(QString)), this, SLOT(Data_From_Web_SLot(QString))); // Работает
    //connect(this, SIGNAL(Write_2_Client_Signal()), TheWeb, SLOT(Write_2_Client_Slot()));
    // Fixed By Miksarus
    //connect(this, &MainWindow::Write_2_Client_Signal, &server, &QSimpleServer::Write_2_Client_SLot);

    // Fixed by E.Kuznetsov
    connect(this, &MainWindow::Write_2_Client_Signal, &server, &QSimpleServer::Write_2_Client_SLot);
    connect(&server, SIGNAL(Info_2_Log_Signal(QString)), this, SLOT(Info_2_Log_Slot(QString)));
    //connect(Robot, SIGNAL(StatusChangedSignal(QString)), &server, SLOT(SetCurrentState(QString)));
    //connect(this, SIGNAL(StartTakeAndPutSignal()), this, SLOT(TakeAndPutSlot()));

    //TheWeb->openSocket();
    //server = new QSimpleServer();

    //#########################################

   // connect(&server, &QTcpServer::newConnection, this, &MainWindow::newConnection_Slot);
    //################### SERIAL SIGNAL/SLOTS ############################
    connect( this, SIGNAL (Open_Port_Signal(QString)), Robot, SLOT(Open_Port_Slot(QString)));
    connect( &Robot->serial, SIGNAL (readyRead()), Robot, SLOT(ReadFromSerial_Slot()));  //&QSerialPort::

    RMath = new Robo_Math();
    //################### Robo_Math SIGNAL/SLOTS #########################
    connect( this, SIGNAL (Pass_XY_Signal(int, int)), RMath, SLOT(Pass_XY_Slot(int, int)));
    connect( RMath, SIGNAL (Return_EL_Signal(float)), this, SLOT(Return_EL_Slot(float)));
    connect( this, SIGNAL (FW_Kinemaic_Signal(int, int, int, int, int, int)), RMath, SLOT(FW_Kinemaic_Slot(int, int, int, int, int, int)));
    connect( RMath, SIGNAL(Return_FW_Kinematic_XYZ_Signal(int, int, int, float)), this, SLOT(Return_FW_Kinematic_XYZ_Slot(int, int, int, float)));

    connect(RMath, SIGNAL (Pass_String_Signal(QString)), this, SLOT(Pass_String_Slot(QString)));
    //#################### Signal to web-server
    connect( Robot, SIGNAL (Moving_Done_Signal()), this, SLOT (Moving_Done_Slot()));;;


    // ============================================== Создаем поток 1 - web-server
//        thread_A = new QThread;
//        chan_A = new MyThread();

//        connect(thread_A, SIGNAL(started()), chan_A, SLOT(A_SLOT()) );
//        connect(chan_A, SIGNAL(finished()), thread_A, SLOT(quit()));
//        connect(chan_A, SIGNAL(Process_A()), TheWeb, SLOT(Output_Data_From_Client_Slot()), Qt::QueuedConnection); //Считываем из железки в ПК по каналу А
                                                                       // Qt::DirectConnection
                                                                       // Qt::QueuedConnection
                                                                       // Qt::BlockingQueuedConnection
                                                                       // Qt::DirectConnection
/*
 * "2) DirectConnection - The slot is invoked immediately, when the signal is emitted. The slot is executed in the emitter's thread,
 *     which is not necessarily the receiver's thread.
 *
    3) QueuedConnection - The slot is invoked when control returns to the event loop of the receiver's thread.
       The slot is executed in the receiver's thread.

    4) BlockingQueuedConnection - The slot is invoked as for the Queued Connection,
       except the current thread blocks until the slot returns. Note: Using this type to connect objects in the same thread will cause deadlock."
*/
//        chan_A->moveToThread(thread_A);
//        chan_A->finthread = false;
//        chan_A->pause_thread = false;




    //+++++++++++++++ ОТкрываем порт Open_Port_Signal(QString portname); ttyUSB0
    //emit Open_Port_Signal("ttyUSB0");




}
//++++++++++++++++++++++++++++++++++++++++++++++
MainWindow::~MainWindow()
{
    GUI_Write_To_Log(0xffff, "Program is going to be closed");
    delete ui;
    delete Robot;
    delete TheWeb;

}
//+++++++++++++++++++++++++++++++++++++++
void MainWindow::GUI_Write_To_Log (int value, QString log_message)
{
    QDateTime curdate ;
    QTextStream uin(&Robot->LogFile);

    QString str, str2;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_message << "\n";


}


//+++++++++++++++++++++++++++++++++++++++
void MainWindow::on_openButton_clicked()
{
QString portname = ui->comL->currentText();
emit Open_Port_Signal(portname);
//    serial.setPortName(ui->comL->currentText());
//    serial.open(QIODevice::ReadWrite);
//    serial.setBaudRate(QSerialPort::Baud115200);
//    serial.setDataBits(QSerialPort::Data8);
//    serial.setParity(QSerialPort::NoParity);
//    serial.setStopBits(QSerialPort::OneStop);
//    serial.setFlowControl(QSerialPort::NoFlowControl);



   /*
    if (!serial.open(QIODevice::ReadWrite)) {
        processError(tr("Can't open %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setBaudRate(QSerialPort::Baud19200)) {
        processError(tr("Can't set rate 9600 baud to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setDataBits(QSerialPort::Data8)) {
        processError(tr("Can't set 8 data bits to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setParity(QSerialPort::NoParity)) {
        processError(tr("Can't set no patity to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setStopBits(QSerialPort::OneStop)) {
        processError(tr("Can't set 1 stop bit to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setFlowControl(QSerialPort::NoFlowControl)) {
        processError(tr("Can't set no flow control to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }
    */
}

void MainWindow::on_sitButton_clicked()
{
    this->update_LineDits_from_position(sit_down_position);
    this->repaint();

    QByteArray dd = QByteArray::fromRawData(sit_down_position, 6);
    dd.append(0x31); // Движение "Туда"
    Robot->GoToPosition(dd);//, sit_down_position
   // update_LineDits_from_servos();
//    for (int i = 0; i<= 57; i++)
//    {
//        dd.append("A");
//    }
//    serial.write(dd);


    //serial.flush();
    //serial.waitForBytesWritten(50);
}
//+++++++++++++++++++++++++++++++

void MainWindow::on_stand_upButton_clicked()
{
    this->update_LineDits_from_position(hwr_Start_position);
    this->repaint();
    QByteArray dd = QByteArray::fromRawData(hwr_Start_position, 6);
    dd.append(0x30); // Движение "Обратно"
    dd.append(LASTONE);
    Robot->GoToPosition(dd);//, hwr_Start_position

//    GUI_Write_To_Log(0xff10, str);
}
//+++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::update_data_from_sliders(int index, int value)
{
    Servos[index]   = value;
    on_set_posButton_clicked();
//    const char *   pchar;
//    pchar = static_cast<const char *>(static_cast<char *>(Servos));
//    QByteArray dd = QByteArray::fromRawData(pchar, 6);
//    QString str = "Servos before write to serial ";
//    for (int i=0; i<= DOF - 1; i++){
//        str += QString::number(Servos[i]);
//        str+= ", ";
//    }
//    GUI_Write_To_Log(0xf002,str);
//    Robot->GoToPosition(dd, pchar);


}
//++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_closeButton_clicked()
{
    Robot->serial.close();
}
//+++++++++++++++++++++++++ update servos from LineEdits

//+++++++++++++++++++++++++++++++++++++++++
//Send data from GUI to robot
void MainWindow::on_set_posButton_clicked()
{
    //QString str;


    // const char *   pchar;
    //pchar = static_cast<const char *>(static_cast<char *>(Servos));
   // QByteArray dd = QByteArray::fromRawData(pchar, 6);
    //GUI_Write_To_Log(0xf003,str);
    QByteArray dd ;
    dd.resize(parcel_size);
    memcpy(dd.data(), Servos, 6);
    dd.insert(parcel_size-2, 0x31); // Движение "Туда"
    dd.insert(parcel_size-1, LASTONE);
    //dd.append(0x31);
    //dd.resize(64);
    //QByteArray dd = QByteArray::fromRawData(Servos, 6);
    Robot->GoToPosition(dd);//, pchar
    //########### данные пользователя
//    str = "Training data : ";
//    str += QString::number(X);str+= ", ";
//    str += QString::number(Y);str+= ", ";
// unsigned char* sData = reinterpret_cast<unsigned char*>(Servos);
//sData[szData-1] = dd.at(szData-1);
//for (int i=0; i<= szData - 1; i++){
//    str += QString::number(sData[i]);
//    str+= ", ";
//}
////########## данные серво

 //serial.write(reinterpret_cast<const char *>(Servos));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_socketButton_clicked()
{
    DetectorState state;
    QString str;
    str = "";
//Сразу открываем захват
//    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
//    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
    Servos[0]=0;
    update_LineDits_from_servos();

    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){
            try_mcinfer(state.objectX, state.objectY);
            X = state.objectX;
            Y = state.objectY;
            str+="DETECTED: ";
            str += QString::number(state.objectX);
            str += ", ";
            str += QString::number(state.objectY);


        } else {
            str += "NOT DETECTED";
        }

       std::cout <<  str.toStdString() << std::endl;
       Robot->Write_To_Log(0xf014, str);
       GUI_Write_To_Log(0xf014, str);
    }
}
//+++++++++++++++++++++++++++++++++++++++  ->text().toInt()
void MainWindow::on_clampButton_clicked()
{
    if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
    else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
    update_LineDits_from_servos();

    on_set_posButton_clicked();
}
//++++++++++++++++++++++++++++++++++++++
void MainWindow::update_LineDits_from_servos(void)
{
    for (int i =0; i<= DOF -1; i++)
    {
       // qle_list[i]->setText(QString::number(Servos[i]));
        qspb_list[i]->setValue((Servos[i]));
       // GUI_Write_To_Log(0xf010, QString::number (Servos[i]));
    }
}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::update_LineDits_from_position(const char *pos)
{
    for (int i =0; i<= DOF -1; i++)
    {
        //qle_list[i]->setText(QString::number(pos[i]));
        qspb_list[i]->setValue(pos[i]);
    }

}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::update_LineDits_from_position(unsigned char *pos)
{
    for (int i =0; i<= DOF -1; i++)
    {
        //qle_list[i]->setText(QString::number(pos[i]));
        qspb_list[i]->setValue(pos[i]);
    }

}
//+++++++++++++++++++++++++++++++++++++
void MainWindow::update_Servos_from_LineEdits(void)
{
    for (int i =0; i<= DOF -1; i++)
    {
       // Servos[i] = qle_list[i]->text().toInt();
        Servos[i] = qspb_list[i]->value();
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainWindow::on_servo_1_spinBox_valueChanged(int arg1)
{
    Servos[0] = arg1;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainWindow::on_servo_2_spinBox_valueChanged(int arg1)
{
    Servos[1] = arg1;
}

void MainWindow::on_servo_3_spinBox_valueChanged(int arg1)
{
    Servos[2] = arg1;
}

void MainWindow::on_servo_4_spinBox_valueChanged(int arg1)
{
    Servos[3] = arg1;
}

void MainWindow::on_servo_5_spinBox_valueChanged(int arg1)
{
    Servos[4] = arg1;
}

void MainWindow::on_servo_6_spinBox_valueChanged(int arg1)
{
    Servos[5] = arg1;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Делаем запрос в simpledetecor, XY, вычисляем длину в мм L от основания робота до XY
void MainWindow::on_getXYButton_clicked()
{
    QString str;
//    DetectorState state;
//    if (readSocket.GetState(&state) == 0)
//      {
//        if (state.isDetected){
//            X = state.objectX;
//            Y = state.objectY;
//            str+="DETECTED: ";
//            str += QString::number(state.objectX);
//            str += ", ";
//            str += QString::number(state.objectY);


//        } else {
//            str += "NOT DETECTED";
//        }
//      }//if (readSocket.GetState(&state) == 0)
      GUI_Write_To_Log(0xf233, "Have got X,Y ");
      X = 500; Y=500;
      str += QString::number(X); str += ", ";
      str += QString::number(Y);
      GUI_Write_To_Log(0xf233, str);
      emit Pass_XY_Signal(X,Y);

      float dl1 = abs(qSin(115));
      str = "sin(115) = ";
      GUI_Write_To_Log(0xf233, str);
      str += QString::number(dl1);
      ui->All_Servos_lineEdit->setText(str);
      GUI_Write_To_Log(0xf233, "No go to Kinematic !");
      emit FW_Kinemaic_Signal(48, 25, 133, RMath->el1, RMath->el2, RMath->el3 ); //1190, 356

}
//++++++++++++++++++++++void Return_XY_Slot(float EL)
void MainWindow::Return_EL_Slot(float EL)
{
    QString str = "Distance value : ";
    str += QString::number(EL);
    GUI_Write_To_Log(0xf112, str);
}
//++++++++++++++++++++++++++++++++++
void MainWindow::Return_FW_Kinematic_XYZ_Slot(int X, int Y, int Z, float EL)
{                //But should be as follows :
    QString str = "Cube coordinates X, Y, L : ";
    str += QString::number(X); str += ", ";
    str += QString::number(Y); str += ", ";
    //str += QString::number(Z); str += ", ";
    str += QString::number(EL);
    GUI_Write_To_Log(0xf1122, str);
    str = "But should be as follows : ";
    str += "1190, 356, 230";// str += ", ";
    GUI_Write_To_Log(0xf1122, str);
}
//+++++++++++++++++++++++++++++++++++++++
void MainWindow::Pass_String_Slot(QString str)
{
    GUI_Write_To_Log(0xf114, str);
}
//+++++++++++++++++++++++++++++++++
//background-color: rgb(26, 148, 255);

void MainWindow::on_submitButton_clicked()
{
    int num;
    QString str;
    QString data = ui->All_Servos_lineEdit->text();
    QStringList list;

    str="The data from line edit are : ";
    str.append(data);
    GUI_Write_To_Log(0xf010, data);

    QRegExp rx("[, ]");// match a comma or a space
  //  QStringList list = {"100", "100", "100", "100", "100", "100" };
    list = data.split(rx, Qt::SkipEmptyParts);
    num = list.size (); //Число элементов, начиная с 1
    if (num < DOF) {
        str = "Данных НЕДОСТАТОЧНО !!! Передано всего "; str.append (QString::number(list.size()));
        //Если данных недостаточно, то меняем цвет кнопки и строки ввода, для подсказки
        GUI_Write_To_Log(0xf010, str);
        str = "background-color: rgb(26, 148, 255)";
        ui->submitButton->setStyleSheet (str);

        str = "background-color: rgb(255, 235, 11)";
        ui->All_Servos_lineEdit->setStyleSheet (str);

    }
    if (num == DOF)
      {
        // А теперь все это в qle_list
        for (int i =0; i<= DOF -1; i++)
        {
            //qle_list[i]->setText(QString::number(Servos[i]));
            qspb_list[i]->setValue(list.at(i).toInt());

        }
        // Если данные введены верно, то возвращаем цвета кнопки и строки ввода к значениям по умолчанию.
        ui->submitButton->setStyleSheet ("");
        ui->All_Servos_lineEdit->setStyleSheet ("");
        GUI_Write_To_Log(0xf010, "Данные переданы корректно");
     }//(num == DOF -1)

//    GUI_Write_To_Log(0xf010, str); //QString::number(list.size())
//    for (int i=0; i< list.size(); ++i ) {
//        GUI_Write_To_Log(0xf010, list.at(i).toLocal8Bit().constData());
//    }


this->repaint();
}
//++++++++++++++++++++++++++
void MainWindow::on_trainButton_clicked()
{
//    for (int i =0; i<= DOF -1; i++)
//    {
//       Servos[i] = train_position[i];
//    }
//    update_LineDits_from_servos ();


    DetectorState state;
    QString str;
    str = "";
//Сразу открываем захват
//    if (ui->servo_1_lineEdit->text().toInt() > 0){ ui->servo_1_lineEdit->setText("0"); Servos[0]=0;}
//    else {ui->servo_1_lineEdit->setText("160"); Servos[0]=160;}
    Servos[0]=0;
    update_LineDits_from_servos();

    if (readSocket.GetState(&state) == 0)
      {
        if (state.isDetected){
            try_mcinfer(state.objectX, state.objectY);
            X = state.objectX;
            Y = state.objectY;
            str+="DETECTED: ";
            str += QString::number(state.objectX);
            str += ", ";
            str += QString::number(state.objectY);
            DETECTED = true;

        } else {
            str += "NOT DETECTED";
        }

       std::cout <<  str.toStdString() << std::endl;
       Robot->Write_To_Log(0xf014, str);
       GUI_Write_To_Log(0xf014, str);
    }

//В этой точке робот опустил захват, открыл захват.

if (DETECTED)
    {

    QByteArray dd ;
    dd.resize(parcel_size);
//    memcpy(dd.data(), Servos, 6);
//    dd.insert(6, 0x31); // Движение "Туда"
//    Robot->GoToPosition(dd);
////    while (!Robot->MOVEMENT_DONE) {;}
   str = "Next movement to robot";
   this->GUI_Write_To_Log (0xF055, str);
   //+++++++++++++++++ make clamp
   //on_clampButton_clicked();
   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
   this->send_Data(NOT_LAST);
   //++++++++++++++++++++ make stand up
   //on_stand_upButton_clicked();
   this->update_LineDits_from_position(hwr_Start_position);
   this->repaint();
   this->update_Servos_from_LineEdits();
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++ put the cube
   this->update_LineDits_from_position (put_position);
   this->repaint();
   update_Servos_from_LineEdits ();
   memcpy(dd.data(), Servos, 6);
   dd.insert(6, 0x31); // Движение "Туда"
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++ Unclamp the gripper
   //on_clampButton_clicked();
   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++++++++++++++
   this->update_LineDits_from_position (after_put_position);
   this->repaint();
   update_Servos_from_LineEdits ();
   memcpy(dd.data(), Servos, 6);
   dd.insert(6, 0x31); // Движение "Туда"
   this->send_Data(NOT_LAST);

   //+++++++++++++++++++++ go back to start position
   //on_stand_upButton_clicked();
   this->update_LineDits_from_position(hwr_Start_position);
   this->repaint();
   this->update_Servos_from_LineEdits();
   this->send_Data(LASTONE); // The last command

  }// if (DETECTED)

   DETECTED = false;


}
//++++++++++++++++++++++++++++++++++++
void MainWindow::send_Data(unsigned char thelast)
{
   // QString str;


    QByteArray dd ;
    dd.resize(parcel_size);
    memcpy(dd.data(), Servos, 6);
    dd.insert(parcel_size-2, 0x31); // Движение "Туда"
    dd.insert(parcel_size-1, thelast);
    //dd.append(0x31);
    //dd.resize(64);
    //QByteArray dd = QByteArray::fromRawData(Servos, 6);
    Robot->GoToPosition(dd);
}
//++++++++++++++++++++++++++
// Пришел запрос от вебсервера. Весь запрос в message
void MainWindow::Data_From_Web_SLot(QString message)
{
 GUI_Write_To_Log(0xf00E, message);
 new_get_request = true;
 thread_counter +=1;
 this->ui->threadlabel->setText(QString::number(thread_counter));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++ Получили запрос от клиента. Парсим его.
void MainWindow::Info_2_Log_Slot(QString message)
{
    QString str, substr;
    int value = 0xf00f;
    new_get_request = true;
    str = "Get new command : "; str += message;
    GUI_Write_To_Log(0xf00f, str);

//    int sPosition, ePosition; // Индекс строки run в запросе.
//    sPosition = message.indexOf("/run?cmd=");

//   QString  wrong_mess = "/favicon.ico HTTP/1.1";

//    if (!message.contains (wrong_mess))
//    {
//        sPosition += 9;
//        ePosition = message.indexOf("&", sPosition);
//        substr = message.mid(sPosition, (ePosition - sPosition));


//        str = "Получена команда : "; str += substr;
//        GUI_Write_To_Log(0xf00f, str);

        // changed by Miksarus
//        if (substr == "start") {
//            //on_clampButton_clicked ();
//            //Robot->SetCurrentStatus ("init");
//            //emit StartTakeAndPutSignal();
//            emit on_trainButton_clicked ();
//         }

//        if (substr == "reset") {
//            if (Robot->GetCurrentStatus () == "done"){
//                Robot->SetCurrentStatus ("wait");
//            }
//         }
//         ///run?cmd=status&123
//        if (substr == "status") {
//            //str = Robot->GetCurrentStatus ();
//            str = Robot->current_status;
//            str = "superpuper";
//            emit Write_2_Client_Signal (str);
//        }
//     }
   substr = message;
   if (substr == "status") {
       //str = Robot->GetCurrentStatus ();
       str = Robot->current_status;
       //str = "status_from_robot";
       emit Write_2_Client_Signal (str);
   }

}


// added by Miksarus
void MainWindow::TakeAndPutSlot()
{
    DetectorState state;
    QString str;
    str = "";

    Servos[0]=0;
    update_LineDits_from_servos();

    bool isDetected = false;

    while (!isDetected) {
        if (readSocket.GetState(&state) == 0)
        {
            isDetected = state.isDetected;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    try_mcinfer(state.objectX, state.objectY);

    str+="DETECTED: ";
    str += QString::number(state.objectX);
    str += ", ";
    str += QString::number(state.objectY);

    std::cout <<  str.toStdString() << std::endl;
    Robot->Write_To_Log(0xf014, str);
    GUI_Write_To_Log(0xf014, str);

    //В этой точке робот опустил захват, открыл захват.

    QByteArray dd ;
    dd.resize(parcel_size);
    str = "Next movement to robot";
    this->GUI_Write_To_Log (0xF055, str);
   //+++++++++++++++++ make clamp
   //on_clampButton_clicked();
   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
   this->send_Data(NOT_LAST);
   //++++++++++++++++++++ make stand up
   //on_stand_upButton_clicked();
   this->update_LineDits_from_position(hwr_Start_position);
   this->repaint();
   this->update_Servos_from_LineEdits();
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++ put the cube
   this->update_LineDits_from_position (put_position);
   this->repaint();
   update_Servos_from_LineEdits ();
   memcpy(dd.data(), Servos, 6);
   dd.insert(6, 0x31); // Движение "Туда"
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++ Unclamp the gripper
   //on_clampButton_clicked();
   if (ui->servo_1_spinBox->value () > 0){ ui->servo_1_spinBox->setValue (0); Servos[0]=0;}
   else {ui->servo_1_spinBox->setValue (90); Servos[0]=90;}
   this->send_Data(NOT_LAST);
   //+++++++++++++++++++++++++++++++++
   this->update_LineDits_from_position (after_put_position);
   this->repaint();
   //update_Servos_from_LineEdits ();
   //memcpy(dd.data(), Servos, 6);
   //dd.insert(6, 0x31); // Движение "Туда"
   //this->send_Data(NOT_LAST);

   //+++++++++++++++++++++ go back to start position
   //on_stand_upButton_clicked();
   this->update_LineDits_from_position(hwr_Start_position);
   this->repaint();
   this->update_Servos_from_LineEdits();
   this->send_Data(LASTONE); // The last command
}

void MainWindow::newConnection_Slot()
{
    GUI_Write_To_Log(0xf01f, "There is new TCP connection !!!");
}
//++++++++++++++++++++++++++
// Пишем в лог сообщение, что комплексная команда (например,взять кубик в одной точке и положить в другую)
// А также это индикатор, что команда выполнена и можно, например, отправить эти данные вебсерверу.
void MainWindow::Moving_Done_Slot()
{
    GUI_Write_To_Log(0xFAAA, "Demo cycle finished !!!");
    strcpy(TheWeb->status_buffer,"done");
    // Меняем статус, теперь "done"
    std::cout<<"Set DONE to Robot!" << std::endl;
    Robot->SetCurrentStatus ("done");
    if (new_get_request) // Тогда даем сигнал серверу на отправку данных клиенту. Данные уже в буфере TheWeb->status_buffer
    {
     // emit Write_2_Client_Signal(Robot->current_status);
      new_get_request = false;
    }

}

void MainWindow::on_start_tcpButton_clicked()
{
    GUI_Write_To_Log(0xFAAA, "Thread Is going to be Start Here !!!");
    thread_A->start();
    GUI_Write_To_Log(0xFAAA, "Thread Started Here !!!");
}
//+++++++++++++++++
void MainWindow::server_New_Connect_Slot()
{
    ;
}
