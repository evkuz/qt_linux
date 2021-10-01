
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
#define uint8_t unsigned char
#define uint16_t unsigned short

#define request_1 "putcube"

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
//changed 20 12 04 }
/*
 * 
 */

char * settime(struct tm *u) {
    char s[40];
    char *tmp;
    for (int i = 0; i < 40; i++) s[i] = 0;
    int length = strftime(s, 40, "%ld.%m.%Y %H:%M:%S, %A", u);
    tmp = (char*) malloc(sizeof (s));
    strcpy(tmp, s);
    return (tmp);
}

int main(int argc, char** argv) {
    int one = 1, client_fd;
    int rdlen;
    struct sockaddr_in svr_addr, cli_addr;
    socklen_t sin_len = sizeof (cli_addr);
    struct tm *u;
    char *f;
    time_t timer;
// Init DAC and SPI

    //setting SPI and CS




    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        err(1, "can't open socket");

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof (int));


    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *) &svr_addr, sizeof (svr_addr)) == -1) {
        close(sock);
        err(1, "Can't bind");
	return 0;
    }
    double setval;
    uint16_t dsetval;

    listen(sock, 5);
    printf("========== Socket listening on port %d:\n", port);
    char ipstr[INET6_ADDRSTRLEN];
    while (1) {
        client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);

	if (cli_addr.sin_family == AF_INET) {
	    struct sockaddr_in *s = (struct sockaddr_in *)&cli_addr;
	    port = ntohs(s->sin_port);
	    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	} else { // AF_INET6
	    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&cli_addr;
	    port = ntohs(s->sin6_port);
	    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}



        printf("========== Connection from %s Request:\n",ipstr);

        if (client_fd == -1) {
            perror("Can't accept");
            continue;
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
       strcpy(request,"Take the cube, pleae, bro");

        printf("%s\n", request);
        
        strpos = 0;
        // Answer for example        {"p13":"13.100000","t8":"8.100000","t10":"10.100000","t3":"3.100000","v9":"9.100000"}
        memset(resp1, 0, 2100);
        memset(nword, 0, 1100);
        sprintf(nword, "{\"p13\":\"%f\",\"t8\":\"%f\",\"t10\":\"%f\",\"t3\":\"%f\",\"v9\":\"%f\"}", 13.1,8.1,10.1,3.1,9.1);
        sprintf(resp1, response, strlen(nword));
        strcat(resp1,nword )  ;
        write(client_fd, resp1, strlen(resp1) ); /*-1:'\0'*/
        close(client_fd);
    }
}



