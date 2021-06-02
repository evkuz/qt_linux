#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QtSerialPort/QSerialPort>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QFile>



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
    QFile       LogFile;
    QString     target_name;

#define Log_File_Name        "./hiwonder.log"
    void Log_File_Open(QString lname);
    void Write_To_Log (QString log_message);

private slots:
    void on_openButton_clicked();

    void on_sitButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
