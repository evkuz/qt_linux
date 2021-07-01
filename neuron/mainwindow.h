#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <stdint.h>
#include <QFile>
#include <QList>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include "hiwonder.h"  // hiwonder class
#include "SocketClient.h"
#include"robo_math.h" //Robo_Math class

//#include "mcinfer.h"




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

    QList<QLineEdit*> qle_list;
    QList<QSpinBox*> qspb_list;
    QList<QSlider*>  slider_list;

    HiWonder *Robot;
    Robo_Math * RMath;

    int X, Y;//Координаты x,y

    unsigned char Servos [6] = {93,93,93,93,93,93};

    void update_data_from_sliders(int index, int value);

    void GUI_Write_To_Log (int value, QString log_message); //Пишет в лог-файл номер ошибки value и сообщение message
    void try_mcinfer(int x, int y);
    void update_LineDits_from_servos(void);
    void update_LineDits_from_position(const char *pos);
    void update_Servos_from_LineEdits(void);

private:
    SocketClient readSocket;

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

    void on_socketButton_clicked();

    void on_clampButton_clicked();



    void on_servo_1_spinBox_valueChanged(int arg1);

    void on_servo_2_spinBox_valueChanged(int arg1);

    void on_servo_3_spinBox_valueChanged(int arg1);

    void on_servo_4_spinBox_valueChanged(int arg1);

    void on_servo_5_spinBox_valueChanged(int arg1);

    void on_servo_6_spinBox_valueChanged(int arg1);

    void on_pushButton_clicked();



    void on_getXYButton_clicked();
    void Return_EL_Slot(float EL);
    void Return_FW_Kinematic_XYZ_Slot(int X, int Y, int Z);

signals:
    void Open_Port_Signal(QString portname); // Сигнал даем по нажатию кнопки "OPEN"
    void Pass_XY_Signal(int x_pix, int y_pix); //Сигнал по нажатию кнопки "Get_XY"
    void FW_Kinemaic_Signal(int S3, int S4, int S5, int l1, int l2, int l3); // Углы приводов, длины соответствующих звеньев.


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
