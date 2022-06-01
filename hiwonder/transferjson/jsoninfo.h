#ifndef JSONINFO_H
#define JSONINFO_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonParseError>
#include <QVariantMap>

class JsonInfo
{
//    Q_OBJECT
public:
    JsonInfo();
#define RC_SUCCESS 0             // запрос выполнен успешно
#define RC_WRONG_VALUE  -1     // неверные параметры
#define RC_UNDEFINED    -2       // action с таким именем не найден
#define RC_FAIL         -3       // Ошибка самого манипулятора, не открыт serial port
#define RC_WAIT   -4

#define DEV_ACTION_STATE_RUN  "inprogress"
#define DEV_ACTION_STATE_WAIT "waiting"
#define DEV_ACTION_STATE_FAIL "fail"
#define DEV_ACTION_STATE_DONE "done"

#define DEV_HEAD_INFO_REQUEST   "Request Accepted"

    QJsonObject jsnActionList;  // list for "action_list" key
//    QJsonArray  jsnArray;       // list for action_list
    QJsonArray  jsnObjArray;    // array of objects



    QJsonObject jsnActionReset;     // for "reset"
    QJsonObject jsnActionCollapse;  // for "collaps"


    void init_actions();

    QJsonObject returnJsnActionReset();
    QJsonObject& returnJsnActionCollapse();

    void SetJsnActionCollapse(QJsonObject &theObj);

};

#endif // JSONINFO_H
