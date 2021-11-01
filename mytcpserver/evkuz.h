#ifndef EVKUZ_H
#define EVKUZ_H
/*
 *  https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
 * //+++++++++++++++++++++++
 * 29.09.2021
 * Описание на память.
 * Программа демонстрирует запуск Tcp-сервера, который слушает порт 8383.
 * При появлении входящего tcp-соединения (onincomingConnection) на этот порт,
 * сервер создает объект класса QThread, объект класса QSocketThread, наследник QObject
 * Далее объект QSocketThread через moveToThread попадает в поток, организованный
 * объектом класса QThread.
 * Таким образом объект класса QSocketThread у нас начинает работать во вновь
 * созданном потоке.
 * В QSocketThread создается объект класса TcpSocket, т.е. обычный сокет.
 * Этот объект через сигналы/слоты начинает обрабатывать сигналы
 * onreadyRead, ondisconnected.
 * По сигналу onreadyRead происходит считывание из сокета пришедших данных.
 * А это должен быть GET-запрос от http-клиента.
 * В ответ клиенту отправляется время, обернутое в http-строку, и это время отображается
 * на веб-страничке клиента.
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
