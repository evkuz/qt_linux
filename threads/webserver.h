#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>

#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#include <string.h>
#include <string>
//++++++++++++
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QBuffer>
#include <QElapsedTimer>
#include <QDateTime>
#include <QTextStream>
#include <QTime>

#define uint8_t unsigned char
#define uint16_t unsigned short


class WebServer : public QObject
{
    Q_OBJECT
public:
//    explicit WebServer(QObject *parent = 0);
    WebServer();
    ~WebServer();
//++++++++++++++++++++++++++++++++++
    QFile       LogFile;
    QString     filename;
    QString     str;
    QByteArray  mystatus[3] = {"ready", "work!", "done!"};
    int counter;

#define STATUS_sz 5
    char status_buffer[STATUS_sz];
//++++++++++++++++++++++++++++++++++
#define uint8_t unsigned char
#define uint16_t unsigned short

#define request_1 "putcube"

int sock;
char ipstr[INET6_ADDRSTRLEN];

int port = 8383;
double val;

char request[2048];
char *pansw;
//changed 20 12 04 {
    char nword[1300], metod[100], protocol[100], uri[1100];
    int strpos;
    char resp1[3048];

    char response[3300] =
    "HTTP/1.0 200 OK\r\n"
    "content-type: text/html\r\n"
    "Content-Length: %d\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Server: HiWonder/1.0.0 G++ 9.3.0\r\n"
        "Date: Tue, 21 Sep 2021 06:11:57 GMT\r\n\r\n";

    char * settime(struct tm *u) {
        char s[40];
        char *tmp;
        for (int i = 0; i < 40; i++) s[i] = 0;
        int length = strftime(s, 40, "%ld.%m.%Y %H:%M:%S, %A", u);
        tmp = (char*) malloc(sizeof (s));
        strcpy(tmp, s);
        return (tmp);
    }

    int one = 1, client_fd;
    int rdlen;
    struct sockaddr_in svr_addr, cli_addr;
    socklen_t sin_len = sizeof (cli_addr);
    struct tm *u;
    char *f;
    time_t timer;
//+++++++++++++++++++++++
int openSocket();
void Write_To_Log(QString log_message);

signals:
void Data_TO_Log_Signal(QString log_message);

public slots:
    void Output_Data_From_Client_Slot();
private slots:
    void Write_2_Client_Slot(); // Слот отправки данных от вебсервера клиенту, срабатывает по сигналу Write_2_Webserver_Signal();
};

#endif // WEBSERVER_H
