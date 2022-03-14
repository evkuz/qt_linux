#ifndef EVKUZ_H
#define EVKUZ_H

/* //++++++++++++++++++++++++++++++++++++
 * 06.02.022
 * Добавляем кнопки для команд в новом формате
 * //++++++++++++++++++++++++++++++++++++
 * 10.01.2022 start
 *
 * Ветка mobman-json (mobman_02)
 * Создаем GUI-клиент для тестирования http-обмена с манипулятором и с CV.
 * Вручную команды вводить уже надоело.
 *
 *
 * //++++++++++++++
 * 25-26.01.2022
 * - Делаем поиск по ключу "detected" - находится во вложенном объекте "data" : {}
 * - Получаем все пары ключ : значение, включая вложенные объекты.
 * - Добавил проверку на тип Array для элемента JSON-объекта.
 *
 * Можно добавлять полученный функционал в demo01. Т.е. в итоге используем встроенные классы Qt для работы с JSON
 *
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
 * Задача :
 * - Отправлять ответ от mobman на команду "get_box". Т.е. команду "get_box" теперь следует  посылать в виде [Поставлено в очередь.]
 *
 *
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++
 * 24.01.2022
 * Получаем ответ от камеры в виде JSON-строки.
 * Задача - работать с этими данными именно как с json-данными. Пока только как строки.
 * Есть пример https://forum.qt.io/topic/94898/get-json-value-from-api-response/5
 *
 * - Научился ловить все значения из json. Есть проблема с первым ключом во вложенном объекте, но это решим. [Решено. НАдо сначало к типу bool приводить, а потом boolvalue.toString]
 * Фиксируем.
 * - Научился ловить значение distance из весго ответа, можно пользоваться.
 *   НО... пока это работает т.к. заранее известны поля и их типы значений. НА универсальность претендовать не может.
 *   Т.е. парсинг запускается, но мы заранее ожидаем, что там есть вложенный объект (делаем проверку однако), а в этом объекте должен быть ключ "distance"(тут уже без проверок).
 *   Т.е. "по уму" надо удостовериться, что в списке ключей есть нужный нам, в данном случае - "distance".  Это задача на будущее, а пока фиксим.
 *
 * //+++++++++++++++++++++++++++++++++++++++++++++++++++
 * 20.01.2022
 * Добавил рекурсивный парсинг json, если есть вложенные json-объекты.
 * Используются QT-классы
 * - <QJsonDocument>
 * - <QJsonParseError>
 * - <QJsonObject>
 * - <QJsonValue>
 * - <QJsonArray>
 * Ф-ция traversJson(QJsonObject json_obj)
 * Будем добавлять либу https://github.com/nlohmann/json
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 18.01.2022
 * В протоколе взаимодействия прописано, что в ответ на action, устройство среди прочего в ответ посылает поле name- имя девайса. Так что
 * можно спокойно делать в одной ф-ции работу с tcpsocket.
 *
 * Задачи :
 * - qtcpsocket - решить проблему с чтением данных.[сделано]
 * - парсинг JSON ответа от CV, вывод значений полей в лог
 * - ответ на команды "get_box", "ready" и т.д. в формате json.
 *
 * С чтением данных проблема решена. Не надо на стороне клиента (здесь) закрывать соединение. Надо ждать, когда сигнал "disconnect" от сервера (отправитель) придет.
 * Т.е. я убрал socketDEV->disconnectFromHost();
 * Те, что остались в других ф-циях - для обратной совместимости. Эти ф-ции не используются, потом уберу их.
 * В таком случае осталась задача убрать из ответа устройства HTTP-заголовки. [Сделано частично - начало JSON-данных ищется по символу '{', все предыдущие данные отбрасываются]
 * В этой связи еще надо посмотреть класс QNetworkAccessManager, т.к. заявлено, что :
 * "One QNetworkAccessManager instance should be enough for the whole Qt application. Since QNetworkAccessManager is based on QObject, it can only be used from the thread it belongs to."
 * Пока тестирую JSON-парсинг на const строке. - СДелано. Получаю значения key - value

 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 17.01.2022
 * При увеличении числа команд, отправляемых по нажатию кнопки (на каждую команду своя кнопка) происходят однотипные действия :
 * - 1. Создать сокет (для камеры CV или для манипулятора - разные девайсы)
 * - 2. Соединиться с хостом
 * - 3. После успешного соединения отправить запрос в сокет
 * - 4. Считать данные (ответ) из сокета при появлении оных там.
 *
 * В этой связи нет смысла на каждую кнопку создавать свой отдельный сокет с одинаковым кодом.
 * У нас меняется только запрос. А ответ мы принимаем одинаково и направляем его в парсер ответов.
 * Тогда получаем следующую последовательность :
 * - Значит запрос формируем на шаге 1 - в слоте нажатия кнопки и храним в глобальной переменной (создаем строку, у нас же запрос - это GET запрос, т.е. строка),
 * и далее - шаг 2 - вызываем ф-цию создания сокета.
 * - Отправляем сформированную строку в сокет на шаге 3. Т.е. на этом шаге берем уже ранее сформированную строку из глобальной переменной.
 * - Ответ направляем в парсер. И вот парсеру придется совсем туго... Как он определить из какого устройства пришел ответ ???
 * Изменения предстоят структурные, т.е. фундаментальные и масштабные :)
 * Поэтому сначала фикс текущего состояния.
 *
 * Создал вышеозначенный функционал. В целом работает.
 * Надо написать отдельный слот из строчки
 * connect(socketDEV, SIGNAL(disconnected()), this, SLOT(CV_onDisconnected()),Qt::AutoConnection); [сделано, ф-ция socketDEV_onDisconnected_Slot()]
 *
 * - Теперь еще надо добавить ответ от манипулятора на запросы "ready", "parking", так, чтобы в логе было понятно т.е. что-то типа "parking DONE"
 * - Продумать как анализировать ответ от устройств. Когда каждый сокет создается как отдельный объект - понятно. А когда сокет один на двоих - непонятно.
 * - В общем, надо вспомнить, что там в описании протокола.
 *
 * //++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 11.01.2022
 * Промучился с QDataStream - не работает.
 *
 * Будем искать дальше варианты
 *
 * ОТкрываем сокет для манипулятора и отдельный сокет для CV. Код там почти повторяется...
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
#endif // EVKUZ_H
