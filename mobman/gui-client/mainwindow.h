#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
#define CVDev_IP     "192.168.1.201"  //"192.168.1.201"
#define CVDev_Port   5001



    QTcpSocket* socketCV;
public slots:
    void onSocketConnected_Slot();
    void onSocketReadyRead_Slot();

private slots:
    void on_GetDistanceButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
