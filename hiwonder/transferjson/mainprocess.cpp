#include "mainprocess.h"

MainProcess::MainProcess(QObject *parent) : QObject(parent)
{
    jsnStore = new  JsonInfo();
    mainjsnObj = jsnStore->returnJsnActionReset(); //set global value

    QString str;


    qDebug() << "mainjsnObj value After initialize : \n" << QJsonDocument(mainjsnObj).toJson(QJsonDocument::Indented);
    mainjsnObj = jsnStore->returnJsnActionCollapse();  // returns QJsonObject&
    qDebug() << "mainjsnObj value After assigning new value : \n" << QJsonDocument(mainjsnObj).toJson(QJsonDocument::Indented);
    ProcessAction(-4, mainjsnObj);

    // В этой точке значение jsnActionCollapse уже изменено внтури ProcessAction
    str = QJsonDocument(jsnStore->returnJsnActionCollapse()).toJson(QJsonDocument::Indented);
    qDebug() << "!!!!!!!!!!!!!!! Current ActionCollapse value :\n" << str;

    // Также изменено jsnStore->jsnActionList, смотрим
    str += QJsonDocument(jsnStore->jsnActionList).toJson(QJsonDocument::Indented);
    qDebug() << "!!!!!!!!!!!!!!! Current jsnActionList value :\n" << str;

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
MainProcess::~MainProcess()
{
    delete jsnStore;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++

void MainProcess::ProcessAction(int indexOfCommand, QJsonObject &theObj)
{
    QString str;
    int returnCode = theObj.value("rc").toInt() ;
    switch (returnCode) {

    case -4: // Выполнен ? "waiting" ? тогда запускаем
        theObj["rc"] = RC_SUCCESS; // Now state "inprogress" //theObj
        theObj["state"] = DEV_ACTION_STATE_RUN;

        qDebug() << "ProcessAction-> theObj After assigning new value : \n" << QJsonDocument(theObj).toJson(QJsonDocument::Indented);
        qDebug() << "ProcessAction-> mainjsnObj just for test : \n" << QJsonDocument(mainjsnObj).toJson(QJsonDocument::Indented);


        launchActionAnswer["name"] = theObj.value("name").toString();
        launchActionAnswer["rc"] = RC_SUCCESS;
        launchActionAnswer["info"] = DEV_HEAD_INFO_REQUEST;
        str = QJsonDocument(launchActionAnswer).toJson(QJsonDocument::Indented);
        qDebug() << "ProcessAction-> launchActionAnswer After assigning new value : \n" << str;

        QJsonObject myObj = jsnStore->returnJsnActionCollapse();
        str = QJsonDocument(myObj).toJson(QJsonDocument::Indented);
        qDebug() << "ProcessAction-> ActionCollapse as myObj current value : \n" << str;

        str = QJsonDocument(jsnStore->returnJsnActionCollapse()).toJson(QJsonDocument::Indented);
        qDebug() << "ProcessAction-> ActionCollapse as returnJsnActionCollapse() current value : \n" << str;

        jsnStore->SetJsnActionCollapse(theObj); // jsnActionCollapse = theObj;

        theObj["state"] = DEV_ACTION_STATE_RUN;
        theObj["rc"] = RC_SUCCESS;
        QJsonValue myjsnValue = QJsonValue(theObj);
        jsnStore->jsnObjArray.replace(1, myjsnValue);
        jsnStore->jsnActionList = {
            {"action_list", jsnStore->jsnObjArray} //jsnArray
        };



        break;


    } // switch (returnCode)


}
