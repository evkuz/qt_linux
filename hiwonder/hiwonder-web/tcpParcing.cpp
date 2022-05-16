#include "mainprocess.h"
#include "positions.h"
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

void MainProcess::Data_From_TcpClient_Slot(QString message)
{
    QString str, substr;
    int value = 0xf00f;
    new_get_request = true;
    str = "From TCP Get new command : "; str += message;
    GUI_Write_To_Log(0xf00f, str);

    substr = message;
    // Определяем индекс команды в списке MainProcess::tcpCommand
    int comIndex = getIndexCommand(substr, tcpCommand);
    if (comIndex < 0) {str = "WRONG DATA !!!"; GUI_Write_To_Log(value, str);return;}
    str = "Index value is "; str += QString::number(comIndex); str += ",\n";
    str += "List value on that index is \""; str += tcpCommand.at(comIndex); str += "\"";
    GUI_Write_To_Log(value, str);

    // So here we've got the index of elemnt representing the command received by TCP
    // set value of Robot->active_command
    Robot->active_command = tcpCommand.at(comIndex);
    str = "Current active command is ";
    str += Robot->active_command;
    GUI_Write_To_Log(value, str);

    //jsnStore->action_command = tcpCommand.at(comIndex); // сигнал updateAction_List_Signal
    jsnStore->setCurrentAction(tcpCommand.at(comIndex));
    // 0 - status, 5 - start
//    if (comIndex==0 or comIndex==5 or comIndex==7) {
//        ProcessAction(comIndex);
//    }

    switch (comIndex) {
    case 0: // status
        ;
        break;

    case 1:
        jsnStore->resetAllActions();
        break;

    case 2: // clamp
        ;
        break;

    case 5: // "start"
        ProcessAction(comIndex, jsnStore->returnJsnActionStart());
        break;
    }

    // ++++++++++++++++++++++++++++++++ Теперь всё остальное

    if (substr == "put_box"){
        Robot->SetCurrentStatus ("inprogress"); // Перед запуском распознавания
        put_box();


    }

    if (substr == "reset") {
        if (Robot->GetCurrentStatus () != "wait"){
            Robot->SetCurrentStatus ("wait");
            str = "Robot changed status, now it is : ";
            str += Robot->GetCurrentStatus();

            GUI_Write_To_Log (value, str);
            //str = "status_from_robot";
            str  = "{\n\t\"status\":\"";
            str += Robot->GetCurrentStatus();
            str += "\"\n}";
            emit Write_2_TcpClient_Signal (str);
        }
     }

    if (substr == "sit") {
        // Go to "sit" position
        QByteArray dd;
        dd = QByteArray::fromRawData(reinterpret_cast<const char*>(sit_down_position), 6);
        dd.append(0x31); // Движение "Туда"
        dd.append(LASTONE); // Всегда последнее ?
        Robot->GoToPosition(dd);//, sit_down_position
    }//"sit"

    if (substr == "standup") {
        // Go to Initial "Start" position
        QByteArray dd;
        dd = QByteArray::fromRawData(reinterpret_cast<const char*>(hwr_Start_position), 6);
        dd.append(0x30); // Движение "Обратно"
        dd.append(LASTONE); // Всегда последнее ?
        Robot->GoToPosition(dd);//, hwr_Start_position

    }

    if (substr =="clamp") {

        str = "######## Try to lock/unlock the gripper #########";
        str += "\n";
        str += "Current clamper value is ";
        str += QString::number(Servos[0]);
        if(Servos[0]==0) { Servos[0]=90;}
        else {Servos[0]=0;}
        this->send_Data(LASTONE);
    }//"clamp"

    if (substr =="lock") {

        str = "######## Try to lock the gripper ######### ";
        str += "Current clamper value is ";
        str += QString::number(Servos[0]);
        Servos[0]=90;
        this->send_Data(NOT_LAST); //NOT_LAST LASTONE
    }//"lock"

    if (substr =="unlock") {

        str = "######## Try to UNlock the gripper ######### ";
        str += "Current clamper value is ";
        str += QString::number(Servos[0]);
        Servos[0]=0;
        this->send_Data(NOT_LAST); //NOT_LAST LASTONE
    }//"unlock"

    if (substr == "close") {
        Robot->serial.close();

    }

    // Запрашиваем в Arduino значения углов сервоприводов
    if (substr == "getservos") {
        QByteArray dd ;
        dd.resize(parcel_size);
        memcpy(dd.data(), get_values_position, parcel_size);
        Robot->GoToPosition(dd);

    }

    if (substr == "info") {
        // Вот тут делаем присвоение статуса.
        str = Robot->GetCurrentStatus();
        jsnStore->setCurrentAction(substr);

        str = QJsonDocument(jsnStore->returnJsnInfo()).toJson(QJsonDocument::Indented);

        GUI_Write_To_Log(value, "!!!!!!!!!!! Current INFO is ");
        GUI_Write_To_Log(value, str);

        //str = QJsonDocument(jsnStatus).toJson(QJsonDocument::Compact);


        emit Write_2_TcpClient_Signal (str);

    }


}// Data_From_TcpClient_Slot
