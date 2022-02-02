#include <QCoreApplication>
/* a client in the unix domain */
/*
 * The steps involved in establishing a socket on the client side are as follows:

1. Create a socket with the socket() system call
2. Connect the socket to the address of the server using the connect() system call
3. Send and receive data. There are a number of ways to do this, but the simplest is to use the read() and write() system calls.

 */
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int sockfd, servlen,n;
    struct sockaddr_un  serv_addr;
    char buffer[82];

    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, argv[1]); // Путь к файл-дескриптору сокета - 1-й аргумент команды запуска
    servlen = strlen(serv_addr.sun_path) +
                  sizeof(serv_addr.sun_family);
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
        error("Creating socket");
    if (connect(sockfd, (struct sockaddr *)
                          &serv_addr, servlen) < 0)
        error("Connecting");
    printf("Please enter your message: ");
    bzero(buffer,82); // обнуляем буфер
    fgets(buffer,80,stdin); // Считываем ввод пользователя
    write(sockfd,buffer,strlen(buffer)); // Посылаем данные в сокет
    n=read(sockfd,buffer,80); // Считываем ответ из сокета
    printf("The return message was\n");
    write(1,buffer,n);
    close(sockfd);
    printf("Connection closed. Type Ctrl-C");

    return a.exec();
}
