#include "jsoninfo.h"

JsonInfo::JsonInfo()
{
    currentStatus = {DEV_NAME, RC_SUCCESS,  "OK", DEV_HEAD_STATE_WAIT}; // Инициализируем структуру
    action_command = "nothing";
//    struc_2_json(jsnOB1, currentStatus); // Инициализируем  jsnOB1 данными из структуры выше
    init_json();
    init_actions();
    isAnyActionRunning = false;
}
//+++++++++++++++++++++


void JsonInfo::init_json()
{

    jsnActionTST = {
        {"action_clamp", {
             {
                 {"name", "clamp"},
                 {"state", "inprogress | done | fail"},
                 {"info", "Open/Close clamper"},
                 {"rc", RC_SUCCESS} // "int - action return code"
             }
          }//list
        }//action_clamp
     };
// nlohmann::json -> QString -> QJsonDocument -> QJsonObject
    s1 = jsnActionTST.dump(3);
    jsnData = QString::fromStdString(s1);
    jsnDoc = QJsonDocument::fromJson(jsnData.toUtf8(), &jsonError);
//    if(jsonError.error != QJsonParseError::NoError){
//            str = "Error: "; str += jsonError.errorString();
//            GUI_Write_To_Log(value, str);
//     }       //return;

    //Get the main JSON object and get the data in it
    jsnObj2 = jsnDoc.object();
//++++++++++++++++++++++++++++++++++++++++++++++



// JSON-объект - это всегда пара "ключ" : "значение"
// Здесь в качестве значения - объект, состоящий из списка объектов
    jsnList = {
        {"action_list", {
         {
             {"name", "clamp"},
             {"state", "inprogress | done | fail"},
             {"info", "Open/Close clamper"},
             {"rc", "int - action return code"}
         },
         {
             {"name", "lock"},
             {"state", "inprogress | done | fail"},
             {"info", "Close clamper"},
             {"rc", "int - action return code"}
         },
         {
             {"name", "unlock"},
             {"state", "inprogress | done | fail"},
             {"info", "Open clamper"},
             {"rc", "int - action return code"}
         },
         {
             {"name", "start"},
             {"state", {"waiting","noDetection", "inprogress", "done", "fail"}},
             {"info", "Get the box by clamper, asking CV about distance in advance"},
             {"st_time", "int - timestamp of last start"},
             {"fin_time", "int - timestamp of finish"},
             {"result", "int - action return code"}
         },
         {
             {"name", "reset"},
             {"state", "succsess | fail"},
             {"info", "Set device status as <Wait>"},
             {"rc", "int - action return code"}
         },
         {
             {"name", "sit"},
             {"state", "inprogress | done | fail"},
             {"info", "Sit position"},
             {"rc", "int - action return code"}
         },
         {
            {"name", "standup"},
            {"state", "inprogress | done | fail"},
            {"info", "Go to start (initital) position"},
            {"rc", "int - action return code"}
         },
         {
            {"name", "put_box"},
            {"state", "inprogress | done | fail"},
            {"info", "put down the object for next actions"},
            {"rc", "int - action return code"}
         },
         {
            {"name", "close"},
            {"state", "inprogress | done | fail"},
            {"info", "Close serial port of the robot"},
            {"rc", "int - action return code"}
         },
         {
            {"name", "getservos"},
            {"state", "inprogress | done | fail"},
            {"info", "Return device' servos values"},
            {"rc", "int - action return code"}
         },
         {
            {"name", "info"},
            {"state", "inprogress | done | fail"},
            {"info", "Print this page"},
            {"rc", "int - action return code"}
         }
             }// json-object
       }//action_list
     }; //list

//  Ответ на команду Action в формате json
    jsnStatus = {
        {"name", DEV_NAME},
        {"rc", RC_UNDEFINED}, //RC_SUCCESS
        {"state", "Wait"},
        {"info", "Request Accepted"}
/*        {"action_list", {
             {
              {"name", "start"},
              {"state", {"running","waiting","noDetection", "inprogress", "done", "fail"}},
              {"info", "Get the box by clamper, asking CV about distance in advance"},
              {"st_time", "int - timestamp of last start"},
              {"fin_time", "int - timestamp of finish"},
              {"result", "int - action return code"}
             },
              {
               {"name", "reset"},
               {"state", "succsess | fail"},
               {"info", "Set device status as <Wait>"},
               {"rc", "int - action return code"}
              }
         } //list
       }//action_list-field
*/

    }; //jsnStatus



    jsnInfo = {
       {"name", DEV_NAME},
       {"rc", RC_UNDEFINED}, //RC_SUCCESS
       {"info", "Request Accepted"},
       {"state", "Wait | Running | Fail"},
       {"action_list", {
          {
           {"name", "clamp"},
           {"state", "inprogress | done | fail"},
           {"info", "Open/Close clamper"},
           {"rc", "int - action return code"}
          },
          {
            {"name", "lock"},
            {"state", "inprogress | done | fail"},
            {"info", "Close clamper"},
            {"rc", "int - action return code"}
          },
          {
            {"name", "unlock"},
            {"state", "inprogress | done | fail"},
            {"info", "Open clamper"},
            {"rc", "int - action return code"}
          },
          {
           {"name", "start"},
           {"state", {"waiting","noDetection", "inprogress", "done", "fail"}},
           {"info", "Get the box by clamper, asking CV about distance in advance"},
           {"st_time", "int - timestamp of last start"},
           {"fin_time", "int - timestamp of finish"},
           {"result", "int - action return code"}
          },
           {
            {"name", "reset"},
            {"state", "succsess | fail"},
            {"info", "Set device status as <Wait>"},
            {"rc", "int - action return code"}
           },
           {
            {"name", "sit"},
            {"state", "inprogress | done | fail"},
            {"info", "Sit position"},
            {"rc", "int - action return code"}
           },
            {
             {"name", "standup"},
             {"state", "inprogress | done | fail"},
             {"info", "Go to start (initital) position"},
             {"rc", "int - action return code"}
            },
            {
             {"name", "put_box"},
             {"state", "inprogress | done | fail"},
             {"info", "put down the object for next actions"},
             {"rc", "int - action return code"}
            },
            {
             {"name", "close"},
             {"state", "inprogress | done | fail"},
             {"info", "Close serial port of the robot"},
             {"rc", "int - action return code"}
            },
            {
             {"name", "getservos"},
             {"state", "inprogress | done | fail"},
             {"info", "Return device' servos values"},
             {"rc", "int - action return code"}
             },
            {
             {"name", "info"},
             {"state", "inprogress | done | fail"},
             {"info", "Print this page"},
             {"rc", "int - action return code"}
            }

          } //list
        }//action_list-field

    }; // jsnInfo

    // Это просто список без значений.
    jsnGetActionsAnswer = {
                            {"name" , "getactions"},
                            {"rc", RC_UNDEFINED}, //RC_SUCCESS
                            {"info" , "Request Accepted"},
        {"data", {
             {
              {"name", "clamp"},
              {"state", "inprogress | done | fail"},
              {"info", "Open/Close clamper"},
              {"rc", "int - action return code"}
             },
             {
               {"name", "lock"},
               {"state", "inprogress | done | fail"},
               {"info", "Close clamper"},
               {"rc", "int - action return code"}
             },
             {
               {"name", "unlock"},
               {"state", "inprogress | done | fail"},
               {"info", "Open clamper"},
               {"rc", "int - action return code"}
             },
             {
              {"name", "start"},
              {"state", {"waiting","noDetection", "inprogress", "done", "fail"}},
              {"info", "Get the box by clamper, asking CV about distance in advance"},
              {"st_time", "int - timestamp of last start"},
              {"fin_time", "int - timestamp of finish"},
              {"result", "int - action return code"}
             },
              {
               {"name", "reset"},
               {"state", "succsess | fail"},
               {"info", "Set device status as <Wait>"},
               {"rc", "int - action return code"}
              },
              {
               {"name", "sit"},
               {"state", "inprogress | done | fail"},
               {"info", "Sit position"},
               {"rc", "int - action return code"}
              },
               {
                {"name", "standup"},
                {"state", "inprogress | done | fail"},
                {"info", "Go to start (initital) position"},
                {"rc", "int - action return code"}
               },
               {
                {"name", "put_box"},
                {"state", "inprogress | done | fail"},
                {"info", "put down the object for next actions"},
                {"rc", "int - action return code"}
               },
               {
                {"name", "close"},
                {"state", "inprogress | done | fail"},
                {"info", "Close serial port of the robot"},
                {"rc", "int - action return code"}
               },
               {
                {"name", "getservos"},
                {"state", "inprogress | done | fail"},
                {"info", "Return device' servos values. Can be viewed in Log file"},
                {"rc", "int - action return code"}
                },
               {
                {"name", "info"},
                {"state", "inprogress | done | fail"},
                {"info", "Print this page"},
                {"rc", "int - action return code"}
               },
         }//list
        }//data
       };//jsnGetActionsAnswer

   // jsnAction = jsnGetActionsAnswer;
    int indent = 3;
    std::string s2 = jsnOB1.dump(indent);
    jsnData = QString::fromStdString(s2);
    jsnDoc = QJsonDocument::fromJson(jsnData.toUtf8(), &jsonError);
//    if(jsonError.error != QJsonParseError::NoError){
//            str = "Error: "; str += jsonError.errorString();
//            GUI_Write_To_Log(value, str);
//     }       //return;

    //Get the main JSON object and get the data in it
    jsnObj = jsnDoc.object();

//+++++++++++++++++++++++++++++
//jsnActionClamp.insert("name", "myname");
// init actionList
//    for (int i=0; i < structActionList.size(); i++){
//        structActionList.at(i).name = "myname";


//    }
//++++++++++++++++++++++++++++++++
// Инициализируем this->action_command крайним значением из списка
    //    setCurrentAction(action_lst.at(action_lst.size()-1));
}
//++++++++++++++++++++++++++++++++

void JsonInfo::init_actions()
{
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    jsnActionClamp = {
        {"name", "clamp"},
        {"state", "waiting"},
        {"info", "Open/Close clamper"},
        {"rc", RC_WAIT} // "int - action return code"
     };

    jsnActionStart = {
        {"name", "start"},
        {"state", "waiting"},
        {"info", "Get the box by clamper, asking CV about distance in advance"},
        {"rc", RC_WAIT}
      };

    jsnActionPutbox = {
        {"name", "put_box"},
        {"state", "done"},
        {"info", "put down the object for next actions"},
        {"rc", RC_WAIT}
       };

    jsnActionReset = {
        {"name", "reset"},
        {"state", DEV_ACTION_STATE_DONE},
        {"info", "Set device status as <Wait> in main Header"},
        {"rc",  RC_WAIT}
    };

    jsnActionCollapse = {
        {"name", "collapse"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "make long time opeartion for test"},
        {"rc",  RC_WAIT}
    };

    jsnActionStandUP = {
        {"name", "standup"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "Go to start (initital) position"},
        {"rc", RC_WAIT}
    };

    jsnActionUnKnown = {
        {"name", "UnKnown"},
        {"state", DEV_ACTION_STATE_FAIL},
        {"info", "There is now action with such a name"},
        {"rc", AC_WRONG_VALUE}
    };
    jsnActionLock = {
        {"name", "lock"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "lock the gripper of manipulator"},
        {"rc",  this->AC_Launch_RC_DONE}
    };
    jsnActionUnLock = {
        {"name", "unlock"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "unlock the gripper of manipulator"},
        {"rc",  this->AC_Launch_RC_DONE}
    };


//    actionListp = {jsnActionClamp, jsnActionStart, jsnActionPutbox, jsnActionReset, jsnActionCollapse};
    jsnObjArray = {jsnActionClamp, jsnActionCollapse, jsnActionStandUP, jsnActionReset, jsnActionStart, jsnActionPutbox, \
                   jsnActionUnKnown, jsnActionLock, jsnActionUnLock}; //
    jsnHeadStatus = {
        {"name", DEV_NAME},
        {"rc", RC_UNDEFINED}, //RC_SUCCESS
        {"state", "Wait"},
        {"info", "Request Accepted"}
    };

    jsnArray = {};
    jsnActionList = {
        {"action_list", jsnObjArray} //jsnArray
    };



}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Тут разные списки - action_lst и actionListp. Следует учитывать.
void JsonInfo::resetAllActions()
{
//    QJsonObject myObject;
    for (int i=0; i < jsnObjArray.size() ; i++) //action_lst.size()
    {
       jsnObjArray.at(i).toObject()["rc"] = this->AC_Launch_RC_DONE;
       jsnObjArray.at(i).toObject()["state"] = "done";
    }
    isAnyActionRunning = false; // Нет активных экшенов

}///init_json
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
}// makeJson_Answer_Slot
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
//+++++++++++++++++++++++++++++++++++++++++++

QJsonObject JsonInfo::returnAllActions()
{
    QJsonValue myValue;
//    bool OK = this->eraseArray(this->jsnObjArray);

//    for (int i = 0; i< actionListp.size(); i++){
//        myValue = actionListp.at(i);
//        jsnArray.append(myValue);
//    } //for

    jsnActionList["action_list"] = jsnObjArray;
    this->jsnData = merge_json(jsnActionList, jsnHeadStatus);
//    OK = this->eraseArray(this->jsnObjArray);
    return this->jsnObj; // Изменено в merge_json


}
//+++++++++++++++++++++++++++++++++++++++++++

void JsonInfo::setActionDone(QJsonObject &theObj)
{
    theObj["rc"] = -4;
    theObj["state"] = "done";

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
//    jsnHeadStatus["info"]  = "No Detection";
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

} // setActionStart2NoDetection
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// fills Up the jsnActionList["action_list"] with active actions only
void JsonInfo::createActionList()
{
    // Перебираем список  jsnObjArray, определяем у кого state == "inprogress"
    // формируем новый список
    // добавляем его в jsnData
    QJsonValue myValue;
    QJsonObject myobj;
    QJsonArray myArray;



   myArray = this->jsnObjArray;
   //myArray.swap(jsnObjArray);
   //jsnObjArray.swap(myArray);
   QString str;
    for (int i=0; i < myArray.size(); i++){
        myobj = myArray.at(i).toObject();
        str = myobj.value("state").toString();
//        если не равно - вынимаем объект из массива, т.е. оставляем только running
//       if (QString::compare(str, DEV_ACTION_STATE_RUN)){
         if (str != DEV_ACTION_STATE_RUN){
             myArray.removeAt(i);
             i--;

       }
    }

    if (!myArray.isEmpty()) {isAnyActionRunning = true;}
    jsnActionList["action_list"] =  myArray; // "testing"; myArray;
    this->jsnData = merge_json(jsnActionList, jsnHeadStatus);
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



}
