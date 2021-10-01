#ifndef EVKUZ_H
#define EVKUZ_H
/*
 *  https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
 *
 * //+++++++++++++++++++++++++++++
 * 28.09.2021
 * ЗАРАБОТАЛО !!!!!!!!!!!!! ЖЕНЯ МОЛОДЕЦ !!!!!!!!!
 * Основная идея - создаем обработчик потока как наследник от
 * QObject, а не от QThread.
 * И создаем объект_А класса QThread
 * А далее делаем обработчик->moveToThread(объект_А)
 * И усё !!! Никаих run() c exec-ами.
 * И, да, - запускаем QSimpleServer не в MainWindow, а в main.
 *
 * //+++++++++++++++++++++++++++++++++++
 * 27.09.2021
 * Рабочий вариант, НО...
 * Поток через run(), что, конечно же, неверно.




*/
#endif // EVKUZ_H
