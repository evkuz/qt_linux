#ifndef PROTOCOL_H
#define PROTOCOL_H
/*
 * у нас структура запроса получается такая:
 * <address>/run?cmd=<name>&<parameter_name>=<parameter_value>
 *
 *
 *
 * Поправил код в соответствии с тем, что мы тут писали, мой код возвращает:
{
  "lastCmd": "reset",
  "lastCmdResult": 0,
  "status": "wait"
}
*
* статусы:
* wait
* inprogress
* done
*
*
*
* имена команд (cmd=<name>):
* start - начало выполнения движения
* reset - сброс состояния до начального
* что_угодно - возвращает статус
*

 */
#endif // PROTOCOL_H
