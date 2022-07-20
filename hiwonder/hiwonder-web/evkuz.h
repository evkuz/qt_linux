#ifndef EVKUZ_H
#define EVKUZ_H


/*
 * https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
 * В к. 406Б использую kit : Qt 5.15.2 (gcc_64)
 * В к. 240Б использую       Qt 5.15.2 (gcc_64)for Desktop
 *  (true = 1 and false = 0)
 *
 *  false == 0
 *  true == 1
 *
 * Ветка hiwonder. Стационарный манипулятор
 * Теперь все через ЦУП, без GUI

 * Проект для более сложной презентации, где участвуют 5-6 роботов
 * Берем кубик с розовой полочки Хрёбота и кладём на пол, где "Ручник" подберет кубик и увезет куда надо.
 *
 * Запускаем supervisor  ~/IQR/supervisor/start.sh и наблюдаем  по адресу http://localhost:5050/
 *
 * Запаускаем gui-client /home/ubuntu/iqr_lit/mobman/gui-client/release/gui-client
 *
 * Запускаем "HIWONDER http server" - http://192.168.1.175:5001
 *
 * Запускаем команды вручную - http://192.168.1.175:8383/run?cmd=getactions&
 *
 * // Synchronizing Threads
 * https://doc.qt.io/qt-5/threads-synchronizing.html
 * https://www.toptal.com/qt/qt-multithreading-c-plus-plus
 *
 * qabstractsocket taking get request from chrome as 2 different connections
 *
 *   The program has unexpectedly finished.
 *   The message "Segmentation fault" means that the program tried to access an area of memory that does not belong here.
 *
 *  https://habr.com/ru/post/551532/
 *  D-Bus в основном используется как локальный IPC поверх сокетов AF_UNIX.
 *  Служба идентифицируется именем в обратной нотации доменного имени.
 *
 *  http://0pointer.de/blog/the-new-sd-bus-api-of-systemd.html
 *
 *   Ensuring programs are race-free is one of the central difficulties of threaded programming.
 *
 * //18.07.2022
 * Убрать отладочные сообщения для команд start/put_box
 * Так они быстрее отрабатывают.
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * //15.07.2022
 * Долго провозился с логикой ситуации :
 * "Если не запрос статуса, И не reset, но есть активный экшен (еще вопрос - тот же самый или другой ?)"
 * Забыл, что отключил запись в лог для этого случая, при этом отправка в tcp-socket осталась - gui-client в очередной
 * раз выручил, там эту запись видно.
 * В общем в ответе код RC-меняю, а в jsoninfo нет, ибо логика существенно усложняется.
 * В общем сервер, отправивший команду, получает ответ, что экшен не запустился, т.к. в это время уже выполняется какой-то,
 * возможно тот же самый экшен.
 * А hiwonder ничего не меняет в состояниях запрошенного и выполняемого экшенов.
 * При выполнение команд start и put_box были ошибки с флагом LASTONE - не было его, там где надо.
 * В одном месте был AFTER_PUT, убрал его. Так вот если придется возвращать, то понадобится перепрошивать arduino, похоже.
 *
 * Теперь следующий шаг - сформировать ответ на запрос "/status?action=[name]"
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++
 * 12.07.2022
 *
 * Start at 1:54 in Realease mode WITH SUPERVICOR (p-mode)
 * Droppeed at 1st detection command :)
 *
 * 14.05 debug
 *
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++
 * 11.07.2022
 * Еще пунктик.
 * Если не запущен процесс/service с именем robot.hiwonder.service - камера, детектирующая кубик, то
 * при вызове ф-ции SocketClient::GetState прога просто виснет, не выводит никаких сообщений.
 * Решение :
 * - Запустить через QProcess проверку, что запущен нужный процесс.
 *
 * Поправил обработку ситуации DETECTED/"NOT DETECTED"
 * Делаем фикс на гит, включая qtlibs
 *
 * Back to "The program has unexpectedly finished."
 *
 * 1. Проверяем без детекции, т.е. процесс robot.hiwonder.service - не запущен и запросы статуса не шлет.
 *    Start at 11:47 IN RELEASE MODE ...
 *    Stop at 12:46, counter value 35000
 *    All is OK.
 *
 * 2. Проверяем с запущенным процессом robot.hiwonder.service, но в тестах не запускаем команду
 *    "start"  - так поймём, что сбой либо из-за взаимодействия процессов
 *    robot.hiwonder.service <=> hiwonder-web,
 *    либо из-за сбоя в потоках.
 *
 *    Start at 12:50 in Release Mode, polling with 100ms interval
 *    Stop at 13:45 Counter is 37700.
 *    All is OK.
 *
 * 3. Подозрения в пользу сбоя при детекции.
 * Старт в 14.00
 *
 *
 *
 * //++++++++++++++++++++++++
 * 10.07.2022
 * Search for dangling pointer.
 *
 * robot.hiwonder.service is off
 * supervisor is off.
 * Delay between request is 100ms
 *
 * Start at 16:45 IN DEBUG MODE ...
 * Stopped at 17:45, counter is 37135
 *
 * Test Number 2
 * robot.hiwonder.service is ON
 * supervisor is off.
 * Delay between request is 100ms
 * start at 17:53 IN DEBUG MODE ...
 * Stopped at 18:52, counter is invisible, but everything worked fine.
 *
 * start at 19.45 IN DEBUG MODE ...
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 09.07.2022
 * Сделаем отдельную запись, ибо важно.
 * Каждый раз, когда меняем значение конкретного экшена, делаем следующее :
 * - Меняем значение, записанное в JsonInfo::jsnObjArray, на вновь измененное
 *   Для этого запускаем ф-цию JsonInfo::setActionData(QJsonObject &theObj)
 *   ...
 *   (Ага, и получаем dangling pointer, блин ...)
 * - (1)Если приходит команда запуска экшена, а в этот момент выполняется другой экшен
 *   ( т.е. jsnStore->isAnyActionRunning = true;), то :
 *   - Записываем в QJsonObject tempObj нужные значения для такой ситуации. Тут самое важное -> {"rc", RC_UNDEFINED}
 *   - Устанавливаем значения вызываемого экшена в значения из tempobj через jsnStore->setActionData(tempObj); (ВАЖНО !)
 *   - Отправляем в сокет.
 *   И так будет, пока текущий экшен не завершится и значение jsnStore->isAnyActionRunning не изменится на false.
 *   - При следующем запросе запуска этого же экшена, у него статус RC_UNDEFINED, значит при попадании в
 *     ProcessAction у него только сменится RC : RC_UNDEFINED -> RC_WAIT
 *     И, что, СУКА, самое важное -  значение сменится не у самого экшена, а у QJsonObject &theObj, который передается в
 *     ProcessAction, а это всего лишь MainProcess::mainjsnObj с какими-то значенияем.
 *     Сам экшен, как объект класс JsonInfo при этом не меняется ! Вот поэтому и происходит зацикливание в (1)
 *     ...
 *     Добавил изменение самого объекта из JsonInfo. Теперь по логике нужно еще раз посылать команду на запуск экшена,
 *     чтобы он сработал со статусом RC_WAIT. Т.е. лишняя команда.
 *     А хорошо бы рекурсивно запустить ф-цию MainProcess::ProcessAction, чтобы уже с новым значением экшен запустился без
 *     доп команды из сокета...
 *
 * ...
 *
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 08.07.2022
 * Добавил запись из MainProcess в отдельный файл.
 * Запустил в 9.22. Ждем. 9.53 - отвалилось.
 *
 * - ДОбавил сигнал/слот обработки ошибки Serial Port.
 * Запустил gui-client в 11.18 (без supervisor) через 20 минут в gui-client :
 * QIODevice::read (QTcpSocket): device not open - но при этом hiwonder-web продолжает открвать потоки и
 * в консоли идут сообщения.
 *
 * QMetaObject *QObject::metaObject() -  Returns a pointer to the meta-object of this object
 *
 *
 * - Переименовал сигнал QSocketThread::finished() в QSocketThread::stopThread_signal
 *   Добавил вызов stopThread_signal после отправки данных в сокет и закрытия в слот
 *   QSocketThread::Data_2_TcpClient_Slot
 *
 * - Убрал обработку сигнала  &QAbstractSocket::disconnected
 * В целом, из firefix можно работать.
 * Осталась проблема "залипание в статусе inprogress", и reset не помогает.
 * Да, это проблема чисто ручного управления, но она возникнет, если роботы будут
 * общаться не через ЦУП, а напрямую. Так что решать надо.
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 07.07.2022
 * Нужно провести тесты из supervisor.
 * В ручном режиме + gui-client Пока все работает.
 * При тестах через "HIWONDER http server" - http://192.168.1.175:5001/
 * Спонтанно вылетает.
 * Предположительно при записи в лог - что важно ДАННЫХ ИЗ SERIAL PORT - или в консоль этих же данных.
 * Надо попробовать за mute-ить это.
 * Тут может быть конфликт записи в файл/консоль, т.к. 2 желающих - Serial Port и TcpSocket
 * QMutexLocker ... MainProcess::Moving_Done_Slot()
 * ...
 * И еще. Если приходит неизвестная команда, сейчас просто закрываем сокет.
 * Надо попробовать отправлять минимальный ответ типа "UnKnown command".
 * ...
 * Оставил на 1.5 часа. Только опрос статуса от supervisor  - прога вылетела...
 * Утечки памяти ?
 * ...
 * Поймал момент возникновения ошибки.
 * Если экшен почти завршился и в этот момент послать запрос на другой экшен, то
 * тот, который завершился, - остается в статусе "inprogress" !!!
 * Точнее, при команде "start" в самом статус остается "inprogress" !!!
 * Потому что в этот момент приходт запрос статуса из процесса robot.hiwonder.service (!),
 * даже кнопок нажимать не надо...
 * Это случается и в середине этапа выполнения экшена, не только в конце.
 * Возможно, слот MainProcess::Moving_Done_Slot() вообще не запускается.
 * И даже  команда reset не помогает
 * Дабавил Qt::QueuedConnection в соединение сигнал/слот для serialport
 * ...
 * При выполнении "put_box" не видно action_list ["inprogress"] action_list идет пустой.
 *
 *
 * Оставил в 20.02 на ночь программу поработать. Счетчик на 1500.
 * Проверим удержится ли сокет...
 *
 * Программа вылетела в 21:04:50, счетчик на 18772. Т.е. отработала 1 час (!)
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 06.07.2022
 * Проблема с maxPendingConnections().
 * По умолчанию их не более 30.
 * Почему-то программа не считает, что сокеты закрываются, и продолжает увеличивать счетчик.
 * При достижении 30 соединений - т.е. просто 30 раз отправлена команда - перестает принимать новые запросы...
 * ...
 * А всё из-за строчки :
 * connect(tcpthread, &QSocketThread::isCreatedSocket_Signal, this, &QSimpleServer::isCreatedSocket_Slot); // get QTcpSocket *socket from tcpthread
 * там происходит addPendingConnection - вот он и накручивал счетчик, несмотря на то, что соединения закрыты...
 *
 * https://doc.qt.io/qt-6/qtcpserver.html#addPendingConnection
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 05.07.2022
 * Еще одна задача :
 * - Записывать в файл вывод в консоль из qDebug()
 *
 * В ответ на запрос иконки отправляю response += "<link rel=\"icon\" href=\"data:,\">";
 * в составе html страницы. Так не работает.
 * Браузер ждет в ответ картинку, а получает html, поэтому снова шлет запрос на иконку.
 * ....
 * Заработало при помощи ссылки response += "<link rel=\"icon\" href=\"data:,\">";
 * Пришлось добавить в conten-type и html, и json, т.е так :
 * "content-type: text/html application/json\r\n";
 * При этом запрос на иконку пропал, но json-данные теперь выводятся в 1 строку.
 * Надо это протестировать в supervisor
 *
 * Также отправлял картинку. Ошибок нет, она доходит, но при этом продолжается запрос на иконку. Кэш браузера чистил.
 * Пробовал и favicon.ico и favicon.png
 * Проблема, скорее всего в том, что данные должны быть в двоичном виде...
 * Добафил Ф-цию  QSocketThread::returnIconChrome() :
 * - считывает данные из файла в QByteArray,
 * - формирует из них строку QString
 * - Отправляет полученную строку как результат.
 * - Исправил на отправку QByteArray, получаю free(): invalid pointer - понятно.
 *
 *
 * Ф-ция QSocketThread::returnIconChrome() отрабатывает правильно.
 * Смотрел в hex-editor данные получаемого файла favicon.ico (браузер все полученные данные называет так) - совпадают с тем, что отправляется.
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 04.07.2022
 * Сейчас запрос favicon.ico просто игнорируется, а надо добавить ответ.
 * Пока вижу решение - отправлять файл favicon.ico минимального размера.
 * После этого chrome должен перестать запрашивать favicon.ico.
 * Либо есть еще такой вариант :
 * You can completely remove the automatic favicon request by adding the following line of code
 * to the <head> section of your HTML page:
 * <link rel="icon" href="data:,">
 * This little code snippet assigns an empty data URL to the favicon’s <link> element which specifies
 * the location of the external resource. This trick stops the user’s browser from sending an automatic
 * HTTP request for the favicon.
 *
 * You can generate a base64 string of your favicon and then insert it into the head of the html document like this:
 * <link href="data:image/x-icon;base64,<your_base64_here>" rel="icon" type="image/x-icon" />
 * <link href="data:image/gif;base64,R0lGODlhIAAgAOf/ADEzMDo0LjY2O9oNHS09P101P1c4NTBCR2A3KHQwO8QaK4otL88aJWw3LL4gJL4gKcAhH5osMT1HO8khKdIeLURGQ9wcJssiJCpOYNUhKVxEQ94gLklLSFNJTENOTnFIGk5ORsAvLWFLPmVLNcwwMzpYZrY3QURWYIBJPT1bUideeoJITU5VZ1pWVVtVXyJjhFxbU0xhSWVaStQ7PVtcZT1oTRlskkNjfAB1psJHOzJxR5tVTWplZLdSQGNndAB+tRqEPXBpUXJnaACBvllxXR6FRppgU1B5V1V5TMdYRGtwZwCGzMxXSySLQ8pYXF51hdZXUrNjWHd2XFmCYGOAZTmGpziQSS+TSn14ceFdWT6PT85kU4J7URePxzaSURqOzESOWrdtXjeWRzqZSb1vbaF/KJZ7elOSZclxXDSfTlGMtk6WXCyVzaKBONxsYchxbTiTxXiJdyaY1mmQZ0GfT0WUwkagSkCiWNl2ZD6nVV2eZEumUWecbWWeZcWAdDqg0kmqTF6iYVuZz0Ohx0esU3WaflWoWU6h3O9+d1CxUnCkdlOwWJudU0y1TpCbis+JhtuHeu2CeFil2pybd32kel6k4NeKgIGjiPODgFi1Y1q5WauhY2ur3G61c2K8Y2a7bbKgn5WskZKujG6y2/KUg7WnfG/AaMykmdyhirCrqXjDcs2vPpa2nc2yJdikkZ63kIjBg4bDe4G65cy1Spm+jtS4Nqa8nuWrnZC+5dS2b43Jhda6RZjEntS7Vcy8Z9u/NN6/NdG8gMLDh+HEMLLFsd/EQcXEldvFZ5vM5uLIUKHUmejKN7POstTIkq3RrOrML9rOeqfZpfDQKKjZq/TDuuvGvO7UK57Z8dzQlvHWIObPi+zUXMfXveHSpu3XU9jXlPbbJvTbR7fiuOLYpOrZg/vfHt/dk+ban/bhLeTZuPzgLvnjIMrixvXmIvziQPnlPObgsfTlV/nmR/XoR/rnUePkufvnYvnoafnocfbqfPHouvPsw//vqP/yt//2mJCRlCH+EUNyZWF0ZWQgd2l0aCBHSU1QACH5BAEKAP8ALAAAAAAgACAAAAj+AP8JHEiwoMGDCBMqXMiwoUOC1W6hQnWr2kOE1Pyg2cIkSZItW9A8snhR4KkeeB48cLByZYgQOd6ULCQCCgkGOC9cYEBhggIFEEw8hDUF1IwMGSZAiRTJTQgKGSxcmPCgIaxolFLNGJAFkpEVKHZYQnTBAgWpThby8jQtWpxHWfwUoPHEhwsDYUhZiLphgCuFnRZ9ikULFJkOL+ocEqRmCIsomDZItsAkITNDhDSp0uXIjIo/h0bJ4iSnSwcECRZEeDCjG0JFezKb0sUMho0uknBdkyVp0A1jpbjI0NCgDUI9dPYk8qSLV4XbuZHJkgPnxCZ86ML5GvEBYaAxdBL+aXYmQcUSOZxGVZJT5cAme+vKZStWxvsVOoCWs4vhwcaSL2x88UMJAYwzzzrgSPNMLcd5MYYdhDSizCUVpPACDj/ggIEAQfTTTjnoPDNMLfAcBMYVVqRBSCKLiBOHBAIQcAABAIAAzz3xWbNgKwiBocUVYtgR2yfiXKJECxVwIMU++cSX4DK1rPLaGk1YIQYdeRCSyStYYFGPPvzg42Q20oyYC0LcrFFEE1dckcYdc/AwiT/53CMPOuWUA441I86STkJ8rAEEm2JQIYQw+NhDz51OKvjLLr4oZMsZWgBRBBFYmHNPPPS8o8468WVjzaO9NLMQK31YUUQF38Qjjzxa74CKIJnAFFNqQ6HooYUOk8zjDjrqlDOriMn0EsxDtvDBRw2MxLqOOjouk0wyx2BTEjGiUHIEI/G4E44322xzDDQlliQQN6xcgoQw5JCjzTnmxivvvPTWW1JAADs=" />

 * Ф-ция QSocketThread::favIconAnswer() срабатывает. Однако под вопросом отправка данных в socket.
 * Клиент принимает ответ, но на экране браузера не видно.
 *
 *
 * Также нужно добавить программку - парсер заголовков. [сделано в onReadyRead]
 *
 *
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 30.06.2022
 * Посмотрим, как удаляются потоки.
 *
 * Выяснилось, что команда "run?cmd=status&", посылаемая из браузера приходит как 2 ,
 * тогда как эта же команда, посылаемая из gui-client приходит как 1 "QSimpleServer::incomingConnection".
 *
 * Задача :
 * - Разобраться с разделением команды из браузера на 2 посылки.
 * ...
 * Разобрался. Это потому, что Chrome посылает запрос на файл "favicon.ico".
 *
 * Особенность браузеров, построенных на платформе Crome в том, что они ищут файл favicon.ico для каждого сайта.
 * И если его нет, то они все равно будут упорно его искать. При каждом обновлении страницы, отдельным запросом к
 * серверу.
 *
 *
 *   - Нужен качественный парсер http-запросов
 *   https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview
 *   - Нужно менять механизм считывания данных из TCP-socket
 *   Сейчас на каждую новую порцию данных срабатывает incomming connection, а браузер посылает запрос в 2 посылки.
 *
 * https://habr.com/ru/post/309436/
 * Mozilla - дает 1 сокет, запрос размером 365 байт.
 * "There are less than 400 bytes of available data !!!"
 *
 * Проверить, есть ли в мозиле заголовок Content-length ?
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 28.06.2022
 * Проверил нагрузку - 10 запросов в секунду + вручную из браузера - норм.
 *
 * ПО логике работы.
 * Если запустить экшен, то получаем НЕпустой action_list.
 * Если во время выполнения экшена запустить его повторно (имитируем ошибку) то action_list
 * становится пустой, хотя экшен еще выполняется.
 *
 * Разобраться, почему в ф-ции MainProcess::on_trainButton_clicked()
 *
 * Значения JsonInfo::jsnActionStart НЕ меняются через jsnStore->setActionData(mainjsnObj);
 *
 * Но меняются напрямую через доступ к public методу :
 * jsnStore->jsnActionStart["rc"] = JsonInfo::AC_Launch_RC_DONE;
 *
 * - Делать запуск, даже если rc = -2 //  "- action с таким именем не запустился"
 * И в ответве на запуск экшена должно быть 3 поля.
 *
 * 6. Hiwonder на QT подвисает при запросе start если нет детекции - проблему выяснил, почти исправлено

7. Исправить ошибку двойной отправки ответа от hiwonder QT (http://192.168.1.175:8383/run?cmd=start&) Принести ноут. ИЗ gui-client все нормально приходит.

8. hiwonder QT отправляет повторно тот же json что и на предыдущий запрос: - Разобраться с reset
после перезапуска:
 * http://192.168.1.175:8383/run?cmd=reset& - ответил rc: -4 и name: "reset" ???
 * http://192.168.1.175:8383/run?cmd=start& - ответил rc: -4 и name: "reset" ???
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 27.06.2022
 * При выполнении команды "start" нет детекции, но это никак не отражается на статусе.
 * Надо исправить.
 * Договорились, что на глобальном статусе это и не должно отражаться.
 * А должно быть видно при запросе action. Т.е. ответ должен быть [-2] - - action с таким именем не запустился
 *
 * Добавил ответ на случай "action с таким именем не найден".
 *  ОТвет реализован по стандартной схеме.
 *  - Если ф-ция getIndexCommand(substr, tcpCommand) не находит такую команду, то возвращает -1
 *  - switch (comIndex)
 *  case -1: -> ProcessAction(comIndex, mainjsnObj);
 *  - ProcessAction выдает ответ по TCP в соответствии с протоколом.
 *
 *  Сегодня mutex.lock()/unlock() не помогает, hiwonder-web падает периодически.
 *  Фиксим на гит, потом разбираемся.
 *
 * Экшен reset должен переписывать все статусы...
 * Если нет детекции, то в консоли вижу
 * sscanf result: 3
 *
 * И прога на этом зависает. Раньше так не было. Было понятно, detected или not detected
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 24.06.2022
 * Подправивл gui-client, теперь можно останавливать поток с запросом статуса и снова его запускать
 * по кнопке. Пока "завалить" hiwonder не получается.
 *
 * Возможно надо будет залочить всю ф-цию MainProcess::Data_From_TcpClient_Slot(QString message)
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 21.06.2022
 * Ставим mutex.lock()/unlock() везде, где есть подозрение на конфликт доступа.
 *

 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 03.06.2022
 * При поллинге в 200мс. Все работает устойчиво.
 * При поллинге в 50мс. (это 20 входящих подключений в 1 сек. всего :( )  в какой-то момент вылетает без предупреждения.
 * Клиент gui-client сообщает ошибку QAbstractSocket::ConnectionRefusedError:
 *
 * Предполагаемый Источник проблемы :
 * - Разные потоки пытаются получить доступ к одним данным.
 *
 * Посему текущие задачи :
 * - Понять, что действительно дело в доступе к одним данным из разных потоков. Назовём это доступ к "занятым" данным.
 * - Как изолировать данные для разных потоков ?
 * - Как избавиться от вылетания, а, хотя бы, сообщать об этом в лог и уже потом закрываться ?
 *
 * В общем, обозначим проблему как "QTcpServer high amount of concurrent connections" и наши
 * connections типа "short-lived"
 *
 *
 * Просто считывать значения - должно быть безболезненно.
 * А вот изменять значения - это надо обрабатывать продуманно.
 *
 * Смотрим в сторону Qt Concurrent
 * - https://doc.qt.io/qt-5/qtconcurrent-index.html
 * - https://doc.qt.io/qt-5/qtconcurrentexamples.html
 *  - QThreadPool
 *
 *  По умолчанию QTcpServer поддерживает 30 входящих соединений одновременно.
 *  QTcpServer::setMaxPendingConnections == 30 by default.
 *  Увеличим это значение до 50.
 *  ...
 *  НЕ помогло.
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++
 * 02.06.2022
 * Добавил опцию при создании сокета:
 * socket->setSocketOption(QAbstractSocket::LowDelayOption,1);
 * При этом gui-client перстал выводить ошибку :
 * "QIODevice::read (QTcpSocket): device not open"
 *
 * Возвращаемся к задаче от 25.05.2022
 * ПО ведет себя неправильно, если во время выполнения экшена приходит команда на другой экшен...
 * Причем другой экшен выполняется тоже, но "предыдущий" остается в статусе "inprogress" навсегда, сбрасывается только по reset.
 * В общем, имеем ситуацию, когда выполняетя экшен, приходит команда - другой экшен. Этот другой - тоже пишется в буфер QSerialPort и
 * выполняется после завершения первого.
 * И мы имеем ситуацию, когда 2 экшена в статусе "inprogress" - для другого роботоа может и нормально, но манипулятор только 1 операцию
 * может выполнять одновременно.
 * Поэтому в этой задаче - HiWonder будем отключать.
 * Пока в такой ситуации будем отвечать :
 * - (-2) - action с таким именем не запустился
 * - ждать когда завершится текущий экшен.
 *
 * Ф-ция уже есть - это JsonInfo::createActionList(), поэтому вводим
 * переменную isThereActiveAction, меняем её значчение в этой ф-ции.
 * Сделано, все работает.
 *
 * Мысль.
 *
 * Сообщения по SerialPort - это же из отдельного объекта класса HiWomder... их в отдельный поток, т.е. не в главный.
 * Т.к. он блокируют вывод статуса...
 *
 * Возвращаемся к задаче вылетания ПО при частом опросе статуса.
 *
 * Going to send data to socket device
 * Going to send data to socket device
 * Going to send data to socket device
 * Segmentation fault (core dumped)

 * Сейчас вываливается при опросе каждые 50мс.

 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++
 * 01.06.2022
 * Опробовали polling от gui-client в один поток. В интервале 200мс - норм. Все записи в логе ровные.
 * ДАльше время не уменьшал.
 * Следующая задача, пожалуй, - сделать  hiwonder-web как systemd сервис. Как сделано в mobman
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++
 * 31.05.2022
 * systemctl stop robot.hiwonder.service
 * Слипаются ответы на запрос статуса, отправляемые клиентом с частотой 2Гц.
 * Разбираемся.
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++
 * 29.05.2022
 * - Заменить строку if (QString::compare(str, DEV_ACTION_STATE_RUN)) на проверку явного равенства
 * - Сравнить размер массива до и после createActionList
 * данные для "после" вставить в info.
 * Победил.
 * Неочевидный глюк сам себе создал.
 * Перебирал массив и удалял из него элементы по индексу... а за индексом не следил.
 * Примечательно что строка
 *    myArray = jsnObjArray;
 * РАБОТАЕТ.
 * НО, строка
 *    myArray.swap(jsnObjArray);
 * НЕ РАБОТАЕТ...

 *
 * Приступаем к задаче :
 * - менять jsnHeadStatus, при запуске экшена пока так :
     "state" = "running" и "rc" = 0, если выполняется экшен
   Создадим отдельную ф-цию, аналог jsnStore->setActionDone(mainjsnObj);
 * [Сделано, ф-ция setJsnHeadStatus(QJsonObject &theObj)]
 *
 * В целом, весь функционал работает, осталось нарастить обработку остальных
 * экшенов и команд от tcp-клиента.
 * И, как вишенку на торт, сделать вывод ключей json-объекта не по алфавиту, а в порядке инициализации (nlohmann ?).
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++
 * 28.05.2022
 * Также, если в момент выполнения команды приходит запрос статуса, то сейчас же меняется
 * HiWonder::active_command
 * MainProcess::Data_From_TcpClient_Slot->comIndex, что, пожалуй, неверно...
 * надо их как-то mute-ить.
 * Еще мысль - работу с SerialPort выделить в отдельный поток.
 *
 *
 * Сделан простой тест : При ответе на запрос статуса отправлять простую строку
 * str = "{\n\t\"status\": \"testing\"\t\n}";
 * И все заработало...
 * Возможно из-затого, что малый объем посылки...
 * Проверяем...
 * Работает и с большой посылкой. Так что размер не имеет значения.
 * Идем дальше.
 *
 * Проблема найдена.
 * При запросе статуса ф-ция формирования списка jsnStore->createActionList(); все портит.
 * При том, что вывод самого списка jsnStore->jsnActionList не влияет.
 *
 * В общем, пришли к главной фазе - отладка статусов.
 * Работаем.

   Проверено. Массив jsnStore->jsnObjArray - статусы экшенов меняются.

   ОБъект jsnStore->jsnActionList инициализируется как
    jsnActionList = {
        {"action_list", jsnObjArray} //jsnArray
    };

    Далее по ходу программы меняется на

    jsnActionList["action_list"] = "testing";

    Все работает.
    СЦУКО !!!!
    Забыл в цикле for добавить i++... и все проблемы из-за этого.
    И сколько раз уже на этот цикл смотрел...

    - Осталось понять, почему экшены start и reset не убираются из списка.[Разобрался, см. выше]
    - Как менять jsnHeadStatus, при разных экшенах
      (например "state" = "running" и "rc" = 0, если выполняется экшен) ?


 * //++++++++++++++++++++++++++++++++++++++++++++++++
 * 27.05.2022
 * СОздаем отдельное консольное приложение, где проверяем предачу данных о QJsonObject объекте
 * между классами. Далее принимаем решение о дальнейшем направлении кодинга в данном проекте.
 * Сделано. Приложение "transferjson". Показало, что все основные ф-ции обработки QJsonObject-ов работают
 * правильно.
 * Далее выяснилось, что в слоте Moving_Done_Slot() запускается ф-ция
 * JsonInfo::setActionDone(QJsonObject &theObj), которая меняет статусы theObj,
 * а потом опустошает массив JsonInfo::QJsonArray  jsnObjArray. А вот это и есть
 * ошибка, т.к. внутри MainProcess::ProcessAction при следующем экшене происходит
 * JsonInfo::jsnObjArray.replace(int index, QJsonObject object), а вот index-то уже и не
 * совпадает !
 *
 * Поэтому была переделана вся схема работы кода.
 * Теперь  в  MainProcess::ProcessAction вызывается ф-ция JsonInfo::setActionData(theObj),
 * которая по theObj определяет какой это экшен, находит его в массиве JsonInfo::jsnObjArray и
 * меняет его (экшена) значение на значение theObj внутри массива JsonInfo::jsnObjArray.
 *
 * И далее работаем только с JsonInfo::jsnObjArray, сам экшен нет нужды трогать.
 * Так убираем каждую из ф-ций SetJsnActionCollapse и т.д.
 * Так в дальнейшем будет удобнее менять все остальные статусы и RC json-объектов(экшенов)
 * Так сокращаем код в Moving_Done_Slot(), при этом сохраняем ф-цию
 * JsonInfo::setActionDone(QJsonObject &theObj), вот тут она как раз и нужна.
 * В общем, стало веселее.
 *
 * Теперь снова возвращаемся к потокам :)
 * Ставим вывод отладочных сообщений на всем пути потока, ибо пока непонятно, где происходит "отвал башки".
 * Помним про таймер в gui-client, можно попробовать использовать его для ловли блох.
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++
 * 26.05.2022
 *
 * Поскольку программа вылетает с сообщением "Segmentation fault (core dumped)"
 * То есть подозрение что дело не в tcp-обмене. И ошибка возникает на этапе подготовки данных к отправке.
 *
 * 1. Попробуем отключить отправку данных по tcp, будем только принимать команды и записывать в лог то,
 * что обычно отправляем по tcp.
 *  Да, проблема с доступом к json-объектам.
 *
 *  ЗАдача :
 *  написать ф-цию changeState - которая будет менять статус каждого экшена.
 *
 *
 *
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++
 * 25.05.2022
 * Снова проблема с потоками.
 * Если в момент, когда робот выполняет экшен, приходит новое tcp-подключение (запрос статуса) все виснет.
 * На одном хосте разные проги. Эта и запрос статсуов через gui-client.
 * ...
 * Похоже проблема в статусах. Т.е. команда collapse не меняет свой статус по заврешении и поэтому другие команды не принимаются...
 *
 * Action collapse is already running. Should wait it to finish
 *
 * В ф-ции MainProcess::Moving_Done_Slot() не срабатывает строчка
 * jsnStore->setActionDone(jsnStore->jsnActionCollapse);
 * jsnActionCollapse - не меняется содержимое.
 *
 * Поменял ф-цию :
 * void JsonInfo::setActionDone(QJsonObject theObj)
 * на
 * void JsonInfo::setActionDone(QJsonObject &theObj)
 *
 * Теперь строчка
 * jsnStore->setActionDone(jsnStore->jsnActionCollapse);
 *
 * РАБОТАЕТ.
 * Статусы обновляются как надо, все ОК.
 *
 * Возвращаемся к прооблеме с потоками.
 *
 * Прога вылетает, если во время выполнения экшена приходит команда на другой экшен...
 * В общем потоки...
 * В общем, логику надо подправить. Исключения будем добавлять.
 *
 * Для начала поправим записи connect.
 * Вместо
 *
 * ывап
 * сделаем
 * connect( this, &MainProcess::Open_Port_Signal, Robot, &HiWonder::Open_Port_Slot);
 *
 * Со стороны HiWonder :
 * QIODevice::write (QTcpSocket): device not open
 *
 * Т.е. где-то (клиент/сервер) происходит преждевременное закрытие сокета. Где ? [тут -> Moving_Done_Slot()]
 * По новому протоколу в ответ на запуск экшена нужно отправлять короткое json-сообщение.
 * {
    "info": "Request Accepted",
    "name": "collapse",
    "rc": 0
}
Вот оно и отправляется. А т.к. после каждого QTcpSocket::write у нас происходит закрытие сокета,
то данные больше нельзя записывать в сокет. А в ф-ции  Moving_Done_Slot() в тестовых целях надо было
проверить содержимое json-объекта и почему-то кроме вывода в лог еще и отправку в сокет поставил...

Задача :
- получать индекс в массиве jsnObjArray по имени theObj.value("name")

 * //++++++++++++++++++++++++++++++++++++++++++++++++
 * 23.05.2022
 * Невозможно изменить значения внутри JsonObject, если он берется из списка QList. Сам по себе - пожалуйста, а как элемент списка QList - нет.
 * Даже если все элементы QJsonObject перенеосим внутри класса в раздел public.
 * Т.е. пока у нас принципиально отсутствует возможность перебирать список экшенов и отбирать активные, т.е. выполняемые в данный момент.
 *
 * Имеет смысл попробовать следующее :
 * - создаем QJsonObject,  у которого value - это полный список инициализированных экшенов.
 * - Проверить, что можем менять значения value у экшенов через insert.[Проверено. Можно]
 *
 * - Если не можем, то точно сможем следующее :
 *   - как только меняется значение state экшена на "inprogress", делаем insert такого экшена в список action_list, если экшена там еще нет.
 *   - если экшен там уже есть, значит НЕ меняем "state", но меняем rc = -3. Код возврата поменяется с -3 на другой только если экшен закончился (выполнился) или
 *     была команда reset, которая переведет экшен в статус "waiting", rc = -4
 *
 *
 *
 * Есть проблема с потоками. tcp-команды не принимаются, пока выполняется текущая команда.
 * и после этого вообще висит.
 *  "2022-05-23__12:24:52:44 > 0xf00f: Current active command is status" и все, 0 реакции.
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 20.05.2022
 * Приступаем к написанию JsonInfo::getActionList()
 * Не отрабатывает  jsnStore->createActionList();
 * Работа со списком QJSonObject-ов превратилась в неразрешимую задачу...
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 19.05.2022
 * Подошел к написанию ф-ции JsonInfo::getActionList() - формирует список экшенов,
 * выполняемых в данный момент. Данный список отправляется по запросу status, см.
 * описание протокола.
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 18.05.2022
 * Поправил накопление данных в setActionDone(QJsonObject theObj).
 * ДОбавляем команды, отладка.
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 17.05.2022
 * Добавил json-обработку команд "status", "reset". Теперь примерно понятно, как дальше быть.
 * Т.е. команды движения типа "sit", "clamp" уже можно настраивать.
 * НЕ принял решение, как менять rc-экшена после завершения действий манипулятора.
 * Решение есть. Был уже глобальный QJSonObject mainjsnObj, в нем хранится текущий экшен.
 * Когда манипулятор завершает операцию этот mainjsnObj передается в класс jsoninfo как аргумент ф-ции.
 * И там внутри класса меняется его rc и прочие параметры.
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 16.05.2022
 * Инициализируем список из QJsonObject после инициализации каждого из из QJsonObject-элементов. Так работает.
 * Надо создать паблик метод для рекурсивного вызова merge_json(QJsonObject &src, QJsonObject &dst),
 * чтобы объединять все jsnAction-ы в один объект. Такая опция нужна для команды "reset". Так же можно будет выводить
 * в лог текущую сводку по всем экшенам, по сути - дубль переменной nlohmann::jsnStatus.
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 15.05.2022
 * Пишем процедуру инициализации json-объектов для каждого экшена.
 * Идем по списку экшенов "action_lst", инициализируем список json-объектов.
 * Проблема, что инициализировать список из элементов типа "struct" мы можем в цикле,
 * т.к. у структуры есть поля. А вот инициализировать - В ЦИКЛЕ - список из элементов
 * "nlohmann:json" - непонятно как. Поэтому пока инициализируем структуру, потом снова
 * в цикле  копируем данные из структуры в json-объект.
 * ...
 * Пробуем инициализировать список из QJsonObject через QJsonObject Class

 * //++++++++++++++++++++++++++++++++++++++++++++
 * 11.05.2022
 *
 * Меняем значение environment variable "EXTLIBS"
 * Текущее значение EXTLIBS="/home/ubuntu/extlibs/json/single_include" не устраивает тем, что создано под конкретную библиотеку nlohmann.
 * А ведь задумано было, что $EXTLIBS - общий путь к внешним библиотекам и в коде нужно конкретизировать путь до нужной библиотеки.
 * В общем нужно такое значение : EXTLIBS="/home/ubuntu/extlibs.
 * Смена значения EXTLIBS в ~/.profile - НЕ ПОМОГЛА !!! Даже после перезагрузки компа !
 * В /etc/profile нет настроек для EXTLIBS.
 * Осталась старая настройка в ~/.bashrc значит она решающая ???
 *
 * По поводу файлов ~/.bashrc, ~/.bash_profile, and ~/.bash_login в мануале сказано следующее :
 * Shell config files such as ~/.bashrc, ~/.bash_profile, and ~/.bash_login are often suggested for setting environment variables.
 * While this may work on Bash shells for programs started from the shell, variables set in those files are not available by default
 * to programs started from the graphical environment in a desktop session.
 * Ну, будем знать. Удаляем из ~/.bashrc - и, наверное, достаточно было перелогиниться, т.е. закрыть/открыть терминал, но перегрузился и все стало как надо.
 *
 * Сохранил копии файлов qsimpleserver, qsocketthread, SocketClient с другим именем, чтобы они в гите были.
 * Т.к. сейчас они уже в библиотечной папке, которая внешняя для папки репозитория.
 * Надо будет потом вернуть библиотеку в репозиторий.
 *
 * Смена данных JSON в "шапке" происходит следующим образом :
 * - меняем данные в структуре JsonInfo::StatusAnswer
 * - Вызываем ф-цию jsnStore->struc_2_jsnObj() где Копируем данные структуры в ordered_json jsnStatus, да именно в конкретную переменную jsnStatus
 * - Вызываем ф-цию jsnStore->returnJsnStatus() где Берем данные из ordered_json jsnStatus и передаем в QJsonObject::jsnObj, да, конкретно из jsnStatus в конкретно jsnObj
 *
 * Далее, при выводе статуса (например, в ProcessAction),  берем QJsonObject::jsnObj из ф-ции QJsonObject JsonInfo::returnJsnStatus(),
 * и преобразуем QJsonObject->QJsonDocument->QString
 * ...
 * Что-то слишком сложно... Попробовать через сигнал/слот ? Внутри слота можно менять private-переменные.
 *
 * Сегодня уточнили все вопросы по протоколу.
 * Сразу после запуска программы, еще до запуска первого экшена ответ на статус со списком action_list будет выглядеть так :
{
    "info": "No Detection",
    "name": "HIWONDER",
    "rc": -5,
    "state": "NoDetection",
    "action_list" : []
}
 * Теперь еще вопрос с выводом.
 * В ordered_json все выводится в заданном порядке, а в QJsonObject - как повезет... Надо с этим разобраться.

 *
 *
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 09.05.2022
 * Работаем в праздники :) Одтохнули уже достаточно. Работа тянет :)
 * Решил спрятать все переменные класса jsoninfo из public в private. Лучше поздно чем никогда :)
 * А это тянет за собой переделку кода в части парсинга tcp-запроса от tcp-клиента.
 * Тогда переделываем по частям, как с mobman.
 * Перенес ф-цию MainProcess::Data_From_TcpClient_Slot(QString message) в отдельный файл "tcpParcing.cpp".
 * Для обработки команд добавил void MainProcess::ProcessAction(int indexOfCommand)
 * Сейчас ProcessAction обрабатывает только 2 кода : "status", "start".
 * В целом, понятно, как работать с json-данными и передавать их клиенту.
 * В итоге что имеем :
 *
 * При запуске программы инициализируются json-объекты, содержащие статусы, экшены и т.д
 * Далее запускается объединение разных json-объектов в один, и данные этого общего объекта выводятся.
 *
 * НА данный момент много избыточного кода. Потом сократим, когда будет полная картина с протоколом.
 *
 * Подключил файлы qsimpleserver.h/.cpp как библиотечные. ДЛя этого в .pro-файле добавил $$(EXTLIBS)/manipulator/qsimpleserver.h
 * Следует учесть, что при "вынимании" родного файла из проекта, он удаляется гитом с диска... но при этом каждый раз предлагает сохранить его с другим именем.
 * При этом файл библиотеки nlohmann подключается в проект просто через #include в проекте, без редактирования .pro-файла проекта.
 * Возможно в этом-то и вся стуть, что в nlohmann заголовки и код в одном файле .hpp (а такое возможно ?), а у меня в разных, т.е. .h и .cpp - соответственно.
 *
 * В общем, библиотеку nlohmann подключить намного удобнее, чем qsimpleserver. С этим следует разобраться. Пока будем так пользоваться.
 *
 * Подключил файлы qsocketthread.h/.cpp по аналогии. Однако, забыл в файле .pro поставить символ " \" - продожение списка source/headers и часа 1.5 разбирался,
 * почему же не подключаются файлы...
 *
 * Теперь встает проблема, что файлы библиотечные лежат в папке на уровень выше, чем папка репозитория, получается git их не будет отслеживать.
 * Надо будет со временем перенести внутрь репозитория.
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 06.05.2022
 * Смотрим пример объединения json объектов :  https://stackoverflow.com/questions/64665582/how-to-merge-two-qjsonobjects-in-qt
 * Написал ф-цю конвертации структуры в ordered_json::nlohmann-объект.
 * void JsonInfo::struc_2_json(ordered_json &jsn, const StatusAnswer &header)
 * Проверено, работает.
 * ОСталось добавлять объект, представляющий данные об экшене к объекту StatusAnswer.[Сделано]
 * Написана ф-ция merge_json(jsnObj2, jsnObj), которая возвращает строку - данные json-объекта, собранного из jsnObj2, jsnObj.
 * Ф-ция вызывается внутри слота makeJson_Answer_Slot.
 * Сигнал для этого слота пока не написан.
 * Однако при объединении json-объектов первым всегда почему-то идет список, т.е. объект получается такой :
 *
 * {
    "action_clamp": [
        {
            "info": "Open/Close clamper",
            "name": "clamp",
            "rc": 0,
            "state": "inprogress | done | fail"
        }
    ],
    "info": "OK",
    "name": "HIWONDER",
    "rc": 0,
    "state": "Waiting"
}

 * Что портит весь вид. Местами объекты менял (т.е. кто с кем объединяется) все равно такой вид.
 *
 * Будем дуамать, как это победить.
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 05.05.2022
 * Продолжаем дорабатывать парсинг JSON.
 * Удобно использовать оба подхода - через библиотеку nlohmann,
 * Данные, ранее подготовленные для nlohmann конвертируем по цепочке nlohmann->std::string->QString->QJsonDocument->QJsonObject
 * Далее направляем данные QJsonObject в ранее написанную ф-цию traversJson(QJsonObject json_obj), которая рекурсивно проходит по всей структуре объекта JSON и пишет в лог
 * полученную структуру json-объекта. [Сделано]
 * Однако треубется доработать ф-цию traversJson(), т.к. она была написана под конкретный случай, где идет поиск конкретного ключа.
 * Теперь желательно, чтобы эта ф-ция делала полный парсинг JSON бъекта.[Сделано]
 * Сейчас, если внутри JSON-объекта попадается список (QJsonArray), то он не парсится, а надо.[СДЕЛАНО]
 * Следующая задача : готовить JSON-ответ супервизору, как набор json-объектов, которые объединяем вместе в один объект и отправляем клиенту.
 * Или, более конкретно, задача следующая :
 *
 * - json-Объект(ключ:список) nlohmann::jsnStatus парсить по значению экшена, т.е. "start", "clamp" и т.д., и по этому значению вытаскивать весь объект(элемент списка).
 *   Применяем цепочку nlohmann->std::string->QString->QJsonDocument->QJsonObject и далее работаем с QJsonObject штатными средствами Qt.
 * - Добавлять полученный объект в структуру StatusAnswer, так получаем готовый ответ tcp-клиенту
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 03.05.2022
 * Создан класс JsonInfo для формирования JSON-ответов tcp-клиенту.
 * Сознательно не стал добавлять работу с JSON в класс HiWonder, т.к. в будущем планируется сделать эти классы библиотечными и максимально избежать дублирования
 * кода при переходе от одного робота к другому.
 * Пока буду использовать оба подхода - через библиотеку nlohmann и через стандартный набор Qt -  QJsonDocument, QJsonObject и др.
 * Посмотрим, как удобнее будет.
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 29.04.2022
 * Вернул в рабочее состояние на 27.04.2022, убрав лишний вывод данных от ком-порта. Функционал не пострадал.
 * В итоге добавилось 3 команды.
 * Следующий шаг - добавляем статусы и json.
 * А пока Фиксим.
 *
 * + Добавляем команду "put_box"
 * + Добавляем JSON-вывод полной инфы по девайсу
 * + Добавил команду "info" которая выводит сводный список экшенов
 * + Добавил парсинг команд из списка QList<QString>  strcommand = { "/run?cmd=", "/service?name=", "/status", "/status?action="};
 * - Однако надо перенести парсинг tcp-данных из класса QSocketThread в MainProcess. Пусть QSocketThread отвечает только за обмен по TCP/ip тогда его легче будет выделить в отдельный
 *   файл, как библиотечный.
 * - Надо создать аналог GUI-клиента, сделанного для mobman
 * - Проверить с помощью GUI-клиента устойчивость работы при tcp-запросах с частотой 2Гц.
 *
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 28.04.2022
 * Долго разбирался с подключением библиотеки.
 * Казалось бы самый просто путь - использовать EXTLIBS="/home/ubuntu/extlibs/json/single_include"
 * Но сейчас путь привязан к библиотеке json, а надо его сделать на 2 уровня выше...
 * Но, если этот путь исправить, то придется вносить изменения в проект mobman, пересобирать его, тестировать, в общем, пока отложили.
 *
 * ОБнаружилась проблема в прошивке arduino. Почему-то перестал открываться хват. Начал разбираться в коде.
 * Делал вывод в ком-порт промежуточных данных, снимал комменты с отладочных строчек.
 * Изменил размер TX-буфера в файле
 * /home/ubuntu/arduino-1.8.16/hardware/arduino/avr/cores/arduino/HardwareSerial.h
 * При компиляции моя среда Arduino 1.8.16 выдавала разные значения свободной памяти при смене размера TX-буфера, т.е. настройки применялись.
 * Однако в выводе лога все равно строка обрезалась на 32 символах. И это проблема в Arduino, т.к. перед чтеним ком-порт выводил, а сколько там данных есть в буфере.
 * Так вот, всегда не более 32.
 * Имеет смысл настроить вывод из ком-порта также в отдельный файл.
 *
 * + Добавлена tcp-команда "getservos" - передать значения углов сервоприводов
 * + Добавлена tcp-команда "lock" - сжать захвт
 * + Добавлена tcp-команда "unlock" - открыть захват
 *
 * В какой-то момент все заработало :) Но непонятно, что повлияло :(
 * Оставляю разбор на завтра. Будем обратно ставить комменты и смотреть, когда перестанет правильно работать :)
 * Фиксируем.

 * //+++++++++++++++++++++++++++++++++++++++++++
 * 27.04.2022
 * Для новой презентации меняем порядок действий для HiWonder.
 * - По команде "start" :
 *          сменить статус на "running",
 *          взять кубик, встать в исходное положение,
 *          сменить статус на "done"
 * - По команде "put_cube" :
 *          сменить статус на "running"
 *          положить кубик, встать в исходное положение,
 *          сменить статус на "done"
 *
 * + Нужно добавить чтение значений сервоприводов из Arduino [сделано 28.04.2022]
 * + Добавить команду "lock", т.е. сжать захват.  [сделано 28.04.2022]
 * + Добавить команду "unlock", т.е. открыть захват.  [сделано 28.04.2022]
 * - Доработать отправку/чтение данных в/из Serial Port по 32 байта (размер буфера Arduino по умолчанию).
 *
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 26.04.2022
 * Удалены следующие лишние ф-ции из класса mainprocess :
 * - Data_From_Web_SLot(QString message);
 * - newConnection_Slot();
 * - server_New_Connect_Slot();
 * - on_openButton_clicked();
 * - on_closeButton_clicked();
 * - on_getXYButton_clicked();
 * - Return_EL_Slot(float EL);
 * - Return_FW_Kinematic_XYZ_Slot(int X, int Y, int Z, float EL);
 * - Pass_String_Slot(QString str);
 * - on_submitButton_clicked();
 *
 * Теперь весь код MainProcess.cpp - 398 строк.
 *
 * Далее пробуем выстроить некую иерархию класса.
 * Класс SerialDevice - аналог HiWonder, но без привязки к имени собственному. ОБъекты, например, - Nano, Mega - по названию плат.
 * Класс Robot (Manipulator) - Дочерний от SerialDevice. Вот тут уже свойства робота можно описывать - DOF, parcsel_size и т.д. Объекты - HiWonder, Mobman, Pinkman  - по названию робота.
 * Далее к объекту класса Robot надо прикручивать связь с другими роботами - создавать объект класса QTcpServer, работа с потоками, парсинг JSON и т.д.
 * Вот и подошли к потокам. Сейчас, выходит, что отправка данных в SerialPort выполняется в том же потоке, что и обмен по tcp/ip.
 * В случае с роботом HiWonder можно попробовать выделить отдельный поток для SerialPort-обмена, т.к. вся прога идет на стационарном ПК.
 * Вот когда это перейдет на mobman - там посмотрим.
 * Фиксируем.
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 25.04.2022
 * Различия начинают сказываться уже хотя бы в том, что у hiwonder - размер посылки в arduino - 8 байт, а у mobman - 6.
 * Число DOF для hiwonder - 6, а для mobman - 4.
 * Также добавляется код из-за использования протокола json, много новых переменных, и т.д. и для случая, когда просто робот управляется по tcp/ip это все лишнее.
 * Для начала удаляем ф-ции, используемые в GUI-версии и не нужные в данной, console-версии.
 * Удалено :
 * - void on_stand_upButton_clicked(); - Выполняется по команде через tcp/ip
 * - void on_sitButton_clicked();
 * - on_set_posButton_clicked();
 *
 * Добавляем важную фичу - перебор USB-Serial портов. Она есть в коде для mobman.
 * Для этого также придется отредактировать ф-цию HiWonder::Open_Port_Slot(QString portname)
 * А вот если бы эта ф-ция была добавлена в базовый класс, то сейчас не пришлось бы возиться с кодом...
 * А для этого надо продумать хранение, чтобы проекты mobman и hiwonder брали базовый класс в одном месте...
 *
 * Также на до проект threads переименовать в hiwonder-gui
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 13.04.2022
 * Пришла пора добавлять протокол взаимодействия роботов через ЦУП.
 * И встал вопрос наследования и масштабирования. А вопрос встал, потому что приходится дублировать код для HiWonder и Mobman.
 * Т.е. надо выделить отдельный код для работы с манипулятором - стационарным, либо мобильным.
 * Т.е. стоит подумать о создании нового класса типа RobotTCP, в котором вся обработка TCP-сообщений.
 * Тогда класс HiWonder - только для стационарного манипулятора.
 * класс Mobman - наследник HiWonder с добавлением функционала работы с CVDevice
 * класс RobotTCP - обработка TCP-сообщений.
 *
 * ДЛя начала попробуем просто выделить код в отдельные ф-ции и файлы. А для этго его надо унифицировать.
 * Чтобы в обоих проектах структура кода была одинаковая.
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 04.04.2022
 * Удалили c remote ветку
 * - mobman-json
 * - mobman
 * - embedded
 * - demo
 * - teseting-threads
 * Локально тоже это все почистили.
 * Фиксим.
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 31.03.2022
 * Заменены файлы SocketClient.cpp/.h
 * Взяты тут
 * /home/ubuntu/agents_python/hiwounder/app/utils/test_client
 *
 * Теперь вместо картинки с камеры на весь экран заходим на
 * http://192.168.1.175:5001/
 * и там маленькая картинка.
 * Поменяли путь к файлу сокета. Теперь только на 1 уровень вверх.
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 14.03.2022
 * Поработал с git-ом, теперь в ветке hiwonder актуальный код проекта "gui-client" для работы с Ручником.
 * Теперь намного проще тестировать одновременную работу HiWonder и mobman.
 *
 * Подходим к ситуации, когда надо писать свой аналог ЦУП для базовых тестов соместной работы роботов.
 *
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 10.03.2022
 * Взял проект embedded, добавил БЗ из проекта threads для розовой полочки.
 * Преобразовал команду "start" к виду :
 * - Взять кубик с розовой полочки
 * - В исходное положение
 * - Положить кубик на крестик в линолеуме, там мобман его возьмет
 * - В исходное положение.
 *
 * Но сейчас это все без всяких статусов, экшенов и json.
 *
 *
 *
 *
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++
 * Ветка embedded
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * Путь к Supervisor-у
 * file:///home/ubuntu/iqr_lit/supervisor/index.html
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++
 * 26.10.2021
 * Крайняя запись в ветке embedded для стационарного робота. Начинаем разработку для мобильного робота.
 * Есть мысль не использовать ф-цию send_Data(), а каждый раз формировать массив на отправку роботу внутри on_trainButton_clicked().
 * Так видно, что отправляешь роботу.
 *
 * //+++++++++++++++++++++++
 * 20.10.2021
 * Поменял имена сигналов/слотов для обмена по TCP, в обе стороны, т.е.
 *
 * и "Сверху вниз", MainProcess--->QTcpServer--->QTcpSocket
 * и "Снизу вверх", QTcpSocket--->QTcpServer--->MainProcess
 *
 * //+++++++++++++++++++++++
 * Перешли на ветку embedded
 *
 * 19.10.2021
 * Тестировал работу консольного варианта управления роботом.
 * Выяснилось(прошел по всем сигналам-слотам снизу вверх), что "старый" вариант соединения сигналов со слотом тут, в консольном
 * варианте, не работает.
 * Те. не работает строка
 *
 * connect(&server, SIGNAL(Info_2_Log_Signal(QString)), this, SLOT(Info_2_Log_Slot(QString)));
 *
 * Поменял на "новый" стиль :
 *
 * connect(&server, &QSimpleServer::Info_2_Log_Signal, this, &MainProcess::Info_2_Log_Slot);
 *
 * И ЗАРАБОТАЛО !!!
 *
 * Фиксируем результат.
 *
 *
 * //+++++++++++++++++++++
 * 18.10.2021
 * После совещания отказались от манипулятора на одноплатном ПК.
 * Поэтому проект переводим в консольное приложение, взаимодействующее с ЦУП.
 * Теперь все через ЦУП, без GUI
 * //+++++++++++++++++++++
 * 15.10.2021
 * Чистим код перед заливкой проекта на одноплатный компьютер.
 * НА транспортере тоже будет стоять манипулятор и управляться данной прогой.
 *
 * Т.к. не будет GUI и все управление по TCP, то, на данном этапе, каждую кнопку заменим отдельной командой, передаваемой
 * по ТСР. Т.е ТСР-парсер теперь - основой центр управления.
 * Ветка embedded- только проект embed, а проект threads - не трогать, оставить как в threads/release
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 07.10.2021
 * Если настроить  QTcpServer на слушание любого ipv4 адреса, т.е. listen(QHostAddress::AnyIPv4, tcpport))
 * То QTcpServer::serverAddress() выдает "0.0.0.0".
 * А если настроить на специфичный адрес, то только его и будет слушать, т.е. даже localhost может не отзываться.
 *
 * Координаты вечером 692, 573
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 06.10.2021
 * Начал создавать Блок-схему работы ПО для манипулятора.
 * Файл threads/Software_Scheme_HiWionder.odg
 * Наиболее интересно - организация работы
 * И, соответственно, передача информации "Сверху вниз", т.е. MainWindow--->QTcpServer--->QTcpSocket
 * и "Снизу вверх",                                      т.е. QTcpSocket--->QTcpServer--->MainWindow
 *
 * Добавил кнопку "PUT", которая повторяет операцию "положить кубик на тележку", чтобы кубик точнее попадал в рабочую
 * область.
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 05.10.2021
 * Со 2-й попытки получилась база, при которой манипулятор хорошо (все же не всегда) берет кубик.
 * Фиксиурем.
 * Также сформировал полноценную строку ответа серверу в формате json, но так Robot Control Center(RCC), написанный Мишей, не понимает ответ и считает,
 * что нет связи с роботом.
 * Пришлось вернуть обратно. Сейчас все работает.
 * Еще разве доработать движение манипулятора при установке кубика на тележку, т.е. чтобы приводы сначала двигались так :
 * 6
 * 3,4,5
 * 1
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 04.10.2021
 * 1140, 499 - координаты на момент ухода домой в конце дня. [Утром было 1140, 510]
 * Посмотрим, какие будут завтра с утра.
 *
 * ДОбавил кнопку  "Fix Data", которая записывает в файл точек координаты точки с камеры и позиции приводов.
 * Так намного быстрее протекает набор точек.
 *
 * Набрал за сегодня 130 точек. (час примерно добавлял кнопку со всем ф-ционалом, + в 12.00 собрание было.)
 * Реально было пол дня рабочих.
 *
 * ЗА выходные Миша прикрепил камеру на новый кронштейн (для подоконников из Стройера), подогнал крестики к шурупам по новой,
 * поэтому была набрана новая база точек.
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 30.09.2021
 *
 * Камера детектит кубик и на тележке 2230, 490
 *
 * Добавил слоты для сигналов quit(), deleteLater() для объекта класса QThread (было только для QSocketThread* tcpthread)
 * Теперь их Аж 3.
 *
 *  connect(tcpthread, SIGNAL(finished()), thread_A, SLOT(quit()));
 *  connect(tcpthread, SIGNAL(finished()), tcpthread, SLOT(deleteLater()));
 *  connect(thread_A, SIGNAL(finished()), thread_A, SLOT(deleteLater()));
 *
 *  И ушла ошибка
 *  (threads:15185): GLib-ERROR **: Creating pipes for GWakeup: Too many open files
 *
 * //++++++++++++++++++++++++++
 *
 *  29.09.2021
 *  Фиксируем результат.
 *  Сработала передача данных(команда) из tcp сокета наверх роботу.
 *  ТАкже сработал обратный ответ статуса от робота вниз в сокет.
 *  Вводим переменную HiWonder.current_staus, которая хранит значение статуса.
 *  В итоге у нас работает сквозной обмен данными из tcp-сокета "наверх" к роботу и обратно от робота "вниз" в tcp-сокет.
 *
 *  Теперь можно настраивать протокол обмена и делать этот обмен скоростным.
 *
 *  Решил проблему со сменой статуса. В ф-ции try_mcinfer была отправка команды роботу через on_set_position_clicked(), сама
 *  ф-ция - это слот нажатия кнопки, т.е., понятно, что это однрократная команда, поэтому там в конце стоял флаг, что это последняя
 *  команда, поэтому статус сразу done. Заменил на стандартный вариант  - send_data(lastcommand_value) - как в обработчике команды из tcp-сокета.
 *  Доработал смену статуса. Теперь робот воспринимает (и правильно реагирует) на следующие команды :
 *
 *  status
 *  reset
 *  start
 *
 * Уже пора составлять диаграмму-схему работы программы. Переключился на работу с классом TcpServer и через пару недель уже не помню какие
 * сигналы/слоты отвечают за работу с сериал-портом. Хотя пока еще искать по коду быстрее, чем искать схему :)
 *
 * Начнем готовить ответ в формате json. Пока заведем отдельную ф-цю в которой будем формировать строку json на основе
 * переменных.
 *
 *

 * //++++++++++
 * 25.09.2021
 *
 * Заработало!!!!!
 *
 * В общем, команду QTcpServer::listen(host, port) нужно задавать из класса MainWindow, а не из QSimpleServer...
 * //++++++++++++
 * 24.09.2021
 * Приступаем к парсингу GET запроса, который получает наш webserver извне, извлекаем команду для робота, передаем её роботу.
 *
 * В примере, на основе которого собран класс webserver считывание из сокета и отправка в сокет данных выполняется в одном бесконечном цикле.
 * Пришла пора разделить считывание данных из и отправка в на 2 разные ф-ции.
 * Слот Output_Data_From_Client_Slot остается только на СЧИТЫВАНИЕ.
 * Добавляем слот на запись. Слот срабаывает, когда робот подает сигнал, что команду завершил.
 * Такой слот уже есть - Moving_Done_Slot(), срабатываен по сигналу от робота Moving_Done_Signal()
 *
 * Надо вводит флаг, что был GET-запрос, чтобы данные отправлялись в сокет только есть есть запрос.
 *
 * Проблема с первой же строчкой кода, которая должна крутиться в потоке :
 *
 * client_fd = accept(this->sock, (struct sockaddr *) &cli_addr, &sin_len);
 * Если убрать код, работающий с сокетом, то поток работает нормально.
 * Оставлю такой вариант кода для дестов. Поместил его в отдельную ветку testing-threads
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 23.09.2021
 * А теперь со всей этой ... мы попытаемся взлететь :)
 * Протупил с выводом в лог из конструктора.
 *
 * Скорее всего в лог омжно выводить только после завершения конструктора, т.е. когда объект класса уже создан.
 * 14:55
 * Криво-косо, но данные GET-запроса получаем,
 *
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++
 * 22.09.2021
 *
 * Создаем отдельный поток для web-сервера.
 * Создал отдельную ветку - threads с этим функционалом.
 *
 *
 *
 * //+++++++++++++++++++++++++++++
 * 21.09.2021
 * Вводим дополнительный байт, - "Последняя команда" ДА/НЕТ.
 * Процессор, увидев что это последняя команда, вместо обычного
 * "Robot movement DONE! Total!!" ответит :
 * "Robot movement DONE! LAST !!"
 * По строке "LAST" ПК поймет, что это была последняя команда и
 * тогда можно будет посылать сообщение web-серверу.
 *
 * Пришлось сдублировать ф-цию Set_Position, clamp, stand_up,
 * чтобы сохранился прежний функционал работы через кнопки.
 * В них добавляется 8-й байт, - индикатор "Последняя команда" ДА/НЕТ.
 * ДОбавил пару сигнал/слот Moving_Done_Signal/Moving_Done_Slot
 * где обрабатывается событие завершения последней команды.
 * В слот, соответственно, надо добавть сообщение веб-серверу, что
 * команду выполнил (кубик опустил на тележку)
 *
 * [Сделано]
 *
 * //++++++++++++++++++++++++++++++
 * 20/09/2021
 *
 * По итогам совещания было принято решение :
 * 1. Для успешной презентации подготовить вариант 0 (ноль), который заключается в следующем :
 *
 * Создается Диспетчер (ответственный Семашко), управляющий роботами через http.
 * Соответственно, в текущую программу управления роботом необходимо добавить http-сервер, скелет С. Семашко уже прислал.
 *
 * В этой связи далее весь проект отпочковывается в новую ветку - demo, - где будет добавлена вышеуказанная фича.
 *
 * 2. Также, заодно следует добавить автоматическое, т.е. без нажатия кнопки, открытие serial-порта с именем ttyUSB0. Для
 * демонстрации так будет нагляднее (т.е. человек нажимает минимум кнопок для запуска взаимодействия роботов)
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++
 * 17/09/2021
 *
 * Зафиксировал размеры посылок в коде arduino:
 * - "Туда", т.е. от ПК к роботу - это 7 байт
 * - "Обратно", т.е от робота к ПК - это 32 байта.
 * Теперь даже если полезных символов в сообщении меньше 32, то в буффер уже на вставляются "посторонние" символы из последующих посылок,
 * т.к. оставшиеся байты уже проинициализированы в самом начале, т.е. нет места для "посторонних"

 * Задача :
 * В arduino проекте разместить код Go_To_Position() внутри parse_command (), чтобы не происходит перескок потока. И тогда, возможно,
 * сообщения в лог будут поступать последовательно.
 *
 * //+++++++++++++++++++++++++++++++++++++++++
 * 16.09.2021
 * Есть недоработка.
 * Если захват изображения не сработал NOT DETECTED
 *
 * Максимальная длина строки от робота имеет вид :
 * 110, 193, 129, 147, 140, 134, 49
 * Это как раз 32 байта - максимальный размер буфера TX arduino.
 * Вот поэтому один символ переходит в следующую отправку !!!
 *
 * Если в посылке от робота к ПК меньше 32 байт, то робот добавляет недостающее до 32 количество байт из последующей посылки... !!!
 * Это полное безобразие !!!
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 15.09.2021
 * Встала задача сделать комплексное движение для робота :
 * - взять кубить, встать в исходное положение, положить кубик в другую область рабочего пространства робота.
 * Такой режим работы подразумевает отправку нескольких команд роботу, причем сначала должна отработать текущая команда,
 * и после этого отправляется следующая.
 * Как ПК узнает, что робот завершил текущую команду ?
 * Для этого после завершения движения всех приводов робот отправляет в ПК сообщение "Robot movevent DONE!"
 * ПК, получив такое сообщение от робота, отправляет следующую команду роботу.
 * Однако ранее робот был запрограммирован так, что получив сообщение от ПК он (робот) сразу же отправляет сообщение в ПК о полученных данных,
 * т.е. сообщает в пк, - "ДА, я получил вот такие данные". Т.е. получив 1 команду от ПК робот отправляет 2 сообщения :
 * 1 - "ДА, я получил вот такие данные"
 * 2 - "Robot movevent DONE!"
 *
 * Значит теперь, ПК, чтобы отправлять следующую команду, должен различать ответ "Robot movevent DONE!", т.е. нужно делать парсинг ответа от ПК.
 * Это сделано. ПК ищет в ответе строку "DONE!" и, найда такую, отправляет следующее сообщение.
 *
 * Как задумано это реализовать ?
 * QSerialPort имеет сигнал readyRead() (не путать с ф-цией waitForReadyRead()), означаютщий, что в RX-буфере ПК
 * есть несчитанные данные, т.е. пришла новая порция оных.
 * В класс HiWonder был добавлен обработчик сигнала readyRead() по имени ReadFromSerial_Slot();
 * Этот обработчик считывает данные из ком-порта, парсит в поисках строки "DONE!", и, если находит, то выставляет флаг MOVEMENT_DONE = true.
 * А слот, выполняющий комплексное движение робота, - MainWindow::on_trainButton_clicked(), - ждет пока этот флаг не будет установлен,
 * и после этого переходит к отправке следующей команды роботу.
 *
 * Так вот в реале прога зависает на этапе ожидания флага, т.е. вот на этой строчке
 * while (!Robot->MOVEMENT_DONE) {;}
 *
 * Как в итоге сделано ?
 *
 * Убрано ожидание установки флага.
 * Теперь все держится на том, что есть слот ReadFromSerial_Slot(), по которому данные считываются из буфера, но также важная ф-ция этого слота -
 * он блокирует остальные обращения к ком-порту, пока слот не будет обработан.
 * Благодаря этому следующая команда посылается в ком-порт только после обработки слота, однако, тогда это будет уже после первого же
 * подтверджения от робота "From Robot :", но этого не просиходит. И скорее всего, потому, что внутри робота еще не завершена ф-ция "parse_conmmand",
 * которая в этот момент запустила приводы в движение. И вот когда приводы закончат свое движение, ф-ция parse_command запустится в новом цикле,
 * и тогда только считает новую порцию из ком-порта, запустит приводы во вновь прибывшие координаты.
 *
 * "Robot movement DONE!" vs "Robot movement DONE! at all"
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 14.09.2021
 * Нарисовалась проблема с типом const char.
 *
 * Мы делаем отправку данных из ПК к роботу через ф-цию
 *
 * qint64 write(const char *data, qint64 len);
 *
 * Так вот при значениях угла более 127 уже не попадаем в char (который со знаком, т.е. signed char)
 *
 * сохранить ардуино файл !!!
 *
 * Пришлось руками редактировать Quiodevice.h
 *
 *
 * ##############################################
 * 10.09.2021
 * Если в поле для кнопки submit введено менее 6 параметров, разделенных запятой, то
 * по нажатию submit, в таком случае, меняется цвет фона кнопки и строки ввода.
 * В лог пишется соответствующее сообщение.
 * Программа уже не вылетает.
 * После ввода правильного количества параметров (пока 6) цвета меняются на значения по умолчанию, т.е.
 * рабочее окно программы приводится к исходному виду.
 * ##############################################
 * 03.09.2021
 * Если в строке для кнопки "Submit" вставить не 6 параметров, а 5, то прога тупо вылетает...
 * Это нужно исправить. [Сделано 10.09.2021, см. выше]
 * ##############################################
 * 02.09.2021
 * В позиции 319, 850, 0, 93, 80, 0, 130, 162 (т.е. угол 5 привода уже 130 и далеко от основания, по Y,- 850)
 * хват робота задевает кубик. ТОгда нужно двигать приводы в следующем порядке :
 * 6,5,1,4,3
 *
 * А в позиции 244, 820
 * хват поднимается недостаточно и при закрытии хватает кубик и тащит его наверх.
 * Значит при обратном движении надо сначала двигать приводы 3,4,1,5,6
 *
 * Решение :
 * Для позиций у дальнего края надо вводить поправку.
 * Если серво 5 >=130, то в ф-ции Go_To_Position(byte *pos) (код процессора) выполняется другая последовательность
 * движения приводов : 5,6,1,4,3 - так работает.
 * #################################################################################
 * 02.07.2021
 * Запустил упрощенный алгоритм FWK (Forward Kinematics).
 * Ф-ция FW_Kinemaic_Slot_02.
 * Также добавил отправку строки для записи в лог из класса RMath в MainWindow через сигнал слот Pass_StringSignal/Slot
 * соответственно.
 * Длина проекции робота на плоскость пола теперь 214.428mm ,:)
 * Будем думать, а пока фиксим на git.
 *
 * #######################################################
 * 01.07.2021
 *
 * Сделал проверку вычисления прямой кинематичесой задачи.
 * По нажатию кнопки "Get XY" внутри соответствующего слота запускаем сигнал
 * FW_Kinemaic_Signal(int S3, int S4, int S5, int l1, int l2, int l3)
 * На это внутри класса Robo_Math срабатывает слот FW_Kinemaic_Slot(int S3, int S4, int S5, int l1, int l2, int l3),
 * Который проводит вычисления x,y,z для всех 3 звеньев l1, l2, l3. И полученные конечные значения x3, y3, z3 отправляет
 * сигналом Return_FW_Kinematic_XYZ_Signal(x3, y3, z3) в класс MainWindow, где соответствующий слот
 * Return_FW_Kinematic_XYZ_Slot(int X, int Y, int Z) записывает эти данные в лог.
 *     Код работает, но в расчетах где-то ошибка, врет сильно.
 * Учёл, что координаты кубика начинаются с дальнего от робота угла. Т.к. при движении кубика от робота
 * координата Y будет уменьшаться. А координата x должна быть почти неизменной, т.к. снимали точки при движении вдоль оси Y.
 *
 * Тогда ключевой индикатор сейчас - координата Y, а она-то как раз близко : 362 вместо 356 !
 * Длина проекции робота на плоскость пола должна быть 230, а вычисляется 208.659
 * dl1 = 119,08
 * Экспериментальным путем увидел, что при угле 0 на приводе 4. Угол между 4 и 5 звеньями не 90, а заметно больше.
 * ДОбавил к вычислениям с углом привода 4 поправку в 15 градусов.
 * Измеренеия проекции на пол делал от центра основания, а надо от центра привода 5.
 * Т.е. в итоге длина короче на 12-15мм.
 * Сделал поправку в 7,150. Так же учел, что координаты x,y получем в пикселях, а при вычислениях используем мм.
 * Потом делаем обратное преобразование из мм. в пиксели.
 * НА участке в 200мм координата Y изменяется на 675 пикселей, значит на 1мм приходится 3.375 пикселя.
 * Сейчас после "подгонки" через поправку в 7.15 градусов значение проекции уже 227.351 - близко к 230.
 * Также понял, что можно по-другому считать все углы, и это упрощает общий алгоритм, и уменьшает число операций.
 * Поэтому пока фиксируем, а потом будем упрощать :)
 *
 * commit 64cddd400699a26d8a187d5d6c9839b68f25590f
 *
 *
 * #################################
 * 29.06.2021
 *
 * Создал класс class Robo_Math : public QObject
 * Для вычисления значений углов через длину робота -> через значения XY центра кубика и угол поворота привода 6.
 * Т.е. это дополнительный к нейронке способ вычисления.
 *
 * Жмем кнопку "Get XY", в соответствующем слоте получаем XY от simpledetector.
 * И передаем значения XY через сигнал Pass_XY_Signal в класс Robo_Math
 * В классе Robo_Math этот сигнал обрабатывается слотом Pass_XY_Slot.
 * Этот слот по значениям XY вычисляет длину EL(float) и передает значение EL обратно в класс MainWindow.
 * Передача проиходит через сигнал Return_EL_Signal(EL), а в классе MainWindow обработку проводит
 * слот Return_EL_Slot(float EL), который выводит значение EL в лог.
 *
 * Проверил на тестовых данных - все работает, данные из класса в класс передаются.
 * Теперь возникает следующая задача.
 *
 * ДЛя простоты считаем, что кубик лежит прямо напротив робота и привод 6 не надо поворачивать.
 * Тогда проекция робота на пол в момент, кода он готов взять кубик, это есть некая длина L.
 * Эту длину легко вычислить, зная углы приводов 5,4,3 и длину соответствующих плечей робота.
 * А вот как сделать обратные вычисления - зная длину L из координат центра кубика X,Y - как вычислить значения 3 углов ?
 * Подбором-перебором ?
 * Такая задача называется Inverse Kinematics (IK) technology
 * ######################################
 * 25.06.2021
 * Т.к. теперь мы разделяем движение "туда" и "обратно" были внесены необходимые изменения
 * во все соответствующие ф-ции.
 *
 * 02.09.2021 А теперь подробнее.
 * Теперь в посылку для робота добавляется 7-й параметр, он же 9-й в строке "Training data..."
 * При движении "Туда", т.е. в исходную позицию это код 0x31 == 49(в десятичной системе)
 * А при движении "Обратно", т.е. это в исходную точку по кнопке "Stand UP" - это код 0x30 == 48 (в десятичной системе)
 * ######################################
 * 19.06.2021
 * Добавил ввод данных для приводов из строки - All_Servos_lineEdit
 * И вслед за этим обновление текста в QSpinBox-ах.
 *
 * ##############################
 * 14.06.2021
 *
 * Кажись заработало "Пробуем QSpinBox вместо QLineEdit"
 * Т.к. большой размер шрифта, то и общее окно программы пришлось увеличить.
 * Фиксируем.
 * #######################
 * 12.06.2021
 * Пробуем QSpinBox вместо QLineEdit
 *
 * servo_1_lineEdit
 * servo_1_spinBox
 * servo_2_spinBox
 * ...
 * Очень много провозился.
 * Оказывается, если в окне "Change StyleSheet" сначала поставить размер шрифта, то все остальные настройки не считываются.
 * Поэтому размер шрифта меняем в самом окне QtDesigner, а далее через "Change StyleSheet"
 * #######################################
 * 10.06.2021
 * Each QWidget has slots repaint() (forced immediate) and update() (queued deferred)
 *
 * Выяснилось, что процедуру захвата лучше разделить на 4 этапа
 * 1. Позиционируем из исходной, захват горизонтально над кубиком. Далее 6 привод не меняется.
 * 2. ОТкрываем захват
 * 3. Чуть опускаем (3,4,5 приводы)
 * 4. Делаем захват.
 *
 * Можно сразу вставать в п.3
 *
 * Дальний ряд (ДР) - (4,5) = (45,155)
 *
 * #############################################
 * 08.06.2021
 * Заменил тип массива Servos на usigned char.
 * Для этого пришлось переписать ф-цию отправки данных роботу - HiWonder::GoToPosition(QByteArray &position),
 * и ф-ции вывода данных на печать.
 * Поведение робота от этого не изменилось, значит точно нейронка неправильно работает.
 *
 * Последний рабочий коммит 765014e4ae86b918485ae40a31d33f283eb85016
 * Фиксируем.
 * коммит с этим фиксом  ba7a1effc39f82f9073fc5d3ce7494d81a074cd0
 * #############################################
 * 07.06.2021
 * Подключаем библиотеку mcinfer
 * ...
 * Заработало. НЕ путаем сборку debug  и release :)
 *
 * Порядок работы :
 * Подключаем веб камеру по usb
 *
 * Запускаем скрипт
 * ~/qt-projects/iqr_lit/simpledetector_cpp/.run.sh
 *
 * Запусаем hiwonder-qt
 * По нажатию кнопки "Read Detetor" ( слот "on_socketButton_clicked()" ) Происходит считывание данных из сокета  - это координаты x,y центра кубика.
 * Далее эти координаты передаются в нейронную сеть fuzzy_model на обработку и сеть возвращает положения углов
 * приводов 3-6 робота - это 4 числа в диапазоне 0 - 180.
 * Приводы 1-2 почти не используются - это захват и положение захвата в горизонтальной плоскоскти.
 *
 * ###################################################
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
 * commit 797ac2dac91d262a6c282a2163f0e8285f08c54f
 * #####################
 *
 * Теперь переносим работу компорта в класс HiWonder из класса MainWindow.
 * По нажатию кнопки генерим сигнал Open_Port_Signal, в ответ запускается слот Robot->Open_Port_Slot
 * Работа с ком-портом перенесена в класс HiWonder.
 * Доработана запись в лог. Еще нужно данные записывать в лог в виде строки.
 *
 * На очереди считывание из ком-порта.
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
