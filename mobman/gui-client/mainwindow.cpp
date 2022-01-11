#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}
//+++++++++++++++++++++++++++++++++++++++
// After socket becoming in state "connected", the connected() signal
// is emitted. Here is the slot for that signal.
void MainWindow::onSocketConnected_Slot()
{
    // Раз сокет готов отправляем ему запрос, предварительно подготовив.
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

     GUI_Write_To_Log(0xfefe, "The following Data is going to be sent to CV :");
     GUI_Write_To_Log(0xfefe, request.toUtf8());
     socketCV->write(request.toUtf8());



}

//++++++++++++++++++++++++++++++++++
// - create qtcpsocket with signal/slot configuration
// - connect to host with CV
void MainWindow::on_GetDistanceButton_clicked()
{
    socketCV = new QTcpSocket(this);
    socketCV->setSocketOption(QAbstractSocket::KeepAliveOption, true);

    //Соединение сигналов со слотами
    connect(socketCV, &QIODevice::readyRead, this, &MainWindow::onSocketReadyRead_Slot);//, Qt::QueuedConnection);
    connect(socketCV, SIGNAL(disconnected()), this, SLOT(CV_onDisconnected()),Qt::AutoConnection);

    connect (this->socketCV, &QTcpSocket::connected, this, &MainWindow::onSocketConnected_Slot);
    socketCV->connectToHost(CVDev_IP, CVDev_Port);
    // После успешного socketCV->connectToHost() будет сигнал connected

}
