#ifndef GARAINBOW_H
#define GARAINBOW_H
/*
 * 18.07.2023
 *
 *
 *  В самом простом виде генетический алгоритм имеет следующую структуру (см. схему):
 *  - начинаем с некоторым решением, в нашем случае, это случайная строка;
 *  - вносим мутации, например, меняем случайно выбранную букву в строке на случайно выбранную букву
 *    (вначале используеи shuffle, а можно просто случайный индекс и случайный символ)
 *    и получаем новый набор строк (k мутаций на строку); из них отбираем только те, которые ближе к исходной
 *    (по количеству совпадающих символом), например 10 таких строк, если исходной среди них нет,
 *    то запускаем процесс заново.
 *
 * 1. Генерация случайного поколения особей-решений
   2. Выбор наиболее качественных особей поколения
   3. Скрещивание особей для получения их потомков
   4. Мутация потомков
   5. Получение нового поколения

   Функция пригодности (ФП) - количество совпадаюищих с исходной строкой символов. (отбираем только те, которые ближе к исходной
  (по количеству совпадающих символом)

   Начальная популяция(набор символов) - исходная строка, перемешанная до неузнаваемости.

   Алгоритм вычисления ФП :

   1. Находим первое совпадение символов

   Формируем массив из словарей.
   В каждом словаре ключ - число совпадений всего, число совпадений более 1 символа подряд, индексы совпадающих символов

 */
#endif // GARAINBOW_H
