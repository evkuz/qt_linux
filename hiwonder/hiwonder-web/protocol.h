#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <QList>

/*
 * у нас структура запроса получается такая:
 * <address>/run?cmd=<name>&<parameter_name>=<parameter_value>
 *
 * <ip-address>:8383/run?cmd=<name>
 * http://192.168.1.175:8383/run?cmd=start&
 *
 *
 *
 * Поправил код в соответствии с тем, что мы тут писали, мой код возвращает:
{"lastCmd": "reset",   "lastCmdResult": 0, "status": "текущий статус"}
*
* статусы:
* init  -  распознавание еще не запустилось
* wait - жду команду
* inprogress - выполняю команду
* done - закончил выполнение команды.
*
*
*
* имена команд (cmd=<name>):
* start - начало выполнения движения
* reset - смена статуса на wait
* просто адрес:порт  - возвращает статус
*
*  Пример GET-запроса от клиента -> серверу
// "GET /run?cmd=status&lgfkhj HTTP/1.1\r\nHost: localhost:8383\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \"Chromium\";v=\"94\", \"Google Chrome\";v=\"94\", \";Not A Brand\";v=\"99\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Linux\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.54 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*//*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n" */
// "GET /favicon.ico HTTP/1.1\r\nHost: localhost:8383\r\nConnection: keep-alive\r\nPragma: no-cache\r\nCache-Control: no-cache\r\nsec-ch-ua: \"Chromium\";v=\"94\", \"Google Chrome\";v=\"94\", \";Not A Brand\";v=\"99\"\r\nsec-ch-ua-mobile: ?0\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.54 Safari/537.36\r\nsec-ch-ua-platform: \"Linux\"\r\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\nSec-Fetch-Site: same-origin\r\nSec-Fetch-Mode: no-cors\r\nSec-Fetch-Dest: image\r\nReferer: http://localhost:8383/run?cmd=status&lgfkhj\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n"






/*
 * Идея списка команд в том, что у каждого робота/девайса он будет разный. А вот унифицировать обработку - это возможно.
 * Т.е. цель - создать один файл - парсер/обработчик приходящих команд и использовать его в разных проектах с минимальными доработками, в идеале только список меняем.
*/

//                                              1                 3                    5                           7                      9                 11
    const QList<QString> tcpCommand = {"clamp", "status", "sit", "standup", "start", "put_box", "getactions", "getservices", "getservos", "reset", "lock", "unlock", "info"};
 //from QSocketThread QList<QString>  strcommand = { "/run?cmd=", "/service?name=", "/status", "/status?action="};
//
#endif // PROTOCOL_H
