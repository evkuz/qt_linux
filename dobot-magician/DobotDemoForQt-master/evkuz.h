#ifndef EVKUZ_H
#define EVKUZ_H
/*
 *  17.07.2022
 *  Удалось скрестить проект DobotDemoForQt с какой-то из библиотек.
 *  Т.е. сам проект шёл без src/ и был собран под Windows.
 *  Взял исходники проекта, скачал с github несколько репозиториев для dobot Magician и в каком-то из них
 *  нашел подходящий src/
 *  И то, в исходниках проекта ф-ция подключения к Serial Port отличается от src/
 *
 *  В проекте : int ConnectDobot(0, 115200, fwType, version, &time)
 *  В    src/ : int ConnectDobot(const char *portName, uint32_t baudrate, char *fwType, char *version);
 *
 *  Т.е. в проекте ф-ция с 5 параметрами, а в src - c 4-мя - без парамтера &time.
 *
 *  Аналогично ф-ция получения версии
 *
 *  В проекте : int GetDeviceVersion(&majorVersion, &minorVersion, &revision, &hwVersion);
 *  В   src/  : int GetDeviceVersion(uint8_t *majorVersion, uint8_t *minorVersion, uint8_t *revision);
 *
 *  Т.е. в src/ без параметра &hwVersion
 *
 *  Лишние параметры были убраны из ф-ций в проекте и все заработало !!!
 *
 *  В этом проекте буду допиливать src/ и сам проект для полного понимания как это все работает :)
 *
 *
 * //++++++++++++++++++++++++++++++++++++++++++++
 * 17.07.2022
 * Класс QSerialPort упоминается в CDobotConnector.cpp
 * Разобраться, что есть QScopedPointer ?
 *
 * Схема подключения к SerialPort
 *
 * MainWindow::onConnectDobot()--->int ConnectDobot(const char *portName, uint32_t baudrate, char *fwType, char *version) --
 * ---> (DobotDll.cpp)
 * Внутри ConnectDobot происходит вызов
 *
 *
 *    QMetaObject::invokeMethod(CDobot::instance()->connector,
                              "connectDobot",
                              Qt::QueuedConnection,
                              Q_ARG(void*, (void*)&(*isFinished)),
                              Q_ARG(void*, (void*)&(*result)),
                              Q_ARG(void*, (void*)portName),
                              Q_ARG(unsigned int, baudrate),
                              Q_ARG(void*, (void*)fwType),
                              Q_ARG(void*, (void*)version));

Где "connectDobot" - имя ф-ции в CDobotConnector.h, а остальное - параметры.

Заострим : Внутри ConnectDobot происходит вызов CDobot::instance()->connector,
которому соответствует вызов CDobotConnector::connectDobot
Внутри CDobotConnector::connectDobot() происходит вызов searchDobot(), где собирается инфа-по SerialPort-у
Вот этот самый CDobot::instance()->connector и выполняет подключение к SerialPort

А теперь разжуём...

CDobot *CDobot::instance(void) - создает объект instance = new CDobot(); и возвращает указатель на него.
Внутри конструктора CDobot::CDobot(QObject *parent) : QObject(parent)
создается объект (member) connector = new CDobotConnector();
А в классе CDobotConnector есть метод CDobotConnector::connectDobot

Таким образом, строчка "CDobot::instance()->connector" делает следующее :
- создает объект instance = new CDobot();
- Возвращает указатель на объект QObject, в данном случае это -
  public member of class CDobot с именем connector (указатель на объект класса CDobotConnector *), создаваемый внутри CDobot::CDobot(QObject *parent) : QObject(parent),
  (который сам является объектом класса CDobotConnector)

Дальше в QMetaObject::invokeMethod идет "connectDobot" - это имя member для объекта CDobot::instance()->connector в классе CDobotConnector.
Остальные 6 аргументов - параметры для CDobotConnector::connectDobot()

Т.е. QMetaObject::invokeMethod - говорящее название.
Вызывает метод "CDobotConnector::connectDobot", через member "CDobot::instance()->connector" для класса CDobot. Указатель на этот member это CDobot::instance()->connector

Рассмотрим работу CDobot::instance()
CDobot::instance() - создание нового объекта класса CDobot::
В конструкторе CDobot::CDobot(QObject *parent) создается 3 сущности - connector, protocol, communicator:

connector :
Объект CDobotConnector *connector
Объект QThread *connectorTargetThread

connector->moveToThread(connectorTargetThread);

QMetaObject::invokeMethod(connector, "onInit", Qt::BlockingQueuedConnection); - индикация, что поток запустился.

В итоге.
Вся соль в том, что работа с SerialPort идет в отдельном потоке CDobot::connector
Вся обработка протокола в отдельном потоке CDobot::protocol



 *
 */
#endif // EVKUZ_H
