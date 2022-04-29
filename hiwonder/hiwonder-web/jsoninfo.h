#ifndef JSONINFO_H
#define JSONINFO_H

#include <QObject>
#include "nlohmann/json.hpp"
using ordered_json = nlohmann::ordered_json;

class JsonInfo
{
public:
    JsonInfo();
    ordered_json jsnStatus; // Ответ на запрос статуса в формате json
    ordered_json jsnAction;  // Ответ на команду Action в формате json
    ordered_json jsnGetServicesAnswer; // Ответ на команду "/service?name=getservices"
    ordered_json jsnGetActionsAnswer; // Ответ на команду "/service?name=getactions"

    ordered_json jsnInfo;

    //++++++++++++++++++++++ JSON data +++++++++++++++++++++++++++++++++++++++++++++

    #define DEV_NAME "HIWONDER"   // device name - mobile manipulator
    #define RC_SUCCESS 0        // запрос выполнен успешно
    #define RC_WRONG_VALUE -1   // неверные параметры
    #define RC_UNDEFINED -2     // action с таким именем не найден
    #define RC_FAIL      -3     // Ошибка самого манипулятора, не открыт serial port

    #define AC_RUNNING 0        // action запущен
    #define AC_WRONG_VALUE -1   // action с таким именем не найден
    #define AC_FAILURE -2       // action с таким именем не запустился
    #define AC_ALREADY_HAVE -3  // action с таким именем уже запущен
    void init_json();

};

#endif // JSONINFO_H
