#ifndef EVKUZ_H
#define EVKUZ_H
/*
 * 03.06.2021
 * Работаем на ветке hw-qt-class
 * Запускаем класс hiwonder
 * ...
 * Запустил.
 * Скорее сего было лишним указывать, что класс HiWonder имеет родительский класс QOBJECT.
 * Закомментировал, работает и без этого.
 * Добавил ф-цию :
 * void GUI_Write_To_Log (int value, QString log_message); //Пишет в лог-файл номер ошибки value и сообщение message
 *
 * Теперь запись в лог (один и тот же) есть и в классе HiWonder и в MainWindow.
 * Делаем фикс перед последующим переносом работы с ком-портом в класс HiWonder.
 * Пока еще запись в ком-порт из главного окна.
 *
 *
 *
 * +++++++++++++++++++++++++++++++++++++++++++
02.06.2021
Решено переписать на qt всю программу управления роботом, т.к. библиотека нейронной сети написана на С++
Попробовал подключить форму form.ui, сделанную ранее для python (однако в Qt Designer), и не пошло.
Проект не компилируется.
Оказалось, что в файле form.ui, переименованном в mainwindow.ui имя окна значилось "main", а надо "MainWindow"


- Открываем лог-файл, пишем сообщение о начале работы программы
- Открываем порт по нажатию кнопки "OPEN"
- Отправялем данные в порт (лампочки мигают)... и робот не реагирует
...
По ошибке отправлял hwr_Start_position вместо sit_down_position вот он и стоял на месте.
Теперь-то все работает :) !!!!!

- Работают кнопки "SIT" и "Stand UP"
- Переходим на ветку git hw_class и создаем в ней класс hiwonder

*/
#endif // EVKUZ_H
