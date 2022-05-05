#ifndef JSONINFO_H
#define JSONINFO_H
/*
 * 05.05.2022
 * Класс для работы с JSON-данными


*/

#include <QObject>
#include "nlohmann/json.hpp"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonParseError>


using ordered_json = nlohmann::ordered_json;



class JsonInfo
{
public:
    JsonInfo();
    ordered_json jsnStatus; // Ответ на запрос статуса в формате json
    ordered_json jsnStatus01; //Модифицированный Ответ на запрос статуса в формате json
    ordered_json jsnAction;  // Хранит данные текущего экшена
    ordered_json jsnGetServicesAnswer; // Ответ на команду "/service?name=getservices"
    ordered_json jsnGetActionsAnswer; // Ответ на команду "/service?name=getactions"
    ordered_json jsnList; // JSON-объект Хранит JSON-список

    ordered_json jsnInfo;

    QJsonObject qjsnAnswer;

    QJsonDocument jsnDoc;    // json-данные, полученные по tcp
    QJsonObject   jsnObj;    // ОБъект, хранящий весь JSON ответ от девайса
    QJsonObject   jsndataObj;// ОБъект, хранящий вложенный JSON-объект (вложенный внутри ответа jsnObj)
                             //   Тут как раз данные о distance
    QJsonObject   jsnActionAnswer; // Ответ на команду Action в формате json
    QJsonObject   jsnStatusActionAnswer; // Ответ на команду "status?action=getbox" в формате json

    //QJsonObject   jsnGetServicesAnswer; // Ответ на команду "/service?name=service_name"

    QJsonParseError jsonError; // ОШибка, если полученные данные - не JSON-объект

    QString jsnData;
    QJsonArray actions_list;


    //++++++++++++++++++++++ JSON data +++++++++++++++++++++++++++++++++++++++++++++

    #define DEV_NAME "HIWONDER"   // device name - mobile manipulator
    #define RC_SUCCESS 0        // запрос выполнен успешно
    #define RC_WRONG_VALUE -1   // неверные параметры
    #define RC_UNDEFINED -2     // action с таким именем не найден
    #define RC_FAIL      -3     // Ошибка самого манипулятора, не открыт serial port

    #define DEV_STATE_RUN  "Running"
    #define DEV_STATE_WAIT "Waiting"
    #define DEV_STATE_FAIL "Fail"


    #define AC_RUNNING 0        // action запущен
    #define AC_WRONG_VALUE -1   // action с таким именем не найден
    #define AC_FAILURE -2       // action с таким именем не запустился
    #define AC_ALREADY_HAVE -3  // action с таким именем уже запущен
    void init_json();

    // Структура хранит данные для json-ответа.
    struct StatusAnswer {
        QString name;
        int     rc;    // "int - request result code",
        QString info;  // text interpretation of return code
        QString state; // "str - global device status: init | run | fail",
        QString action_list; // Список активных на данных моментэкшенов. И вот тут вопрос :
                             // Или как jsnDocument или как nlohmann::ordered_json;



    //        - 0 - action запущен -  "Is running"
    //        -1 - action с таким именем не найден
    //        -2 - action с таким именем не запустился, т.е. ошибка ?
    //        -3 - action с таким именем уже запущен
    //        -4 - action с таким именем не запущен (ожидание)


    };//struct StatusAnswer

    StatusAnswer currentStatus;
    QString action_command; // команда, которая сейчас исполняется
    const QList<QString> action_lst = {"clamp", "start", "put_box", "getactions", "getservices","reset", "lock", "unlock", "info"};

}; // class JsonInfo
#endif // JSONINFO_H
