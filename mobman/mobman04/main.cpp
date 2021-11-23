#include <QCoreApplication>
#include <QObject>
#include "QDebug"
#include <QString>
#include <QFile>
#include <QDateTime>
#include <QDataStream>
#include <QTextStream>
#include <QRegExp>

// servos=75,90,45,45,45,45,49,222

void Log_File_Open(QString lname);
void Write_To_Log (int value, QString log_message);
QString parse_command (QString command);
void analyze_command (QString command);

#define Log_File_Name        "./hiwonder.log"
QFile       LogFile;
unsigned char Servos [6] = {93,93,93,93,93,93};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Started";
    Log_File_Open(Log_File_Name);
    Write_To_Log (0xAAAA, "WORKS !!!");

    QString tcp_command="/run?cmd=servos=25,35,45,55,65,75,125,222&";
    //parse_command(str_command);
    QString mobman_command = parse_command(tcp_command);
    Write_To_Log(0xBBBB, mobman_command);

    // Now get analyze command
    analyze_command(mobman_command);
    // Now other command
    tcp_command="/run?cmd=status";
    mobman_command = parse_command(tcp_command);
    Write_To_Log(0xBBBB, mobman_command);
    analyze_command(mobman_command);

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
//++++++++++++++++++++++++++++++
QString parse_command (QString command)
{
    //Парсим команду.
    QString message, substr;
    message = command;
    int sPosition, ePosition; // Индекс начала и конца команды в строе.
    sPosition = message.indexOf("/run?cmd=");

    sPosition += 9;
    ePosition = message.indexOf("&", sPosition);
    substr = message.mid(sPosition, (ePosition - sPosition));

    // Получили команду. Передаем её наверх
    qDebug() << "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM TCP SOCKET !!!!!!!!!!!!!!!!!!!";
    qDebug() << substr;

    return  substr;

}
//+++++++++++++++++++++++++++++++++++++
// Определить значения углов приводов, записать в массив Servo
void analyze_command (QString command)
{
    QRegExp rx("^servos=.*");
    QString message, substr;
 //   int sPosition, ePosition; // Индекс строки в запросе.

    //if (rx.exactMatch(command)){
      if(command.startsWith("servos=")){
        qDebug() << "Yes, command for servos ";
        // So get servo values
        substr = command.remove("servos=");
        qDebug() << "servos are " << substr;
        // now split the string and store substrings to servos array
        QStringList list1 = substr.split(QLatin1Char(','));
        for (int i=0; i<list1.size(); ++i)
        {
            Servos[i] = list1.at(i).toUInt();
            qDebug() <<"Servo["<<i<<"]=" << Servos[i];
            // so now we have here servos array with actual values
        }//for
    }//if
    else qDebug() << "No, other command";



}
