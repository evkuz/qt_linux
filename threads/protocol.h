#ifndef PROTOCOL_H
#define PROTOCOL_H
/*
 * у нас структура запроса получается такая:
 * <address>/run?cmd=<name>&<parameter_name>=<parameter_value>
 *
 * <ip-address>:8383/run?cmd=<name>
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

 */
#endif // PROTOCOL_H
