#include "mainprocess.h"
#include "positions.h"
//#include "tcpParcing.h"

/*
преобразование строки параметров
?a=12&a=12432354&b=123&c=1241234
в массив ['a', '12'] ['a', '12432354'] ['b', '123'] ['c', '1241234']
        lst = str(request.args)
        lst2 = lst.find('[')
        a = lst[lst2:-1].replace('\'','\"').replace('(','[').replace(')',']')
        aa = json.loads(a)
*/
//+++++++ Получили данные (запрос) от клиента. Парсим.
//   0          1                    3                      5                           7                                   9
//{"clamp", "get_box", "parking", "ready", "status", "getactions", "getservices", "setservos=", "srvfromfile",  "status?action=get_box", "formoving"};


// Делаем временное решение.
// ProcessAction запускаем только для значений индекса команды 0 или 1.
// Для остальных значений, оставляем как было. Только в новом файле теперь.

void MainProcess::Data_From_TcpClient_Slot(QString message, int socketNumber, QObject* theSender)
{
    QString str, substr;
    int value = 0xf00f;

//    QMutexLocker locker(&mutex);

    ptrTcpClient = theSender;
    this->tcpSocketNumber =  socketNumber;
    new_get_request = true;
    str = "From TCP Get new command : "; str += message;
    GUI_Write_To_Log(value, str);

    str = "$$$$$$$$$$$$$$$$$$$$$$ <Data_From_TcpClient_Slot>'s thread is ";
    //str += QString("0x%1").arg((qintptr)this->QObject::thread(),8);
    str += pointer_to_qstring(QObject::thread());
    GUI_Write_To_Log(value, str);

    QString mystr = "Pointer to SENDER QObject is ";
    mystr += pointer_to_qstring(ptrTcpClient);
    GUI_Write_To_Log(value, mystr);



    substr = message;

    //mutex.lock();
    int comIndex = getIndexCommand(substr, tcpCommand);


    // So here we've got the index of elemnt representing the command received by TCP
    //jsnStore->createActionList();
    //mutex.unlock();

    // And now make decision
    // Если не запрос статуса и нет активных экшенов, меняем active_command, идем дальше
    // active_command != экшен

//    if (comIndex > 0 && !(jsnStore->isAnyActionRunning)){
//        //mutex.lock();
//        Robot->active_command = tcpCommand.at(comIndex);

//        //mutex.unlock();
//        str = "Current active command is ";
//        str += Robot->active_command;
//        str += " and isAnyActionRunning is ";
//        str += QVariant(jsnStore->isAnyActionRunning).toString();

//        GUI_Write_To_Log(value, str);

//    }

    // Если не запрос статуса, И не reset, но есть активный экшен (еще вопрос - тот же самый или другой ?)
    // Меняем у запрошенного, - если не текущий, - экшена статус - на "не запустился"
    // Другими словами - во время выполняения экшена приходит команда выполнить другой экшен...
    // Манипулятор не может 2 экшена выполнять одновременно.
    // comIndex > 0 && comIndex != 1
    // #################### ВНИМАНИЕ !!! ТУТ НЕТ ЗАПИСИ В ЛОГ, ЧТОБ БЫСТРЕЕ БЫЛО #########
    if (comIndex > 1 && jsnStore->isAnyActionRunning ){
         QJsonObject tempObj;
                     tempObj = {
                         {"name", substr},
                         {"info", "Another action is already running"},
                         {"rc", RC_UNDEFINED}

                     };

        // ставим экшену статус -2... Если это отличный от текущего
        if (comIndex != currentCommandIndex)
        {
//                {"state", "Not applied"},
            tempObj["rc"] = jsnStore->AC_Launch_RC_FAILURE;
         }
        else        //if (comIndex == currentCommandIndex)
        {
            tempObj["info"] = "This action is STILL running";
            tempObj["rc"] = jsnStore->AC_Launch_RC_ALREADY_HAVE; // Уже запущен

        }
        // set aimed action values to tempObj
        // Here we are managed to not getting dangling pointer.
        // Ничего не меняем в состояниях экшена, так проще
//        jsnStore->setActionData(tempObj);
        //launchActionAnswer = tempObj;
        str = QJsonDocument(tempObj).toJson(QJsonDocument::Indented);
        // отправляем ответ на запуск экшена
        //emit Write_2_TcpClient_Signal(str, socketNumber);
        QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, str),
                                  Q_ARG(qintptr, tcpSocketNumber));


        return;
    }



// Если в данный момент есть работающий экшен, то уже нельзя менять значение mainjsnObj...



//    str = "Before switch";
//    str += " and isAnyActionRunning is ";
//    str += QVariant(jsnStore->isAnyActionRunning).toString();

//    GUI_Write_To_Log(value, str);

   //QString S3;

    QJsonObject tempObj;
    switch (comIndex) {
    case 0: // status

        // Нужно добавлять активный экшен, либо пустой список
        jsnStore->createActionList(); // формируем список, записываем данные в jsnData, делаем merge (jsnActionList, jsnHeadStatus)

        //str += QJsonDocument(jsnStore->jsnActionList).toJson(QJsonDocument::Indented);
        str = jsnStore->returnJsnData();
        GUI_Write_To_Log(value, str);
        //        str = jsnStore->returnJsnData();
        //        str = "{\n\t\"status\": \"testing\"\t\n}";
        //        s2 = jsnStore->jsnInfo.dump(indent);
        //        str = QString::fromStdString(s2);


                //Robot->Write_To_Log(value, str);


//        Robot->active_command = "status";
//        emit Write_2_TcpClient_Signal (str, this->tcpSocketNumber);
        QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, str),
                                  Q_ARG(qintptr, tcpSocketNumber));



        break;
    case -1: // Unknown Action !!! or Error message
        if (substr.startsWith("socketError")) {
            GUI_Write_To_Log(value, substr);
            return;
        }
        if (substr.startsWith("setservos=")){
               substr = substr.remove("setservos=");
               QStringList list1 = substr.split(QLatin1Char(','));
               for (int i=0; i<DOF; ++i)
               {
                   Servos[i] = list1.at(i).toUInt();
               }//for

               this->send_Data(NOT_LAST);
           }
        else
        {
// Неизвестный экшен не должен выполняться...
//        mainjsnObj = jsnStore->returnJsnActionsUnKnown();
//        mainjsnObj["name"] = message;
//        str = "There is wrong action command : ";
          tempObj = jsnStore->returnJsnActionsUnKnown();
          str = QJsonDocument(tempObj).toJson(QJsonDocument::Indented);
          GUI_Write_To_Log(value, str);
          QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, str),
                                  Q_ARG(qintptr, tcpSocketNumber));
        }
        break;

    case 1:  // reset
        // Robot->active_command = "reset";
        jsnStore->resetAllActions();
        jsnStore->isAnyActionRunning = false;
        //str = "I'm in reset";
        mainjsnObj = jsnStore->returnAllActions();
        str = jsnStore->jsnData;
        QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, str),
                                  Q_ARG(qintptr, tcpSocketNumber));


        GUI_Write_To_Log(value, str);
//        mainjsnObj = jsnStore->returnJsnActionReset();
//        str = QJsonDocument(mainjsnObj).toJson(QJsonDocument::Indented);
//        GUI_Write_To_Log(value, str);

//        jsnStore->createActionList(); // формируем список, записываем данные в jsnData, делаем merge (jsnActionList, jsnHeadStatus)
//        str = jsnStore->returnJsnData();
//        GUI_Write_To_Log(value, str);

//        emit Write_2_TcpClient_Signal (str, this->tcpSocketNumber);

//        str = "Current ActionList is :";
//        mainjsnObj = jsnStore->returnAllActions();
//        // Берем значение  jsnActionList, весь ответ не нужен
//        str += QJsonDocument(jsnStore->jsnActionList).toJson(QJsonDocument::Indented);
//        GUI_Write_To_Log(value, str);
        break;

    case 2: // clamp
        // Robot->active_command = "clamp";
        mainjsnObj = jsnStore->returnJsnActionClamp();
//        jsnStore->jsnObjArray[0].toObject()["state"]= "MANYMANY";
//        str = QJsonDocument(jsnStore->jsnObjArray[0].toObject()).toJson(QJsonDocument::Indented);
//        GUI_Write_To_Log(value, str);

//        emit Write_2_TcpClient_Signal (str);
       // ProcessAction(comIndex, mainjsnObj);
        break;
    case 3: // "get_box"
        mainjsnObj = jsnStore->returnJsnActionGetBox();
        ProcessAction(comIndex, mainjsnObj);

        break;


    case 4: // "parking"
        // Robot->active_command = "standup";
        mainjsnObj = jsnStore->returnJsnActionParking();
        ProcessAction(comIndex, mainjsnObj);
        break;
    case 5: // "ready"

        mainjsnObj = jsnStore->returnJsnActionReady();
        ProcessAction(comIndex, mainjsnObj);
        break;
    case 12: // "put_box"
        mainjsnObj = jsnStore->returnJsnActionPutbox();
        ProcessAction(comIndex, mainjsnObj);
       break;

    case 6: //getactions
      //  str = QJsonDocument(jsnStore->returnAllActions()).toJson(QJsonDocument::Indented);
        jsnStore->isAnyActionRunning = false;
        //mainjsnObj = jsnStore->returnAllActions();
        tempObj = jsnStore->returnAllActions();
        str = jsnStore->jsnData;
        QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, str),
                                  Q_ARG(qintptr, tcpSocketNumber));

        str += str.insert(0, "Total action_list : \n");

        GUI_Write_To_Log(value, str);

        break;
    case 11: // "formoving"
        mainjsnObj = jsnStore->returnJsnActionForMoving();
        ProcessAction(comIndex, mainjsnObj);
       break;




    case 13: //lock
        mainjsnObj = jsnStore->returnJsnActionLock();
        ProcessAction(comIndex, mainjsnObj);
       break;
    case 14: //unlock
        mainjsnObj = jsnStore->returnJsnActionUnLock();
        ProcessAction(comIndex, mainjsnObj);
       break;
    case 15: // "detach"
        jsnStore->isAnyActionRunning = false;
        for (int i=0; i<DOF; ++i)
        {
            Servos[i] = 250; //0xFA
        }//for

        this->send_Data(LASTONE); //NOT_LAST

        tempObj = jsnStore->returnJsnActionDetach();
        str = QJsonDocument(tempObj).toJson(QJsonDocument::Indented);
        GUI_Write_To_Log(value, str);

        QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot",
                                Qt::QueuedConnection,
                                Q_ARG(QString, str),
                                Q_ARG(qintptr, tcpSocketNumber));


        tempObj = {
            {"rc", RC_SUCCESS}, //RC_SUCCESS
            {"state", "Running, but all servos are detached"},
            {"info", "Action performing"}
        };

        jsnStore->setJsnHeadStatus(tempObj);


       break;

    case 16: // "attach"
        jsnStore->isAnyActionRunning = false;
        for (int i=0; i<DOF; ++i)
        {
            Servos[i] = 252; //0xFC
        }//for

        this->send_Data(LASTONE); //NOT_LAST
        tempObj = jsnStore->returnJsnActionAttach();
        str = QJsonDocument(tempObj).toJson(QJsonDocument::Indented);
        GUI_Write_To_Log(value, str);
        QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot",
                                Qt::QueuedConnection,
                                Q_ARG(QString, str),
                                Q_ARG(qintptr, tcpSocketNumber));
        tempObj = {
            {"rc", RC_SUCCESS}, //RC_SUCCESS
            {"state", "Running"},
            {"info", "Action performing"}
        };

        jsnStore->setJsnHeadStatus(tempObj);


        break;



//    case 13: //collapse
//        // Robot->active_command = "collapse";
//        mainjsnObj = jsnStore->returnJsnActionCollapse();
//        ProcessAction(comIndex, mainjsnObj);
//        break;

    default:
        str = "The Command with index ";
        str += QString::number(comIndex);
        str += " is not found";
        GUI_Write_To_Log(value, str);
        break;
    }




    // ++++++++++++++++++++++++++++++++ Теперь всё остальное



    if (substr == "close") {
        Robot->serial.close();

    }

    // Запрашиваем в Arduino значения углов сервоприводов
//    if (substr == "getservos") {
//        QByteArray dd ;
//        dd.resize(parcel_size);
//        memcpy(dd.data(), get_values_position, parcel_size);

//    }

    if (substr == "info") {
        // Вот тут делаем присвоение статуса.
        str = Robot->GetCurrentStatus();
        jsnStore->setCurrentAction(substr);

        str = QJsonDocument(jsnStore->returnJsnInfo()).toJson(QJsonDocument::Indented);

        GUI_Write_To_Log(value, "!!!!!!!!!!! Current INFO is ");
        GUI_Write_To_Log(value, str);

        //str = QJsonDocument(jsnStatus).toJson(QJsonDocument::Compact);


        emit Write_2_TcpClient_Signal (str, this->tcpSocketNumber);

    }

    if (substr.startsWith("setservos=")){
           substr = substr.remove("setservos=");
           QStringList list1 = substr.split(QLatin1Char(','));
           for (int i=0; i<DOF; ++i)
           {
               Servos[i] = list1.at(i).toUInt();
           }//for

           this->send_Data(LASTONE); //NOT_LAST
       }

    // Запрашиваем список экшенов
//    if (substr == "getactions") {
//        QByteArray dd ;
//        dd.resize(parcel_size);
//        memcpy(dd.data(), get_values_position, parcel_size);
//     }


}// Data_From_TcpClient_Slot
