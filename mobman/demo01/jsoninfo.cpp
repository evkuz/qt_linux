#include "jsoninfo.h"

JsonInfo::JsonInfo(QString deviceName)
{

    DEV_NAME = deviceName;
    currentStatus = {DEV_NAME.toStdString(), RC_SUCCESS,  "OK", DEV_HEAD_STATE_WAIT}; // Инициализируем структуру
    action_command = "nothing";
    init_actions();
    init_services();
    init_actions_new();
    isAnyActionRunning = false;
}
//+++++++++++++++++++++
//++++++++++++++++++++++++++++++++

void JsonInfo::init_actions()
{
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    jsnActionStart = {
        {"name", "start"},
        {"state", "waiting"},
        {"info", "Get the box by clamper, asking CV about distance in advance"},
        {"result", RC_WAIT}
      };


    jsnActionGetBox = {
        {"name", "get_box"},
        {"state", "waiting"},
        {"info", "Get the box by clamper, asking CV about distance in advance"},
        {"result", RC_WAIT}
      };

    jsnActionPutbox = {
        {"name", "put_box"},
        {"state", DEV_ACTION_STATE_DONE},
        {"info", "put down the object for next actions"},
        {"result", this->AC_Launch_RC_DONE}
       };

    jsnActionReset = {
        {"name", "reset"},
        {"state", DEV_ACTION_STATE_DONE},
        {"info", "Set device status as <Wait> in main Header"},
        {"result",  RC_WAIT}
    };

    jsnActionCollapse = {
        {"name", "collapse"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "make long time opeartion for test"},
        {"result",  RC_WAIT}
    };

    jsnActionStandUP = {
        {"name", "standup"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "Go to start (initital) position"},
        {"result", RC_WAIT}
    };

    jsnActionUnKnown = {
        {"name", "UnKnown"},
        {"state", DEV_ACTION_STATE_FAIL},
        {"info", "There is now action with such a name"},
        {"result", AC_WRONG_VALUE}
    };
    jsnActionLock = {
        {"name", "lock"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "lock the gripper of manipulator"},
        {"result",  this->AC_Launch_RC_DONE}
    };
    jsnActionUnLock = {
        {"name", "unlock"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "unlock the gripper of manipulator"},
        {"result",  this->AC_Launch_RC_DONE}
    };
    jsnActionParking = {
        {"name", "parking"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "Go to start (initital) position"}, //"Set device's clamper in transporting position"
        {"result", RC_WAIT}
    };

    jsnActionReady = {
        {"name", "ready"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "TEST position"}, //"Set device's clamper in transporting position"
        {"result", RC_WAIT}

    };

    jsnActionForMoving = {
        {"name", "formoving"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "Going to target position"}, //"Set device's clamper in transporting position"
        {"result", RC_WAIT}


    };

    jsnActionDetach = {
        {"name", "detach"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "Detach servos FROM arduino pins"},
        {"result", RC_WAIT}
    };

    jsnActionAttach = {
        {"name", "attach"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "Attach servos TO arduino pins"},
        {"result", RC_WAIT}

    };

    jsnActionSetservos = {
        {"name", "setservos="},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "Move servos to values supplied, as parameters"},
        {"result", RC_WAIT}


    };

// actions array
    jsnObjArray = {jsnActionParking, jsnActionReady,  jsnActionReset,     jsnActionGetBox, jsnActionPutbox, \
                   jsnActionUnKnown, jsnActionLock,   jsnActionUnLock, jsnActionForMoving, jsnActionDetach, \
                   jsnActionAttach,  jsnActionSetservos
    }; // jsnObjArray

    // Поле "name" задается 1 раз и больше не меняется.
    jsnHeadStatus = {
        {"name", DEV_NAME},
        {"rc", RC_UNDEFINED}, //RC_SUCCESS
        {"info", "Request Accepted"},
        {"state", DEV_HEAD_STATE_WAIT}
    };

    //jsnArray = {};
    jsnActionList = {
        {"action_list", jsnObjArray} //
    };

}// init_actions
//++++++++++++++++++++++++++++++++

void JsonInfo::init_services()
{
    jsnServiceGetActions = {
        {"name", "getactions"},
        {"info", "Return list of actions available"},
        {"rc", RC_UNDEFINED} // инициализация, в runtime будет переписываться

    };

    jsnServiceGetServices = {
        {"name", "getservices"},
        {"info", "Return list of services available"},
        {"rc", RC_UNDEFINED} // инициализация, в runtime будет переписываться

    };

    jsnObjServiceArray = {jsnServiceGetActions, jsnServiceGetServices};
//    jsnHeadServices = {
//        {"name", DEV_NAME},
//        {"rc", RC_UNDEFINED}, //RC_SUCCESS
//        {"state", "Wait"},
//        {"info", "Request Accepted"}
//    };

} // init_services
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Обновленная версия экшенов
void JsonInfo::init_actions_new()
{
//    jsnActionForMovingNEW = {
//        {"name", "formoving"},
//        {"rc", RC_WAIT},
//        {"info", "Going to target position"}, //"Set device's clamper in transporting position"
//        {"state", DEV_ACTION_STATE_WAIT}
//    };

//    ordjsnHeadStatus = {
//        {"name", DEV_NAME.toStdString()},
//        {"rc", RC_UNDEFINED}, //RC_SUCCESS
//        {"info", "Request Accepted"},
//        {"state", RC_WAIT}
//    };

    actStatuses = {
        {"Running", "Action is running"},
        {"Fail", "Action launch is failed"},
        {"Waiting", "Action is in awaiting state"}
    };

    actGetStatuses = {
        {"Running", "Action is running"},
        {"Fail", "Action launch is failed"},
        {"Waiting", "Action is in awaiting state"},
        {"No Detection !", "There is no detection of object by CV-camera"}
    };

    actSetServoParams = {
        {"Servo_1", "servo 1 value 0-180, base"},
        {"Servo_2", "servo 2 value 0-180"},
        {"Servo_3", "servo 3 value 0-180"},
        {"Servo_4", "servo 4 value 0-180, gripper"},
        };
//        values in range 0-180 Degrees, separated by coma, started from base to gripper""


//        {"Undefined", "There is now action with such a name"},


} // init_actions_new

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Тут разные списки - action_lst и actionListp. Следует учитывать.
void JsonInfo::resetAllActions()
{
    QJsonObject myObject;
    for (int i=0; i < jsnObjArray.size(); i++) //action_lst.size()
    {
       myObject = jsnObjArray.at(i).toObject();
       myObject["rc"] = this->AC_Launch_RC_DONE;
       myObject["state"] = "done";
       jsnObjArray.replace(i, myObject);

//       jsnObjArray.at(i).toObject()["rc"] = this->AC_Launch_RC_DONE;
//       jsnObjArray.at(i).toObject()["state"] = "done";

    }
    isAnyActionRunning = false; // Нет активных экшенов

} //resetAllActions()
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// merge src and dst QJsonObjrcts and return QString representation of the result
QString JsonInfo::merge_json(QJsonObject src, QJsonObject dst)
{
    map = src.toVariantMap();
    map.insert(dst.toVariantMap());
    dst = QJsonObject::fromVariantMap(map);
    jsnDoc = QJsonDocument(dst);
    jsnObj = jsnDoc.object(); // Так можно получить доступ извне через returnJsonObject()

    QString str = jsnDoc.toJson(QJsonDocument::Indented);

    return str;

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void JsonInfo::struc_2_json(ordered_json &jsn, const Action &header)
{
    jsn = ordered_json{
                        {"name", header.name},
                        {"rc", header.rc},
                        {"info", header.info},
                        {"state", header.state}
    };

}//struc_2_json
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Копируем данные структуры в ordered_json jsnStatus
void JsonInfo::struc_2_jsnObj()
{
    jsnStatus["name"] = currentStatus.name;
    jsnStatus["rc"] = currentStatus.rc;
    jsnStatus["info"] = currentStatus.info;
    jsnStatus["state"] = currentStatus.state;
//QJsonObject jsnaa
//            {
//                {"name", header.name},
//                {"rc", header.rc},
//                {"info", header.info},
//                {"state", header.state}
//             };

}//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Слот сигнала MainProcess::makeJson_Answer_Signal(QString theAction)
// ГОтовим json-ответ - статус экшена
void JsonInfo::makeJson_Answer_Slot()
{
    // По имени экшена идем по списку, и мерджим с соответствующим объектом.
    //this->jsnData = merge_json(jsnObj2, jsnObj);
    this->jsnData = merge_json(jsnActionList, jsnObj);

    // Теперь результат объединения находится в jsnObj2
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// слот сигнала MainProcess::SetActionData_Signal
void JsonInfo::SetActionData_Slot(QJsonObject &theObj)
{
    // Ищем в jsnObjArray по "name" нужный action, меняем его на значения theobj
    // Так работаем только с массивом.
        QString theName = theObj.value("name").toString();
        // Проходим jsnObjArray, ищем совпадение по "name", меняем "rc" и "state", делаем replase

        for (int i=0; i < jsnObjArray.size(); i++) {
            if (jsnObjArray.at(i).toObject().value("name").toString() == theName){
                jsnObjArray.replace(i, theObj);
            }
        }

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Преобразуем список data[] в ОТВЕТЕ
ordered_json& JsonInfo::QtJson_2_NlohmannJson(QJsonObject theObj)
{
   // QMap <QJs>
    jsnAction.empty(); // Очищиаем.
    QString str1, str2;

    str1 = theObj.value("name").toString();
    str2 = theObj.value("info").toString();
    jsnAction = {
        {"name", str1.toStdString()},
        {"description", str2.toStdString()},
        {"parameters", "No any parameters"},
//        {"statuses", this->actStatuses}

    };

    if (str1 == "setservos="){
        jsnAction["parameters"] = this->actSetServoParams;
    }

    if (str1 == "get_box") { // Тут особые статусы есть, не как у остальных экшенов
         jsnAction["statuses"] = this->actGetStatuses;
    }
    // Результат вызова getservices аналогичен результату вызова getactions,
    // за исключением отсутствующего поля statuses.
    else if ((str1 != "getservices") && (str1 != "getactions")) {
        jsnAction["statuses"] = this->actStatuses;

    }

    return jsnAction;
} //QtJson_2_NlohmannJson
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// преобразуем ЗАГОЛОВОК ОТВЕТА QJsonObject -> ordered_json
// theobj - экшен
ordered_json &JsonInfo::QtJson_2_NlohmannJson_Head(QJsonObject theObj)
{
    jsnAction.empty(); // Очищиаем.
    QString str1, str2, str3, str4;
    int rc;

    str1 = theObj.value("name").toString();
    str2 = theObj.value("info").toString();
    rc = theObj.value("rc").toInt();
    str3 = theObj.value("state").toString();
    jsnAction = {
        {"name", str1.toStdString()},
        {"rc", rc},
        {"info", str2.toStdString()},
        {"state", str3.toStdString()}
    };

    return jsnAction;


} // QtJson_2_NlohmannJson_Head
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Преобразуем список data[] в ОТВЕТЕ
ordered_json &JsonInfo::QtJson_2_NlohmannJson_Data(QJsonObject theObj)
{
    jsnAction.empty(); // Очищиаем.
    QString str1, str2, str3, str4;
    int result;

    str1 = theObj.value("name").toString();
    str2 = theObj.value("info").toString();
    result = theObj.value("result").toInt();
    str3 = theObj.value("state").toString();
    jsnAction = {
        {"name", str1.toStdString()},
        {"state", str3.toStdString()},
        {"info", str2.toStdString()},
        {"result", result}
    };

    return jsnAction;

} // QtJson_2_NlohmannJson_Data
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QJsonObject JsonInfo::returnJsonObject()
{
    // Передаем основной json-object на обработку/парсинг
    return jsnObj;
} // returnJsonObject
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QJsonObject JsonInfo::returnJsnInfo()
{
// Берем данные из ordered_json jsnInfo и передаем через QJsonObject
    int indent = 3;
    std::string s2 = jsnInfo.dump(indent);
    jsnData = QString::fromStdString(s2);
    jsnDoc = QJsonDocument::fromJson(jsnData.toUtf8(), &jsonError);

    //Get the main JSON object and get the data in it
    jsnObj = jsnDoc.object();

    return jsnObj;
} // returnJsnInfo()
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QJsonObject JsonInfo::returnJsnStatus()
{
    // Берем данные из ordered_json jsnStatus и передаем через QJsonObject
//        int indent = 3;
//        std::string s2 = jsnStatus.dump(indent);
//        jsnData = QString::fromStdString(s2);
//        jsnDoc = QJsonDocument::fromJson(jsnData.toUtf8(), &jsonError);

//        //Get the main JSON object and get the data in it
//        jsnObj = jsnDoc.object();

        return jsnHeadStatus;

}// returnJsnStatus()
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QString JsonInfo::returnJsnData()
{
    // Делаем поиск активных экшенов и если их нет, то меняем jsndata
    return this->jsnData;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
QJsonObject JsonInfo::returnJsonObject2()
{
    return this->jsnObj2;
}
//+++++++++++++++++++++++++++++++++++++++++++
QJsonObject& JsonInfo::returnJsnActionStart()
{
    return this->jsnActionStart;
}
//+++++++++++++++++++++++++++++++++++++++++++
QJsonObject &JsonInfo::returnJsnActionGetBox()
{
    return this->jsnActionGetBox;
}
//+++++++++++++++++++++++++++++++++++++++++++
QJsonObject& JsonInfo::returnJsnActionReset()
{
    return this->jsnActionReset;
}
//+++++++++++++++++++++++++++++++++++++++++++

QJsonObject& JsonInfo::returnJsnActionClamp()
{
    return this->jsnActionClamp;
}
//+++++++++++++++++++++++++++++++++++++++++++

QJsonObject& JsonInfo::returnJsnActionCollapse()
{
    return jsnActionCollapse;
}

QJsonObject& JsonInfo::returnJsnActionStandUP()
{
    return this->jsnActionStandUP;
}
//+++++++++++++++++++++++++++++++++++++++++++++
QJsonObject &JsonInfo::returnJsnActionsUnKnown()
{
    return this->jsnActionUnKnown;
}
//+++++++++++++++++++++++++++++++++++++++++++++

QJsonObject &JsonInfo::returnJsnActionPutbox()
{
    return this->jsnActionPutbox;
}
//+++++++++++++++++++++++++++++++++++++++++++++
QJsonObject &JsonInfo::returnJsnActionLock()
{
    return this->jsnActionLock;
}
//+++++++++++++++++++++++++++++++++++++++++++++
QJsonObject &JsonInfo::returnJsnActionUnLock()
{
    return this->jsnActionUnLock;
}
//+++++++++++++++++++++++++++++++++++++++++++++
QJsonObject &JsonInfo::returnJsnActionParking()
{
    return this->jsnActionParking;
}
//+++++++++++++++++++++++++++++++++++++++++++++
QJsonObject &JsonInfo::returnJsnActionReady()
{
    return this->jsnActionReady;
}
//+++++++++++++++++++++++++++++++++++++++++++++

QJsonObject &JsonInfo::returnJsnActionForMoving()
{
    return this->jsnActionForMoving;
}
//+++++++++++++++++++++++++++++++++++++++++++++
QJsonObject &JsonInfo::returnJsnActionDetach()
{
    return this->jsnActionDetach;
}
//+++++++++++++++++++++++++++++++++++++++++++++
QJsonObject &JsonInfo::returnJsnActionAttach()
{
    return this->jsnActionAttach;
}
//+++++++++++++++++++++++++++++++++++++++++++++
QJsonObject &JsonInfo::returnJsnAcionSetservos()
{
    return this->jsnActionSetservos;
}
//+++++++++++++++++++++++++++++++++++++++++++
// Формируем список всех доступных экшенов
QString JsonInfo::returnAllActions()
{
    QJsonValue myValue;
    QJsonArray myArray;
    QJsonObject myobj;

    jsnGetActionsAnswer.clear();

    jsnGetActionsAnswer["name"] = "getactions";
    jsnGetActionsAnswer["rc"] = RC_SUCCESS;
    jsnGetActionsAnswer["info"] = "successful";
    //jsnGetActionsAnswer["data"]


    myArray = this->jsnObjArray;
    for (int i=0; i < myArray.size(); i++){
        myobj = myArray.at(i).toObject();
        jsnAction = QtJson_2_NlohmannJson(myobj); // adding statuses if needed
        jsnGetActionsAnswer["data"] += jsnAction;

    } // for

    return QString::fromStdString(jsnGetActionsAnswer.dump(3)); // Изменено в merge_json


} // returnAllActions()
//++++++++++++++++++++++++++++++++++++++++
// Формируем список всех доступных сервисов
QString JsonInfo::returnAllServices()
{
    QJsonValue myValue;
    QJsonArray myArray;
    QJsonObject myobj;

    jsnGetServicesAnswer.clear();

    jsnGetServicesAnswer["name"] = "getservices";
    jsnGetServicesAnswer["rc"] = RC_SUCCESS;
    jsnGetServicesAnswer["info"] = "successful";
    //jsnGetActionsAnswer["data"]


    myArray = this->jsnObjServiceArray;
    for (int i=0; i < myArray.size(); i++){
        myobj = myArray.at(i).toObject();
        jsnAction = QtJson_2_NlohmannJson(myobj); // adding statuses if needed
        jsnGetServicesAnswer["data"] += jsnAction;

    } // for

    return QString::fromStdString(jsnGetServicesAnswer.dump(3)); // Изменено в merge_json

}
//+++++++++++++++++++++++++++++++++++++++++++

void JsonInfo::setActionDone(QJsonObject &theObj)
{
    theObj["result"] = -4;
    theObj["state"] = "done"; //success

    QString theName = theObj.value("name").toString();

    // Вот тут по "name" нужно менять соответствующий экшен

    QJsonValue myValue;
    // Проходим jsnObjArray, ищем совпадение по "name", меняем "rc" и "state", делаем replase

    for (int i=0; i < jsnObjArray.size(); i++) {
        if (jsnObjArray.at(i).toObject().value("name").toString() == theName){
            jsnObjArray.replace(i, theObj);
        }
    }


    jsnActionList["action_list"] = jsnObjArray;
    this->jsnData = merge_json(jsnActionList, jsnHeadStatus);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void JsonInfo::setJsnStatus()
{
    // Берем данные из ordered_json jsnStatus и передаем через QJsonObject
        int indent = 3;
        std::string s2 = jsnStatus.dump(indent);
        jsnData = QString::fromStdString(s2);
        jsnDoc = QJsonDocument::fromJson(jsnData.toUtf8(), &jsonError);

        //Get the main JSON object and get the data in it
        jsnObj = jsnDoc.object();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void JsonInfo::setJsnHeadStatus(QJsonObject &theObj)
{
    jsnHeadStatus["state"] = theObj.value("state").toString();// "Running";
    jsnHeadStatus["info"]  = theObj.value("info").toString();// "Action performing";
    jsnHeadStatus["rc"] = theObj.value("rc").toInt();   // RC_SUCCESS;

//    jsnHeadStatus["state"] = "Running";
//    jsnHeadStatus["info"]  = "Action performing";
//    jsnHeadStatus["rc"] = RC_SUCCESS;


} // setJsnHeadStatus
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Передаем строковое значение theAction, а возможно лучше индекс...
void JsonInfo::setCurrentAction(QString theAction)
{
    //Меняем значение текущего экшена. И в целом, сам QJsonObject
    this->action_command = theAction;
    // И дальше, по имени экшена меняем остальные параметры JSON, если необходимо.

} // setCurrentAction
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Make empty the action_list
bool JsonInfo::eraseArray(QJsonArray &theArray)
{
    int sz = theArray.size();
    bool OK = false;
    for (int i =0; i < sz; i++){
        if (!theArray.isEmpty() ){
            theArray.removeLast();
            OK = true;
        }
        else {return true;}

    }
    return OK;
} // eraseArray()
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void JsonInfo::setHeadStatusFail()
{
    jsnHeadStatus["state"] = "Fail";
    jsnHeadStatus["info"]  = "Serial port malfunction";
    jsnHeadStatus["rc"] = RC_FAIL;

} //setHeadStatusFail()
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void JsonInfo::setActionStart2NoDetection()
{
    jsnActionStart["state"] = DEV_ACTION_STATE_FAIL;
    jsnActionStart["info"] = DEV_HEAD_INFO_NO_DET;
    jsnActionStart["rc"] = this->AC_Launch_RC_FAILURE;

//    jsnHeadStatus["state"] = DEV_HEAD_STATE_RUN;
    jsnHeadStatus["info"]  = "No Detection";
//    jsnHeadStatus["rc"] = this->AC_Launch_RC_RUNNING;

//    QJsonValue myValue = jsnActionStart;

//    bool OK = this->eraseArray(this->jsnObjArray);
//    if (!OK){
//        this->jsnObjArray.append(myValue);
//    }


//    jsnActionList["action_list"] = this->jsnObjArray;
//    this->jsnData = merge_json(jsnActionList, jsnHeadStatus);
//    OK = this->eraseArray(this->jsnObjArray);

this->jsnData = QJsonDocument(jsnActionStart).toJson(QJsonDocument::Indented);
// That's ok. But now changed value of jsnActionStart should be copied to jsnObjArray
    this->setActionData(jsnActionStart);
    this->setActionData(jsnHeadStatus);

} // setActionStart2NoDetection
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// fills Up the jsnActionList["action_list"] with active actions only
// required for answer to status request
QString JsonInfo::createActionList()
{
    // Перебираем список  jsnObjArray, определяем у кого state == "inprogress"
    // формируем новый список
    // добавляем его в jsnData
    QJsonValue myValue;
    QJsonObject myobj;
    QJsonArray myArray;



   myArray = this->jsnObjArray; // Это копирование всех данных. массив jsnObjArray не меняется
   //myArray.swap(jsnObjArray);
   //jsnObjArray.swap(myArray);
   QString str;
    for (int i=0; i < myArray.size(); i++){
        myobj = myArray.at(i).toObject();
        str = myobj.value("state").toString();
//        если не равно - вынимаем объект из массива, т.е. оставляем только running
//       if (QString::compare(str, DEV_ACTION_STATE_RUN)){
         if (str != DEV_ACTION_STATE_RUN ){
             myArray.removeAt(i);
             i--;

       }
    }

    if (!myArray.isEmpty()) {isAnyActionRunning = true;}
    // ДОбавляем новое поле
//    jsnActionList["action_list"] =  myArray; // "testing"; myArray;
//    this->jsnData = merge_json(jsnActionList, jsnHeadStatus);


    // Так вывод фиксированный. А меняться должен через jsnHeadStatus
    // А jsnHeadStatus был изменен в MainProcess::returnActionLaunch

    // Получаем заголовок ответа на запрос статуса
    // Это самый верхний header
    ordered_json header = QtJson_2_NlohmannJson_Head(jsnHeadStatus);

// Заголовок получили, теперь добавляем action_list, снова проходим массив
// каждый элемент преобразуем QJsonObject -> ordered_json

    header["action_list"] =  ordered_json::array({});
    for (int i=0; i < myArray.size(); i++){
        myobj = myArray.at(i).toObject();
        ordered_json actlst = QtJson_2_NlohmannJson_Data(myobj);
        header["action_list"] +=  actlst;
    }

    jsnData = QString::fromStdString(header.dump(3));


    return jsnData;

} //createActionList
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void JsonInfo::SetJsnActionCollapse(QJsonObject &theObj)
{
    this->jsnActionCollapse = theObj;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void JsonInfo::SetJsnActionStandUP(QJsonObject &theObj)
{
    this->jsnActionStandUP = theObj;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Set actual data for the action - it means
// repalce data in array jsnObjArray with the new data from theObj
void JsonInfo::setActionData(QJsonObject &theObj)
{
// Ищем в jsnObjArray по "name" нужный action, меняем его на значения theobj
// Так работаем только с массивом.
    QString theName = theObj.value("name").toString();
    // Проходим jsnObjArray, ищем совпадение по "name", меняем "rc" и "state", делаем replase

    for (int i=0; i < jsnObjArray.size(); i++) {
        if (jsnObjArray.at(i).toObject().value("name").toString() == theName){
            jsnObjArray.replace(i, theObj);
        }
    }
//    jsnActionList["action_list"] = jsnObjArray;
//    this->jsnData = merge_json(jsnActionList, jsnHeadStatus);


} // setActionData
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
