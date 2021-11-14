#include <QCoreApplication>
/* a server in the unix domain.  The pathname of
   the socket address is passed as an argument */
/*
 * The steps involved in establishing a socket on the server side are as follows:

1. Create a socket with the socket() system call
2. Bind the socket to an address using the bind() system call. For a server socket on the Internet, an address consists of a port number on the host machine.
3. Listen for connections with the listen() system call
4. Accept a connection with the accept() system call. This call typically blocks until a client connects with the server.
5. Send and receive data

 */

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int sockfd, newsockfd, servlen, n;
    socklen_t clilen;
    struct sockaddr_un  cli_addr, serv_addr;
    char buf[80];

    if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
        error("creating socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, argv[1]);
    servlen=strlen(serv_addr.sun_path) +
                      sizeof(serv_addr.sun_family);
    // Связываем фд сокета с файлом на ПК
    if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0)
        error("binding socket");

    listen(sockfd,5);// НАчинаем слушать сокет
    clilen = sizeof(cli_addr);
    newsockfd = accept(
         sockfd,(struct sockaddr *)&cli_addr,&clilen);
    if (newsockfd < 0)
         error("accepting");
    n=read(newsockfd,buf,80);
    printf("I've got the message\n");
    write(1,buf,n); // To stdout of server the data now stored in the buf FROM client

    string str1(buf);
    string str2("Alex");
    if (str1.compare(0,4,str2)==0){
        write(newsockfd,"Ustas\n",6); // To client via socket
    }
    else write(newsockfd,"Wrong password\n",15);
    close(newsockfd);
    close(sockfd);
    printf("Connection closed. Type Ctrl-C");
    return a.exec();
}
