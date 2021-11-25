#ifndef SERIAL_STUFF_H
#define SERIAL_STUFF_H
/*
 *   Reimplemented Protected
 * - qint64 QSerialPort::writeData(const char *data, qint64 maxSize)
 * - qint64 QSerialPort::readData(char *data, qint64 maxSize)
 *
 * А есть an overloaded function.
 * - qint64 QIODevice::write(const QByteArray &byteArray)
 *
 *
 * Данные на отправку (Координаты приводов) храним в массиве
 * const char sit_down_position [6] = {93, 93, 93, 48, 48, 93} ;
 * Обращаем внимание - элементы массива - знаковые значения, т.е. от -127 до 127
 *
 * В обоих случаях ф-ция требует массив const, это и есть камень преткновения.
 *
 * Хорошо, если данные на отправку в serial port заранее известны, т.е. можно их взять из массива const char.
 * А если нет? Если данные зависят от ввода пользователя ?
 * Вот тут надо каким-то образом преобразовать (привести) тип данных к типу const QByteArray &byteArray, либо const char *data.
 *
 * НАчнем с const QByteArray &byteArray т.к. в самом проекте, ф-ция, выполняющая отправку данных, принимает QByteArray :
 * void GoToPosition(QByteArray &position);
 *
 * ЗАДАЧА
 * Как нам массив const char преобразовать в const QByteArray ?
 *
 * РЕШЕНИЕ
 *
 * QByteArray dd = QByteArray::fromRawData(sit_down_position, 6);
 * Так массив dd содержит данные из массива sit_down_position
 *
 * ЗАДАЧА
 * Надо преобразовать данные QByteArray к типу unsigned char
 * Такая задача возникает, когда надо данные-числа записать, например в лог в текстовом (а не в двоичном) виде.
 *
 * РЕШЕНИЕ
 *
 * QByteArray position;
 *
 * unsigned char sData [6]= {0,0,0,0,0,0};
 * memcpy(&sData, position,6);
 * Так массив sData содержит данные массива position.
 * А можно и так :
 * str+= QString::number(position.at(i));
 *
 *
 * ЗАДАЧА
 * Преобразовать unsigned char в QByteArray
 *
 *
 * РЕШЕНИЕ
 * unsigned char Servos[6];
 * QByteArray dd ;
 *
 * dd.resize(64);
 * memcpy(dd.data(), Servos, 6);
 *
 * Так данные из массива unsigned char Servos попадают в массив QByteArray dd
 *
 * ######################################################
 *
 *
 */
#endif // SERIAL_STUFF_H
