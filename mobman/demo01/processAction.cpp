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
    int returnCode = theObj.value("rc").toInt();

    switch (returnCode) {

    case 0: //rc==0 (уже запущен)=="Is running" -> Выходим, ничего не меняем
            // Сюда даже не должны попадать, т.к. это отсеивается в tcpparcing.cpp
        str = "Action "; str += theObj.value("name").toString();
        str += " is already running. Should wait it to finish";
        GUI_Write_To_Log(value, str);
        theObj["rc"] = RC_FAIL;
        theObj["state"] = jsnStore->DEV_ACTION_STATE_RUN;
        theObj["info"] = jsnStore->DEV_ACTION_INFO;
    break;

    case -1: // action с таким именем не найден
        str = QJsonDocument(theObj).toJson(QJsonDocument::Indented);
        // Вот это нужно, отправляем ответ на запуск экшена
        //emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);
        QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot", //this->sender()
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, str),
                                  Q_ARG(qintptr, tcpSocketNumber));

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
        //emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);
        QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot", //this->sender()
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, str),
                                  Q_ARG(qintptr, tcpSocketNumber));



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
            theObj["state"] = jsnStore->DEV_ACTION_STATE_FAIL;
            GUI_Write_To_Log(value, str);
            jsnStore->setActionData(theObj);
        }
        else { // Все нормально, запускаем манипулятор

            jsnStore->isAnyActionRunning = true;
            // Это вместо jsnActionAnswer ?
            theObj["rc"] = jsnStore->AC_Launch_RC_RUNNING;//RC_SUCCESS; // Now state "inprogress" //theObj
            theObj["state"] = jsnStore->DEV_ACTION_STATE_RUN;

            str = QJsonDocument(theObj).toJson(QJsonDocument::Indented);

            // Отправляем ответ на запуск экшена для всех кроме манипулятора, т.к. могут быть проблемы с детекцией, дистанцией и т.д.
            // !!! while detection is not checked dont's send that it's ok !!!
            // Вот тут надо брать значение из словаря/кортежа, а не текстовое...
            if (tcpCommand[indexOfCommand] != "get_box") {
                //emit Write_2_TcpClient_Signal(str, this->tcpSocketNumber);
                QMetaObject::invokeMethod(this->ptrTcpClient, "Data_2_TcpClient_Slot", //this->sender()
                                          Qt::QueuedConnection,
                                          Q_ARG(QString, str),
                                          Q_ARG(qintptr, tcpSocketNumber));

                jsnStore->setActionData(theObj);
            }
// Это общий ответ, индикатор, что девайс вообще жив и на связи.
            QJsonObject headStatus = {
                {"rc", RC_SUCCESS}, //RC_SUCCESS
                {"state", "Running"},
                {"info", "Action performing"}
            };

            jsnStore->setJsnHeadStatus(headStatus);

            // Установлено значение экшена
            // Установлено значение заголовка
            // А вместе они склеиваются в tcpParcing.cpp (Data_From_TcpClient_Slot)
            // И это хороший вопрос, что быстрее - тут склеить в один JsonObject и при запросах статуса его выдавать готовый
            // Или склеивать их каждый раз при запросе статуса ?
            // По логике  - каждый раз склеивать - более время затратно, чем брать готовое...


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

            case 2: // clamp FULL_CLOSED = 80; FULL_OPENED = 35;
                if(Servos[0]<=80) { Servos[0]=80;}
                else {Servos[0]=35;}
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
            case 11: // "formoving"
                str = "Go to <ForMoving> position";
                GUI_Write_To_Log(value,str);
                memcpy(Servos, mob_2_moving_position, DOF);
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


                // в позицию "formoving", хват постепенно закрывается
                memcpy(Servos, mob_moving_position, DOF);
                this->send_Data(LASTONE);

                break;
            case 13: // "lock"
                str = "Going to lock the gripper";
                Robot->Write_To_Log(value,str);

                Servos[0]=FULL_CLOSED;
                this->send_Data(LASTONE); //NOT_LAST LASTONE

                break;
            case 14: // "unlock"
                str = "Going to UNlock the gripper";
                Robot->Write_To_Log(value,str);
                Servos[0]=FULL_OPENED;
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
}

void MainProcess::LogFile_Open(QString fname)
{
    mobWebLogFile.setFileName(fname);
    mobWebLogFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
}// ProcessAction
