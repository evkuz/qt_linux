#ifndef EVKUZ_H
#define EVKUZ_H


/*
 * Ветка mobman, разработка для мобильного робота (мобильный манипулятор)
 * За основу взят софт для стационарного манипулятора.
 *
 * https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
 * В к. 406Б использую kit : Qt 5.15.2 (gcc_64)
 * В к. 240Б использую       Qt 5.15.2 (raspi) Т.К. ЭТО проект для Raspberry Pi 4b
 *
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * Путь к Supervisor-у
 * file:///home/ubuntu/iqr_lit/supervisor/index.html
 *
 * //+++++++++++++++++++++++++++++++++
 * 28.11.2021
 * Добавил библиотеку json.hpp
 * Источник тут https://github.com/nlohmann/json
 * Чтобы на разных ПК иметь доступ к этой (и другим в последующем) библиотеке ввёл переменную окружения
 *  "EXTLIBS"
 * Прописал в *.pro файле, в разделе headers
 * ${EXTLIBS}/nlohman/json.hpp
 *
 * qt приняло настройку в /etc/profile и только после перезагрузки.
 *
 * И, скорее всего, потому, что настройка в файле выглядела так :
 * #INCLUDEPATH += $$(EXTLIBS)
 * INCLUDEPATH += $$system( echo $EXTLIBS )
 *
 * Т.е., получается, что локальная переменная не работает, но работает системная, а она прописана в /etc/profile и применяется только после
 * перезагрузки в Ubuntu, в отличие от обычной переменной окружения.
 *
 * Смена настроек на
 * INCLUDEPATH += $$(EXTLIBS)
 * #INCLUDEPATH += $$system( echo $EXTLIBS )
 *
 * Т.е. подключение переменной окружения, - не повлияло. Файл и так виден.
 * Также в .pro файле убран из раздсела HEADERS, однако добавлен как
 * #include "nlohmann/json.hpp"
 * После того как стал виден json.hpp также важно добавить строчку
 *
 * using json = nlohmann::json;
 *
 * Вот после этого виден тип "json"
 *
 * Теперь для чистоты эксперимента :
 * - НА другом ПК добавляем сначала переменную окружения пользователя ${EXTLIBS}.
 *   echo "export EXTLIBS="/path/to/lib" >> ~/.bashrc
 *   source ~/.bashrc
 *
 * - Пытаемся подключить #include "nlohmann/json.hpp"
 * И если не поможет, то меняем строчки в .pro файле на системные и т.д.
 *
 * //+++++++++++++++++++++++++++++++++
 * 25.11.2021
 * Для дальнейшей работы с камерой(openCV) надо отправлять ей http-запрос.
 * Получится запрос "себе" же. Софт, управляющий работой камеры обработает запрос, спросит камеру и также через http отправит ответ.
 * Т.е. теперь для получения данных от opencv не нужно создавать сокет (это делает софт работы с камерой), а только отправить http-запрос.
 *
 *
 *
 * //++++++++++++++++++++++++++++++++
 * 23.11.2021
 * Есть разница в том, какая команда приходит и какие значение отправляются в serial port.
 * Ниже строчки из лога.
 * 2021-11-23__18:52:39:493 > 0xf00f: From TCP Get new command : servos=45,20,45,165,0,0,125,222
 * 2021-11-23__18:52:39:494 > 0xf001: To Robot: 45, 20, 45, 165, 111, 0, 49, 200,
 * Ищем по ключевым строкам
 * 111 == 0x6F
 * 125 == 0x7D
 * 222 == 0xDE
 *
 *
 *
 * //++++++++++++++++++++++
 * 12.11.2021
 * Заработала команда по TCP servos=90,90,90,160,45,45,49,222&
 * Теперь можно управлять мобильным манипулятором через tcp.
 *
 *
 * //++++++++++++++++++
 * 10.11.2021
 * Оставляем размер посылки в 8 байт, иначе очень много переделок в arduino.
 *
 * 03.11.2021
 * Вот тут должна была быть запись, что этот проект на мобильном роботе на Raspberry Pi 4B и все сообщения выше относятся к мобильному роботу,
 * ветка mobman.
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++
 * 26.10.2021
 * Есть мысль не использовать ф-цию send_Data(), а каждый раз формировать массив на отправку роботу внутри on_trainButton_clicked().
 * Так видно, что отправляешь роботу.
 *
 * //+++++++++++++++++++++++
 * 20.10.2021
 * Поменял имена сигналов/слотов для обмена по TCP, в обе стороны, т.е.
 *
 * и "Сверху вниз", MainWindow--->QTcpServer--->QTcpSocket
 * и "Снизу вверх", QTcpSocket--->QTcpServer--->MainWindow
 *
 * //+++++++++++++++++++++++
 * 19.10.2021
 * Тестировал работу консольного варианта управления роботом.
 * Выяснилось(прошел по всем сигналам-слотам снизу вверх), что "старый" вариант записи connect (...) соединения сигналов со слотом тут, в консольном
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
