#include "jsoninfo.h"

JsonInfo::JsonInfo()
{
    init_actions();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void JsonInfo::init_actions()
{
    jsnActionReset = {
        {"name", "reset"},
        {"state", DEV_ACTION_STATE_DONE},
        {"info", "Set device status as <Wait>"},
        {"rc",  RC_WAIT}
    };

    jsnActionCollapse = {
        {"name", "collapse"},
        {"state", DEV_ACTION_STATE_WAIT},
        {"info", "make long time opeartion for test"},
        {"rc",  RC_WAIT}
    };
    jsnObjArray = {jsnActionReset, jsnActionCollapse};

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QJsonObject JsonInfo::returnJsnActionReset()
{
    return jsnActionReset;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QJsonObject &JsonInfo::returnJsnActionCollapse()
{
    return jsnActionCollapse;
}

void JsonInfo::SetJsnActionCollapse(QJsonObject &theObj)
{
    this->jsnActionCollapse = theObj;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

