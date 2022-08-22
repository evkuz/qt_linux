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
#include <QVariantMap>

using ordered_json = nlohmann::ordered_json;


class JsonInfo: public QObject
{
    Q_OBJECT

public:
    JsonInfo();
     QJsonObject jsnObj1;
     QJsonObject jsnObj2;

//     QJsonParseError jsonError; // ОШибка, если полученные данные - не JSON-объект

//     QString jsnData;
//     QJsonArray actions_list;

     std::string s1;
     QVariantMap map;


    //++++++++++++++++++++++ JSON data +++++++++++++++++++++++++++++++++++++++++++++

    #define DEV_NAME "HIWONDER"   // device name - mobile manipulator
    #define RC_SUCCESS 0             // запрос выполнен успешно
    #define RC_WRONG_VALUE  -1     // неверные параметры
    #define RC_UNDEFINED    -2       // action с таким именем не найден
    #define RC_FAIL         -3       // Ошибка самого манипулятора, не открыт serial port
    #define RC_WAIT   -4
     //    #define RC_NO_DETECTION -5      // Нет детекции объекта.
    #define DEV_RC_ERROR -5 // "Device Error" - какой-то из девайсов не работает.

    #define DEV_HEAD_STATE_RUN  "Running"
    #define DEV_HEAD_STATE_WAIT "Waiting"
    #define DEV_HEAD_STATE_FAIL "Fail"
    #define DEV_HEAD_STATE_DONE "Done"


    #define DEV_HEAD_INFO_NO_DET    "No detection"
    #define DEV_HEAD_INFO_REQUEST   "Request Accepted"

    #define DEV_ACTION_INFO   "Action is already running"

    #define DEV_ACTION_STATE_RUN  "inprogress"
    #define DEV_ACTION_STATE_WAIT "waiting"
    #define DEV_ACTION_STATE_FAIL "fail"
    #define DEV_ACTION_STATE_DONE "done"




    #define AC_STATE_DONE  "DONE"


    #define AC_RUNNING 0        // action запущен
    #define AC_WRONG_VALUE -1   // action с таким именем не найден
    #define AC_FAILURE -2       // action с таким именем не запустился
    #define AC_ALREADY_HAVE -3
    #define AC_DONE -4          // Ожидание
    #define AC_POSTPONED -5     // action отложен до завершения текущего

     typedef enum ActionLaunchAnswer{
         AC_Launch_RC_RUNNING = 0,       // action запущен
         AC_Launch_RC_WRONGVALUE = -1,   // action с таким именем не найден
         AC_Launch_RC_FAILURE = -2,      // action с таким именем не запустился
         AC_Launch_RC_ALREADY_HAVE = -3, // action с таким именем уже запущен
         AC_Launch_RC_DONE = -4          // Ожидание

     }ActionLaunchAnswer;




    ordered_json jsnInfo;
    QString jsnData;
    QJsonObject jsnHeadStatus;  // Шапка в ответе
    QJsonObject   jsnObj;

     QJsonObject jsnActionClamp;     // Объект экшена "clamp"
     QJsonObject jsnActionStart;     // Объект экшена "strart"
     QJsonObject jsnActionStandUP;   // "standup"
     QJsonObject jsnActionPutbox;    // for "put_box"
     QJsonObject jsnActionReset;     // for "reset"
     QJsonObject jsnActionCollapse;  // for "collapse"
     QJsonObject jsnActionUnKnown;   // for unknown action
     QJsonObject jsnActionLock;      // for "lock"
     QJsonObject jsnActionUnLock;    // for "unlock"

     QJsonObject jsnActionList;  // list for "action_list" key
     QJsonArray  jsnArray;       // list for action_list
     QJsonArray  jsnObjArray;    // array of objects


     QList<QJsonObject> actionListp;

    const int RC_NO_DETECTION = -5;      // Нет детекции объекта.
    const short INDEX_NODETECTION = 4;


    void init_json();
    void init_actions();
    void resetAllActions();
    QString merge_json(QJsonObject src, QJsonObject dst);

    QJsonObject returnJsonObject();
    QJsonObject returnJsnInfo();
    QJsonObject returnJsnStatus();
    QString     returnJsnData();
    QJsonObject returnJsonObject2();
    QJsonObject& returnJsnActionStart();
    QJsonObject& returnJsnActionReset();
    QJsonObject& returnJsnActionClamp();
    QJsonObject& returnJsnActionCollapse();
    QJsonObject& returnJsnActionStandUP();
    QJsonObject& returnJsnActionsUnKnown();
    QJsonObject& returnJsnActionPutbox();
    QJsonObject& returnJsnActionLock();
    QJsonObject& returnJsnActionUnLock();

    QJsonObject returnAllActions();

    bool isAnyActionRunning; // флаг, что выополняется экшен
    void setActionDone(QJsonObject &theObj);  //Меняем rc of action upon device moving
    void setJsnStatus();
    void setJsnHeadStatus(QJsonObject &theObj); // Меняем значения jsnHeadStatus на theObj
    void setCurrentAction(QString theAction);
    bool eraseArray(QJsonArray &theArray); // Очистка массива
    void setHeadStatusFail(); // serial port problem
    void setActionStart2NoDetection();
    void createActionList(); // Подготовить список активных (выполняемых в данный момент) экшенов
    void SetJsnActionCollapse(QJsonObject &theObj);
    void SetJsnActionStandUP(QJsonObject &theObj);
    void setActionData(QJsonObject &theObj); // Меняем данные экшенов на данные "снаружи", целевой экшен определяем по name


    // Структура хранит данные для json-ответа.
    struct Action {
        std::string name;
        int     rc;    // "int - request result code",
        std::string  info;  // text interpretation of return code
        std::string  state; // "str - global device status: init | run | fail",
//        QString action_list; // Список активных на данных моментэкшенов. И вот тут вопрос :
                             // Или как jsnDocument или как nlohmann::ordered_json;
}; //Action

    struct LaunchActionState {
        std::string name;
        int     rc;    // "int - request result code",
        std::string  info;  // text interpretation of return code
    //        -0 - action запущен -  "Is running"
    //        -1 - action с таким именем не найден
    //        -2 - action с таким именем не запустился, т.е. ошибка ?
    //        -3 - action с таким именем уже запущен
    //        -4 - action с таким именем не запущен (ожидание)


    };//struct LaunchActionState



    Action currentStatus;
    LaunchActionState currentActionState;
    QString action_command; // команда, которая сейчас исполняется
    //                                             1                     3                         5                7
    const QList<QString> action_lst = {"clamp", "start"}; //, "put_box", "getactions", "getservices","reset", "lock", "unlock", "info"};
    //                                             1                    3
     QList<QString> statuslst = { "wait", "init", "inprogress", "done", "NoDetection" };


    void struc_2_json(ordered_json& jsn, const Action& header);   // конвертируем структуру в nlohmann::json object
    void struc_2_jsnObj();  // QJsonObject& jsn, const StatusAnswer& header конвертируем структуру в QJsonObject::jsn
public slots:

void makeJson_Answer_Slot();
protected:

private:
    ordered_json jsnStatus; // Ответ на запрос статуса в формате json
    ordered_json jsnStatus01; //Модифицированный Ответ на запрос статуса в формате json
    ordered_json jsnAction;  // Хранит данные текущего экшена
    ordered_json jsnGetServicesAnswer; // Ответ на команду "/service?name=getservices"
    ordered_json jsnGetActionsAnswer; // Ответ на команду "/service?name=getactions"
    ordered_json jsnList; // JSON-объект Хранит JSON-список

//    ordered_json jsnInfo;
    // List of actions
    ordered_json jsnActionTST;   // Оъект для тестов
//    ordered_json jsnActionLock;

    ordered_json jsnOB1;  // Объект для шапки, "заголовок"
    ordered_json jsnOB2;  // Объект для списка
    ordered_json jsnOB3;  // Объект результирующий.

    // Порядок элементов должен совпадать с action_lst
    //QList<QJsonObject> actionList = {&jsnActionClamp, &jsnActionStart};
//    QList<QJsonObject> actionListp;
    Action clampAction, lockAction;
    QList<Action> structActionList = {clampAction, lockAction};

    QJsonObject qjsnAnswer;

    QJsonDocument jsnDoc;    // json-данные, полученные по tcp
//    QJsonObject   jsnObj;    // ОБъект, хранящий весь JSON ответ от девайса
                             // В него подставляем разные QJsonDocument из разных ordered_json объектов
                             // Это общая переменная для хранения json-объекта с разными данныими, из разных переменных.
    QJsonObject   jsndataObj;// ОБъект, хранящий вложенный JSON-объект (вложенный внутри ответа jsnObj)
                             //   Тут как раз данные о distance
    QJsonObject   jsnActionAnswer; // Ответ на команду Action в формате json
    QJsonObject   jsnStatusActionAnswer; // Ответ на команду "status?action=getbox" в формате json

    //QJsonObject   jsnGetServicesAnswer; // Ответ на команду "/service?name=service_name"

    QJsonParseError jsonError; // ОШибка, если полученные данные - не JSON-объект

//    QString jsnData;
    QJsonArray actions_list;
//    QJsonObject jsnHeadStatus;  // Шапка в ответе



}; // class JsonInfo
#endif // JSONINFO_H
