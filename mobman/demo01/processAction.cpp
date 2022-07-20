#include "mainprocess.h"
#include "positions.h"

//#include "ProcessAction.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++
// - обрабатываем текущий статус экшена
// - запускаем экшен, если надо
//void MainProcess::ProcessAction(int indexOfCommand, QJsonObject &theObj);


void MainProcess::ProcessAction(int indexOfCommand, QJsonObject &theObj)
{
    int value;
    QString str;

    value = 0x1122;
//    str = "I'm in  ProcessAction"; GUI_Write_To_Log(value, str);

// Меняем содержмое theObj в зависимости от rc
//    str = theObj.value("rc").toString();
//    GUI_Write_To_Log(value, str);
    int returnCode = theObj.value("rc").toInt() ;

    switch (returnCode) {

    case 0: //rc==0 (уже запущен)=="Is running" -> Выходим, ничего не меняем
            // Сюда даже не должны попадать, т.к. это отсеивается в tcpparcing.cpp
        str = "Action "; str += theObj.value("name").toString();
        str += " is already running. Should wait it to finish";
        GUI_Write_To_Log(value, str);
        theObj["rc"] = RC_FAIL;
        theObj["state"] = DEV_ACTION_STATE_RUN;
        theObj["info"] = DEV_ACTION_INFO;
    break;

    case -1: // action с таким именем не найден
        str = QJsonDocument(theObj).toJson(QJsonDocument::Indented);
        // Вот это нужно, отправляем ответ на запуск экшена
        emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);
        str.insert(0, "There is wrong action command !!! : \n");
        GUI_Write_To_Log(value, str);
        break;

    case -2: // // action с таким именем не запустился меняем только info
        //str = "Serial port is UNAVAILABLE !!! CAN'T move the ARM !!!";
        str = "Did not started last time. Changing rc to ";
        str += RC_WAIT;
        str += ", so Try again."; //"There is another action running at the moment"
        theObj["info"] = str;
        GUI_Write_To_Log(value, str);
        // change gor next request to be successfull
        theObj["rc"] = RC_WAIT;
        // Так надо ж поменять значение самого экшена в классе JsonInfo !!!
        jsnStore->setActionData(theObj);


//        launchActionAnswer["name"] = theObj.value("name").toString();
//        launchActionAnswer["rc"]   = RC_SUCCESS;
//        launchActionAnswer["info"] = DEV_HEAD_INFO_REQUEST;
        str = QJsonDocument(theObj).toJson(QJsonDocument::Indented);
        // Вот это нужно, отправляем ответ на запуск экшена
        emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);



        break;

    case -3: // (уже запущен) -> Выходим, ничего не меняем
        str = "Action "; str += theObj.value("name").toString();
        str += " is STILL running. Try RESET action or wait for finsih.";
        theObj["info"] = "Already In progress. Try RESET action or wait for finish.";
        break;

    case -4: // Выполнен, т.е. "waiting" ? тогда запускаем

        // 1. Проверяем, открыт ли SerialPort ? Если нет то
        //    - Пишем в лог.
         if (!Robot->SerialIsOpened){
            str = "Serial port is UNAVAILABLE !!! CAN'T move the ARM !!! Action is FAILED !!!";
            theObj["rc"] = RC_FAIL;
            theObj["info"] = str;
            theObj["state"] = DEV_ACTION_STATE_FAIL;
            GUI_Write_To_Log(value, str);
            jsnStore->setActionData(theObj);
        }
        else { // Все нормально, запускаем манипулятор

            jsnStore->isAnyActionRunning = true;
            theObj["rc"] = RC_SUCCESS; // Now state "inprogress" //theObj
            theObj["state"] = DEV_ACTION_STATE_RUN;

            str = QJsonDocument(theObj).toJson(QJsonDocument::Indented);
            // Вот это нужно, отправляем ответ на запуск экшена

            // while detection is not checked don's send that it's ok
            if (indexOfCommand != 5) {
                emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);
                jsnStore->setActionData(theObj);


            }

            QJsonObject headStatus = {
                {"rc", RC_SUCCESS}, //RC_SUCCESS
                {"state", "Running"},
                {"info", "Action performing"}
            };

            jsnStore->setJsnHeadStatus(headStatus);

//            launchActionAnswer["name"] = theObj.value("name").toString();
//            launchActionAnswer["rc"] = RC_SUCCESS;
//            launchActionAnswer["info"] = DEV_HEAD_INFO_REQUEST;
//            str = QJsonDocument(launchActionAnswer).toJson(QJsonDocument::Indented);
//            // Вот это нужно, отправляем ответ на запуск экшена
//            emit Write_2_TcpClient_Signal(str);
//            str.insert(0, "Data supposed to be sent to tcp, but only to log : \n");
//            GUI_Write_To_Log(value, str);

            // Теперь запускаем манипулятор. Определяем команду из списка
            QJsonValue myjsnValue;
            currentCommandIndex = indexOfCommand;
            switch (indexOfCommand) {
//            case 1: //reset, у текущего (всех) action делаем rc = -4
//                jsnStore->setActionDone(theObj);
//                break;

            case 2: // clamp
                if(Servos[0]==0) { Servos[0]=90;}
                else {Servos[0]=0;}
                this->send_Data(LASTONE);
                break;
            case 3: // "get_box"
                request_CV(); //Запросить дистанцию, выставить соответствующие углы серво
                break;
            case 4: // "parking"
                memcpy(Servos, mob_parking_position, DOF);
                this->send_Data(LASTONE);

                str = "Parking ACTION, current theObj value :\n";
                str += QJsonDocument(theObj).toJson(QJsonDocument::Indented);
                GUI_Write_To_Log(value,str);

                break;
            case 5: // "ready"
                // - встаём в исходную точку
//                str = "ProcessAction: Going to move  the cube";
//                GUI_Write_To_Log(value,str);
//                on_trainButton_clicked ();
                memcpy(Servos, mob_ready_position, DOF);
                this->send_Data(LASTONE);

                break;
            case 12: // "put_box"
                str = "ProcessAction: Going to put the box down";
                GUI_Write_To_Log(value,str);
                // раскладываем на 4 команды :
                // 1. хват в позицию mob_put_23
                // 2. открыть хват 3. Поднять привод [3] 4. в позицию "formoving"

                memcpy(Servos, mob_2_put_23, DOF);
                this->send_Data(NOT_LAST);

                // Открываем
                Servos[0] = 45;
                this->send_Data(NOT_LAST);

                //Поднимаем крайний привод, чтобы снова не схватить кубик при движении обратно
                Servos[3] = 65;
                this->send_Data(NOT_LAST);
                // в позицию "formoving"
                memcpy(Servos, mob_moving_position, DOF);
                this->send_Data(LASTONE);

                break;
            case 10: // "lock"
                str = "Going to lock the gripper";
                GUI_Write_To_Log(value,str);
                Servos[0]=90;
                this->send_Data(LASTONE); //NOT_LAST LASTONE

                break;
            case 11: // "unlock"
                str = "Going to unlock the gripper";
                GUI_Write_To_Log(value,str);
                Servos[0]=0;
                this->send_Data(LASTONE); //NOT_LAST LASTONE

                break;


//            case 13: //collapse
//                memcpy(Servos, collapse, DOF);
//                this->send_Data(LASTONE);

                break;
            default: // Нет команды с таким индексом. Дубль из Data_From_TcpClient_Slot, там уже была проверка
                currentCommandIndex = -1;
                str = "ProcessAction:  The Command with index ";
                str += QString::number(indexOfCommand);
                str += " is not found";
                GUI_Write_To_Log(value, str);

            break;
            } // switch (indexOfCommand)
        } //else
        break; // case -4:
    default:
        str = "ProcessAction:  The return Code with index ";
        str += QString::number(returnCode);
        str += " is not found";
        GUI_Write_To_Log(value, str);
    break;


    } // switch (returnCode)

//    str = "PROCESSING ACTION IS FINISHED";
//    GUI_Write_To_Log(value,str);
}// ProcessAction
