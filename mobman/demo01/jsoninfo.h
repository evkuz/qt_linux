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
#include <QMetaObject>

//#include <QSharedPointer>

using ordered_json = nlohmann::ordered_json;


class JsonInfo : public QObject
{
    Q_OBJECT
public:
    JsonInfo(QString deviceName);
    QString DEV_NAME;
//    QJsonObject jsnObj1;
//    QJsonObject jsnObj2;

//     QJsonParseError jsonError; // ОШибка, если полученные данные - не JSON-объект

//     QString jsnData;
//     QJsonArray actions_list;

    std::string s1;
    QVariantMap map;


    //++++++++++++++++++++++ JSON data +++++++++++++++++++++++++++++++++++++++++++++

//    #define DEV_NAME "HIWONDER"   // device name - mobile manipulator
    #define RC_SUCCESS 0             // запрос выполнен успешно
    #define RC_WRONG_VALUE  -1     // неверные параметры
    #define RC_UNDEFINED    -2       // action с таким именем не найден
    #define RC_FAIL         -3       // Ошибка самого манипулятора, не открыт serial port
    #define RC_WAIT   -4
     //    #define RC_NO_DETECTION -5      // Нет детекции объекта.
    #define DEV_RC_ERROR -5 // "Device Error" - какой-то из девайсов не работает.

    const char* DEV_HEAD_STATE_RUN = "Running";
    const char* DEV_HEAD_STATE_WAIT = "Waiting"; //init
    const char* DEV_HEAD_STATE_FAIL = "Fail";
    const char* DEV_HEAD_STATE_DONE = "Done";


    #define DEV_HEAD_INFO_NO_DET    "No detection"
    #define DEV_HEAD_INFO_REQUEST   "Request Accepted"

    //#define DEV_ACTION_INFO   "Action is already running"

//    #define DEV_ACTION_STATE_RUN  "inprogress"
    #define DEV_ACTION_STATE_WAIT "waiting"
//    #define DEV_ACTION_STATE_FAIL "fail"
    #define DEV_ACTION_STATE_DONE "done"

    // #define DEV_ACTION_INFO_OUT "The object distance is out of range"




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

    typedef enum ActionResult{
        AC_RESULT_SUCCESS = 0,
        AC_RESULT_FAILURE = -2
    }ActionResult;



//    ordered_json jsnInfo;
    QString jsnData;
    QJsonObject jsnHeadStatus;      // Шапка в ответе на запрос status
//    QJsonObject jsnHeadServices;    // Шапка в ответе на запрос getservices
    QJsonObject jsnObj;

    QJsonObject jsnActionClamp;     // Объект экшена "clamp"
    QJsonObject jsnActionStart;     // Объект экшена "strart"
    QJsonObject jsnActionStandUP;   // "standup"
    QJsonObject jsnActionPutbox;    // for "put_box"
    QJsonObject jsnActionReset;     // for "reset"
    QJsonObject jsnActionCollapse;  // for "collapse"
    QJsonObject jsnActionUnKnown;   // for unknown action
    QJsonObject jsnActionLock;      // for "lock"
    QJsonObject jsnActionUnLock;    // for "unlock"

    QJsonObject jsnActionParking;   // mobman
    QJsonObject jsnActionGetBox;    // mobman
    QJsonObject jsnActionReady;     // mobman
    QJsonObject jsnActionForMoving; // mobman "formoving"
    QJsonObject jsnActionDetach;    // mobman "detach"
    QJsonObject jsnActionAttach;    // mobman "attach"
    QJsonObject jsnActionSetservos; // mobman "setservos="

    QJsonObject jsnActionList;      // list for "action_list" key
//    QJsonArray  jsnArray;           // list for action_list
    QJsonArray  jsnObjArray;        // array of objects
    QJsonArray  jsnObjServiceArray; // array of services

    QJsonObject  jsnServiceGetActions;  // mobman "/service?name=getactions"
    QJsonObject  jsnServiceGetServices;  // mobman "/service?name=getservices"
     //++++++++++++++++++++ NEW Actions
    QJsonObject jsnActionForMovingNEW; // mobman "formoving"



    QList<QJsonObject> actionListp;

    const int RC_NO_DETECTION = -5;      // Нет детекции объекта.
    const short INDEX_NODETECTION = 4;

    const char* DEV_STATUS = "TEST";

    const char* DEV_ACTION_INFO = "Action is already running";
    const char* DEV_ACTION_INFO_OUT = "The object distance is out of range";
    const char* DEV_ACTION_INFO_NODETECT = "NO DETECTION";
    const char* DEV_ACTION_INFO_INCORRECT = "INCORRECT distance";

    const char* DEV_ACTION_STATE_FAIL = "fail";
    const char* DEV_ACTION_STATE_RUN = "inprogress";
    const char* DEV_ACTION_STATE_NODETECT = "NO DETECTION";
    const char* DEV_ACTION_STATE_INCORRECT = "INCORRECT distance";

    // Global device status: init | run | fail
    const char* DEV_GLOBAL_STATE_INIT = "init";
    const char* DEV_GLOBAL_STATE_RUN  = "run";
    const char* DEV_GLOBAL_STATE_FAIL = "fail";


    void init_actions();
    void init_services();
    void init_actions_new();
    void resetAllActions();
    QString merge_json(QJsonObject src, QJsonObject dst);

//    QJsonObject returnJsonObject();
//    QJsonObject returnJsnInfo();
//    QJsonObject returnJsnStatus();
    QString     returnJsnData();
    QJsonObject returnJsonObject2();
    QJsonObject& returnJsnActionStart();
    QJsonObject& returnJsnActionGetBox();
    QJsonObject& returnJsnActionReset();
    QJsonObject& returnJsnActionClamp();
    QJsonObject& returnJsnActionCollapse();
    QJsonObject& returnJsnActionStandUP();
    QJsonObject& returnJsnActionsUnKnown();
    QJsonObject& returnJsnActionPutbox();
    QJsonObject& returnJsnActionLock();
    QJsonObject& returnJsnActionUnLock();
    QJsonObject& returnJsnActionParking();
    QJsonObject& returnJsnActionReady();
    QJsonObject& returnJsnActionForMoving();
    QJsonObject& returnJsnActionDetach();
    QJsonObject& returnJsnActionAttach();
    QJsonObject& returnJsnAcionSetservos();

    QString returnAllActions();
    QString returnAllServices(); // answer to "/service?name=getservices" request


    bool isAnyActionRunning; // флаг, что выополняется экшен
    void setActionDone(QJsonObject &theObj);  //Меняем rc of action upon device moving
//    void setJsnStatus();
    void setJsnHeadStatus(QJsonObject &theObj); // Меняем значения jsnHeadStatus на theObj
    void setCurrentAction(QString theAction);
    bool eraseArray(QJsonArray &theArray); // Очистка массива
    void setHeadStatusFail(); // serial port problem
    void setActionStart2NoDetection();
    QString createActionList(); // Подготовить список активных (выполняемых в данный момент) экшенов
    QString createStatusParamList(QString paramName); // Ответ на запрос статуса с параметрами
    //    QString createServiceList(); // Подготовить список доступных сервисов

    void SetJsnActionCollapse(QJsonObject &theObj);
    void SetJsnActionStandUP(QJsonObject &theObj);

    QString returnActionData(QString actionName);
    QString returnActionLaunch(QJsonObject &theObj);
    void setActionData(QJsonObject &theObj); // Меняем данные экшенов на данные "снаружи", целевой экшен определяем по name



    // Структура хранит данные для json-ответа на запрос статуса.
    struct statusHeader {
        std::string name;
        int     rc;    // "int - request result code",
        std::string  info;  // text interpretation of return code
        std::string  state; // "str - global device status: init | run | fail",
//        QString action_list; // Список активных на данных моментэкшенов. И вот тут вопрос :
                             // Или как jsnDocument или как nlohmann::ordered_json;
}; //statusHeader

    // Быстрый ответ на запуск экшена
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



    statusHeader currentStatus;
    LaunchActionState currentActionState; // НЕ используется...
    QString action_command; // команда, которая сейчас исполняется
    //                                             1                     3                         5                7
    const QList<QString> action_lst = {"clamp", "start"}; //, "put_box", "getactions", "getservices","reset", "lock", "unlock", "info"};
    //                                             1                    3
     QList<QString> statuslst = { "wait", "init", "inprogress", "done", "NoDetection" };


//    void struc_2_json(ordered_json& jsn, const statusHeader& header);   // конвертируем структуру в nlohmann::json object
//    void struc_2_jsnObj();  // QJsonObject& jsn, const StatusAnswer& header конвертируем структуру в QJsonObject::jsn
public slots:

void makeJson_Answer_Slot();
//void SetActionData_Slot(QJsonObject &theOb);

protected:

private:
    ordered_json jsnStatus; // Ответ на запрос статуса в формате json
    ordered_json jsnStatus01; //Модифицированный Ответ на запрос статуса в формате json
    ordered_json jsnAction;  // Хранит данные текущего экшена
    ordered_json jsnGetServicesAnswer; // Ответ на команду "/service?name=getservices"
    ordered_json jsnGetActionsAnswer; // Ответ на команду "/service?name=getactions"
    ordered_json jsnList; // JSON-объект Хранит JSON-список
    ordered_json ordjsnHeadStatus;  // Шапка в ответе в формате nlohmann

//    ordered_json jsnInfo;
    // List of actions
    ordered_json jsnActionTST;   // Оъект для тестов
//    ordered_json jsnActionLock;

    ordered_json jsnOB1;  // Объект для шапки, "заголовок"
    ordered_json jsnOB2;  // Объект для списка
    ordered_json jsnOB3;  // Объект результирующий.

    ordered_json& QtJson_2_NlohmannJson(QJsonObject theObj);     //convert single QJsonObject action to single  ordered_json object
    ordered_json& QtJson_2_NlohmannJson_QuickAnswer(QJsonObject theObj);// convert header of each answer
    ordered_json& QtJson_2_NlohmannJson_Head(QJsonObject theObj);// convert header of each answer
    ordered_json& QtJson_2_NlohmannJson_Data(QJsonObject theObj);// convert data[] of each answer

    ordered_json& getAllActionsOrderedJson(QJsonObject theObj);
    ordered_json actStatuses; // Хранит статусы экшена. ключ — имя статуса, значение — описание данного статуса.
    ordered_json actGetStatuses; // ТО же для команды "get_box", есть особые статусы
    ordered_json actSetServoParams;


    // Порядок элементов должен совпадать с action_lst
    //QList<QJsonObject> actionList = {&jsnActionClamp, &jsnActionStart};
//    QList<QJsonObject> actionListp;
//    Action clampAction, lockAction;
//    QList<Action> structActionList = {clampAction, lockAction};

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
