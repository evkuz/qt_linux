#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QFile>
#include <QList>
#include "hiwonder.h"  // hiwonder class
#include "mcinfer.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort serial;
    QByteArray *buff;

    QString     target_name;
    //char       *servos;    //unsigned char
    QByteArray LineEdits[6];

    HiWonder *Robot;

    char Servos [6] = {93,93,93,93,93,93};

    void update_data_from_sliders(int index, int value);

    void GUI_Write_To_Log (int value, QString log_message); //Пишет в лог-файл номер ошибки value и сообщение message

private slots:
    void on_openButton_clicked();

    void on_sitButton_clicked();

    void on_stand_upButton_clicked();

    void on_closeButton_clicked();

    void on_servo_1_lineEdit_editingFinished();

    void on_servo_2_lineEdit_editingFinished();

    void on_servo_3_lineEdit_editingFinished();

    void on_servo_4_lineEdit_editingFinished();

    void on_servo_5_lineEdit_editingFinished();

    void on_servo_6_lineEdit_editingFinished();

    void on_set_posButton_clicked();
    void on_S1_verSlider_valueChanged(int value);

    void on_S2_verSlider_valueChanged(int value);

    void on_S3_verSlider_valueChanged(int value);

    void on_S4_verSlider_valueChanged(int value);

    void on_S5_verSlider_valueChanged(int value);

    void on_S6_verSlider_valueChanged(int value);

signals:
    void Open_Port_Signal(QString portname); // Сигнал даем по нажатию кнопки "OPEN"

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
