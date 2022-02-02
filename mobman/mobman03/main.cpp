#include <QCoreApplication>
#include <QObject>
#include "QDebug"
#include <QString>
#include <QFile>
#include <QDateTime>
#include <QDataStream>
#include <QTextStream>

void Log_File_Open(QString lname);
void Write_To_Log (int value, QString log_message);

#define Log_File_Name        "./hiwonder.log"
QFile       LogFile;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    qDebug() << "Started";
    Log_File_Open(Log_File_Name);
    Write_To_Log (0xCCCC, "WORKS !!!");
    LogFile.close();

    return a.exec();
}
//+++++++++++++++++
void Log_File_Open(QString lname)
{
    LogFile.setFileName(lname);
    LogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

}
//++++++++++++++++
void Write_To_Log (int value, QString log_message)
{
    QDateTime curdate ;
    QTextStream uin(&LogFile);

    QString str, str2;
    //int value = 0xf000;
    curdate = QDateTime::currentDateTime();

    str = curdate.toString("yyyy-MM-dd__hh:mm:ss:z").toLocal8Bit(); str.append(" > ");
    str2 = QString("0x%1: ").arg(value, 4, 16, QChar('0'));

    uin << str << str2 << log_message << "\n";

}
