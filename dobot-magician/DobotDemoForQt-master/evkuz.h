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
 *
 *
 *
 */
#endif // EVKUZ_H
