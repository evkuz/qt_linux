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
// "GET /run?cmd=status& HTTP/1.1\r\nHost: localhost:8383\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \"Chromium\";v=\"94\", \"Google Chrome\";v=\"94\", \";Not A Brand\";v=\"99\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Linux\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.54 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*//*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n" */
//  GET /run?cmd=status& HTTP/1.1\r\nHost: 192.168.1.175:8383\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\n\r\n"
// GET /run?cmd=status& HTTP/1.1\r\n
// GET /run_action?device=hiwonder&action=putcube HTTP/1.1
// Host: 192.168.1.175:8383\r\n
// Connection: keep-alive\r\n
// Cache-Control: max-age=0\r\n
// Upgrade-Insecure-Requests: 1\r\n
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36\r\n
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n
// Accept-Encoding: gzip, deflate\r\n
// Accept-Language: en-US,en;q=0.9\r\n\r\n






// "GET /favicon.ico HTTP/1.1\r\nHost: localhost:8383\r\nConnection: keep-alive\r\nPragma: no-cache\r\nCache-Control: no-cache\r\nsec-ch-ua: \"Chromium\";v=\"94\", \"Google Chrome\";v=\"94\", \";Not A Brand\";v=\"99\"\r\nsec-ch-ua-mobile: ?0\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.54 Safari/537.36\r\nsec-ch-ua-platform: \"Linux\"\r\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\nSec-Fetch-Site: same-origin\r\nSec-Fetch-Mode: no-cors\r\nSec-Fetch-Dest: image\r\nReferer: http://localhost:8383/run?cmd=status&lgfkhj\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n"

// readAll() считывает такую строки и показывает 385 байт, при этом строка длиной 403 байт. Символы "\r\n - 16 байт, если считать \r как один символ"
//"GET /favicon.ico HTTP/1.1\r\nHost: 192.168.1.175:8383\r\nConnection: keep-alive\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36\r\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\nReferer: http://192.168.1.175:8383/run?cmd=status&\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9\r\n\r\n"


//case 10: //lock
//    mutex.lock();
//    mutex.unlock();



/*
 * Идея списка команд в том, что у каждого робота/девайса он будет разный. А вот унифицировать обработку - это возможно.
 * Т.е. цель - создать один файл - парсер/обработчик приходящих команд и использовать его в разных проектах с минимальными доработками, в идеале только список меняем.
*/

//                                            1                   3                  5                     7                           9                 11                  13
const QList<QString> tcpCommand = {"status", "reset", "clamp", "sit", "standup", "start", "put_box", "getactions", "getservices", "getservos", "lock", "unlock", "info", "collapse"};
 //from QSocketThread QList<QString>  strcommand = { "/run?cmd=", "/service?name=", "/status", "/status?action="};
//

//  1.  Use QJsonObject::toVariantMap to convert all JSON objects to QVariantMap

//  2.  Use QMap::insert to insert all maps into one

//  3.  Use QJsonObject::fromVariantMap to convert the resulting map back to JSON object



//    QJsonObject json1{{"foo_key", "foo_value"}};
//    QJsonObject json2{{"moo_key", "moo_value"}, {"boo_key", "boo_value"}};
//    QVariantMap map = json1.toVariantMap();

//    map.insert(json2.toVariantMap());

//    qDebug() << QJsonObject::fromVariantMap(map);



//enum QAbstractSocket::SocketState
//This enum describes the different states in which a socket can be.

//Constant	Value	Description
//QAbstractSocket::UnconnectedState	0	The socket is not connected.
//QAbstractSocket::HostLookupState	1	The socket is performing a host name lookup.
//QAbstractSocket::ConnectingState	2	The socket has started establishing a connection.
//QAbstractSocket::ConnectedState	3	A connection is established.
//QAbstractSocket::BoundState       4	The socket is bound to an address and port.
//QAbstractSocket::ClosingState     6	The socket is about to close (data may still be waiting to be written).
//QAbstractSocket::ListeningState	5	For internal use only.

/*
<link href="data:image/x-icon;base64,AAABAAEAEBAQAAAAAAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAEhEQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD//wAA//8AAP//AAD//wAA//8AAP//AAD//wAA//8AAP7/AAD//wAA//8AAP//AAD//wAA//8AAP//AAD//wAA" rel="icon" type="image/x-icon" />
*/

//response += "<link rel=";
//response += '"';
//response += "icon";
//response += '"';

//response += "href=";
//response += '"';

//response += "data:,";
//response += '"';

//response += ">";

//response += "</head>";
//response += "<body>";
//response += "IT'S FROM FAVICON !";
//response += "</body>";
//response += "</html>";
//response += "\r\n";

#endif // PROTOCOL_H
