#include "webserver.h"

//WebServer::WebServer(QObject *parent) : QObject(parent)
WebServer::WebServer()
{
  memset(status_buffer, 0x41,sizeof (status_buffer));

}
//++++++++++++++++++++++++++++++++++++++++++++
WebServer::~WebServer()
{}
//++++++++++++++++++++++++++++++++++++++++++++
int WebServer::openSocket()
{
    str.sprintf("========== Openeing Socket : %d\n", this->port);
    emit this->Data_TO_Log_Signal(str);


    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        err(1, "can't open socket");

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof (int));


    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *) &svr_addr, sizeof (svr_addr)) == -1) {
        close(sock);
        err(1, "Can't bind");
    //return 0;
    }
    double setval;
    uint16_t dsetval;

    listen(sock, 5);
  //  printf("========== Socket listening on port %d:\n", port);
    str.sprintf("========== Socket listening on port : %d\n", port);

   //emit this->Data_TO_Log_Signal(logmessage);

}//openSocket
//+++++++++++++++++++++++++++++++++
void WebServer::Write_To_Log(QString log_message)
{
  emit Data_TO_Log_Signal(log_message);
}
//++++++++++++++++++++++++++++++++++++++++++++ Тут и Read и Write
// Это слот для сигнала Process_A(), приходящего из потока, где слушается сокет.
// Считываем данные из сокета (GET request), отправляем в класс MainWindow на обработку.
// main idea is read data from socket and send them for parsing
void WebServer::Output_Data_From_Client_Slot ()
{
    client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);

if (cli_addr.sin_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in *)&cli_addr;
    port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
}
else { // AF_INET6
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&cli_addr;
    port = ntohs(s->sin6_port);
    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
}



  //  printf("========== Connection from %s Request:\n",ipstr);

    if (client_fd == -1) {
        perror("Can't accept");
        //continue;
    }

    timer = time(NULL);
    u = localtime(&timer);
    f = settime(u);

    //++++++++++ Читаем запрос в буфер request - http://localhost:8383/putcube/
    rdlen = read(client_fd, request, 2000);

    //+++++++++++ Парсим запрос на предмет слова "putcube"
/*        char parsing[7];
    std::string mystr;
    strcpy(mystr, request);
    strcpy(parsing, request_1);
    if (mystr.find(parsing) != std::string::npos){
        std::cout << "found!" << '\n';
    }
    else {
        std::cout << "NOT === found!" << '\n';
    }
*/

    // Данные считали. Отправляем роботу команду, парсим GET запрос.

    str.asprintf("%s", request);
    emit this->Data_TO_Log_Signal(str); // ОТправили данные GET-запроса в класс MainWindow, там их ловит слот Data_From_Web_SLot

// +++++++++++ Далее идет подготовка ответа и отправка (ответа) клиенту.

//    strcpy(request, this->status_buffer);
//    str.asprintf("%s", request);

//    str+= " Status buffer"; // Это имитация данных, полученных от Робота
//    emit this->Data_TO_Log_Signal(str);

//    strcpy(request,"Take the cube, pleae, bro");
//    str.sprintf(request);
//    emit this->Data_TO_Log_Signal(str);


//   printf("%s\n", request);


   // Вот тут обращаемся к роботу, берем у него данные. Делаем отдельным сигнал/слотом

//    strpos = 0;
//    // Answer for example        {"p13":"13.100000","t8":"8.100000","t10":"10.100000","t3":"3.100000","v9":"9.100000"}
//    memset(resp1, 0, 2100);
//    memset(nword, 0, 1100);
//    sprintf(nword, "{\"p13\":\"%f\",\"t8\":\"%f\",\"t10\":\"%f\",\"t3\":\"%f\",\"v9\":\"%f\"}", 13.1,8.1,10.1,3.1,9.1);
//    sprintf(resp1, response, strlen(nword));
//    strcat(resp1,nword )  ;
//    write(client_fd, resp1, strlen(resp1) ); /*-1:'\0'*/

////    QString rresponse = "HTTP/1.1 200 OK\r\n\r\n%1";
////    write(client_fd, rresponse.arg(QTime::currentTime().toString()).toUtf8(), 50);

//    close(client_fd);

}
//++++++++++++++++++++++++++++++++++
// Слот сигнала Write_2_Webserver_Signal()
// ОТправляем в сокет данные клиенту.
void WebServer::Write_2_Client_Slot()
{
    strcpy(request, this->status_buffer);
    str.asprintf("%s", request);

    str+= " Status buffer"; // Это имитация данных, полученных от Робота
    emit this->Data_TO_Log_Signal(str);

//    strcpy(request,"Take the cube, pleae, bro");
//    str.sprintf(request);
//    emit this->Data_TO_Log_Signal(str);


   printf("%s\n", request);

    strpos = 0;
    // Answer for example        {"p13":"13.100000","t8":"8.100000","t10":"10.100000","t3":"3.100000","v9":"9.100000"}
    memset(resp1, 0, 2100);
    memset(nword, 0, 1100);
    sprintf(nword, "{\"p13\":\"%f\",\"t8\":\"%f\",\"t10\":\"%f\",\"t3\":\"%f\",\"v9\":\"%f\"}", 13.1,8.1,10.1,3.1,9.1);
    sprintf(resp1, response, strlen(nword));
    strcat(resp1,nword )  ;
    write(client_fd, resp1, strlen(resp1) ); /*-1:'\0'*/

//    QString rresponse = "HTTP/1.1 200 OK\r\n\r\n%1";
//    write(client_fd, rresponse.arg(QTime::currentTime().toString()).toUtf8(), 50);

    close(client_fd);

}
//+++++++++++++++++++++++++++++++++++++++++++
