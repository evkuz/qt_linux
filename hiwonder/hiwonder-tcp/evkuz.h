#ifndef EVKUZ_H
#define EVKUZ_H
/*
 * 26.04.2022 Начало проекта
 *
 * Ветка hiwonder. Стационарный манипулятор
 * Теперь все через ЦУП, без GUI
 *
 * Проект для отладки разделения кода для разных роботов через инкапсуляцию, наследование и полиморфизм.
 *
 *
 *
 * //+++++++++++++++++++++++++++++++++++++
 * 27.04.2022
 * Заработало !
 * HiWonder принимает команды.
 * Соединение сигнала QSerialPort::readyRead() со слотом SerialDevice::ReadFromSerial_Slot() сделано в конструкторе  класса SerialDevice.
 * Так в основном файле проекта (mainprocess.cpp) не будет этой строки, меньше кода.
 * Теперь надо:
 * + Дбавить запись в лог ответа от робота.
 * + Разобраться с дублированием записи :
 *   "0xf001: Robot finished
 *   0xf001: Robot finished complex command
 *   " [Разобрался]
 * - Добавить работу с TCP/ip
 * - ДОбавить парсинг JSON-данных
 *
 *
 * //+++++++++++++++++++++++++++++++++++++
 * 26.04.2022
 *
 * Выстраиваем иерархию объекта через несколько классов.
 * Класс SerialDevice - аналог HiWonder, но без привязки к имени собственному. ОБъекты, например, - Nano, Mega - по названию плат.
 * Класс Robot (Manipulator) - Дочерний от SerialDevice. Вот тут уже свойства робота можно описывать - DOF, parcsel_size и т.д. Объекты - HiWonder, Mobman, Pinkman  - по названию робота.
 * Далее к объекту класса Robot надо прикручивать связь с другими роботами - создавать объект класса QTcpServer, работа с потоками, парсинг JSON и т.д.
 *
 * Создал класс SerialDevice - общий класс для устройств QSerialPort
 * Создал класс MainProcess - где будет основной поток программы
 *
 *
 *
 *
*/

#endif // EVKUZ_H
