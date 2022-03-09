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

void MainProcess::Data_From_TcpClient_Slot(QString message)
{
//    QByteArray dd ;
    QString str, substr;
    int value = 0xf00f;
    new_get_request = true;
    //str = "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSimpleServer->Info_2_Log_Signal !!!!!!!!!!!!!!!!!!!";
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


    QStringList list1; // Обязательно сдесь, иначе switch не скомпилируется, внутри switch нельзя объявить.

    // А можно же в ProcessAction передавать сам comIndex и тогда switch не нужен ?!
    // Не совсем... у разных экшенов все-таки разная обработка...

    switch (comIndex) {

        case 0: //"clamp"
                on_clampButton_clicked();
        break;

        case 1: //"get_box"  - это экшен (к вопросу о типе)
                // Поэтому Process Action
                ProcessAction(&Robot->getbox_Action);
        break;
        case 2: //"parking"
                ProcessAction(&Robot->parking_Action);

        break;


        case 3: //"ready"
                ProcessAction(&Robot->ready_Action);


        break;
        case 7: //"setservos="
                substr = substr.remove("setservos=");
                list1 = substr.split(QLatin1Char(','));
                for (int i=0; i<DOF; ++i)
                {
                    Servos[i] = list1.at(i).toUInt();
                }//for

                this->send_Data(NOT_LAST);
                jsnStatusActionAnswer["state"] = "running";

        break;

        case 9: //"get_box" в новом формате - это экшен (к вопросу о типе) "status?action=get_box"
                // Поэтому Process Action
                ProcessAction(&Robot->getbox_Action);
        break;

        case 10: //"formoving"
//                memcpy(Servos, mob_moving_position, DOF);
//                this->send_Data(LASTONE);
                ProcessAction(&Robot->forMoving_Action);
        break; //"put_box"

        case 11: //"put_box" в новом формате - это экшен (к вопросу о типе) "status?action=get_box"
                // Поэтому Process Action
                ProcessAction(&Robot->putbox_Action);
        break; //"reset"
        case 12: //"reset" в новом формате - это экшен (к вопросу о типе) "status?action=get_box"
                 // НО, т.к. сбрасываем текущую, то без ProcessAction, ставим всем командам RC=-4
                Robot->getbox_Action.rc = -4;
                Robot->putbox_Action.rc = -4;
                Robot->parking_Action.rc = -4;
                Robot->ready_Action.rc = -4;
                Robot->forMoving_Action.rc = -4;


        break; //




        default:
                  ;
        break;


    }//switch (comIndex)



//============================ start
        if (substr == "start") {
            Robot->SetCurrentStatus ("init"); // Перед запуском распознавания

            // Движение только начинаем, поэтому обнулим значение LASTONE
            emit on_trainButton_clicked ();
            str = "Robot current status is ";
            str += Robot->current_status;
            Robot->Write_To_Log(0xf00F, str);

            str = Robot->current_status;
            emit Write_2_TcpClient_Signal (str);
         }
//============================ reset
        if (substr == "reset") {
            if (Robot->GetCurrentStatus () != "wait"){
                Robot->SetCurrentStatus ("wait");
                str = "Robot changed status, now it is : ";
                str += Robot->current_status;

                GUI_Write_To_Log (value, str);
                str = Robot->current_status;
                //str = "status_from_robot";
                emit Write_2_TcpClient_Signal (str);
            }
         }
//         ///run?cmd=status&123

//============================ status
   if (substr == "status") {
      // str  = "{\n\t\"status\":\"";
       Robot->current_status = Robot->statuslst.at(Robot->current_st_index);
       str = Robot->current_status;
       jsnStatus["state"] = str.toStdString();
       jsnStatus["rc"] = RC_SUCCESS;
       //jsnStatus[""]

       // serialization with pretty printing
       // pass in the amount of spaces to indent
       int indent = 3;
       std::string s2 = jsnStatus.dump(indent);

       GUI_Write_To_Log(value, "!!!!!!!!!!! Current STATUS is ");
       GUI_Write_To_Log(value, QString::fromStdString(s2));

       str = QString::fromStdString(s2);
      // str = QJsonDocument(jsnStatus).toJson(QJsonDocument::Compact);

       emit Write_2_TcpClient_Signal (str);
   }
//============================ sit
   if (substr == "sit") {

       QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(sit_down_position), 6);
       dd.append(0x31); // Движение "Туда"
       Robot->GoToPosition(dd);//, sit_down_position
   }//"sit"

   if (substr == "standup") {
       QByteArray dd = QByteArray::fromRawData(reinterpret_cast<const char*>(hwr_Start_position), 6);
       dd.append(0x30); // Движение "Обратно"
       dd.append(LASTONE);
       Robot->GoToPosition(dd);//, hwr_Start_position

   }

//   if (substr == "clamp") { on_clampButton_clicked();}//"sit"

//   if (substr == "parking")
//   {
//       // str = "Before parking memcpy "; Servos_To_Log(str);
//        memcpy(Servos, mob_parking_position, DOF);
//        //str = "After parking memcpy "; Servos_To_Log(str);
//        this->send_Data(LASTONE);
//   }


//   if (substr == "ready")
//   {
//       memcpy(Servos, mob_ready_position, DOF);
//       this->send_Data(LASTONE);
//   }

   if (substr == "servo2_20")
   {
       Servos[1]=20;
       QByteArray dd ;
       dd.resize(parcel_size);
       memcpy(dd.data(), Servos, DOF);
       dd.insert(6, 0x31); // Движение "Туда"
       Robot->GoToPosition(dd);

   }
   if (substr == "servo2_90")
   {
       Servos[1]=90;
       QByteArray dd ;
       dd.resize(parcel_size);
       memcpy(dd.data(), Servos, DOF);
       dd.insert(6, 0x31); // Движение "Туда"
       Robot->GoToPosition(dd);

   }

   if (substr == "pos_11") { memcpy(Servos, mob_pos_11, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_12") { memcpy(Servos, mob_pos_12, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_13") { memcpy(Servos, mob_pos_13, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_14") { memcpy(Servos, mob_pos_14, DOF);  this->send_Data(LASTONE); }

   if (substr == "pos_15") { memcpy(Servos, mob_pos_15, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_16") { memcpy(Servos, mob_pos_16, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_17") { memcpy(Servos, mob_pos_17, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_18") { memcpy(Servos, mob_pos_18, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_19") { memcpy(Servos, mob_pos_19, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_20") { memcpy(Servos, mob_pos_20, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_21") { memcpy(Servos, mob_pos_21, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_22") { memcpy(Servos, mob_pos_22, DOF);  this->send_Data(LASTONE); }
   if (substr == "pos_23") { memcpy(Servos, mob_pos_23, DOF);  this->send_Data(LASTONE); }


//++++++++++++++++++ Если команда длинная, а для распознавания
// достаточно первые несколько символов

//   if (substr.startsWith("setservos=")){
//       substr = substr.remove("setservos=");
//       QStringList list1 = substr.split(QLatin1Char(','));
//       for (int i=0; i<DOF; ++i)
//       {
//           Servos[i] = list1.at(i).toUInt();
//       }//for

//       this->send_Data(NOT_LAST);
//       jsnStatusActionAnswer["state"] = "running";
//   }
//+++++++++++++++++++ action  "get_box" ++++++++++++++++++++++++++++++++++++++++++++
//   if (substr == "get_box") {
////       jsn_answer_info = Robot->current_status;
////       str = "Current status value is ";
////       str += jsn_answer_info;
////       GUI_Write_To_Log(value, str);
////       // Проверяем статус, не запущен ли уже такой action ?
////       if (Robot->current_status == "inprogress"){jsn_answer_rc = -3;}
////       else{
////           Robot->current_status = "inprogress";
////           jsn_answer_rc = 0;
////           jsn_answer_info = "Action started";
////           jsn_answer_name = "get_box";
////       }

//       // Этот же ответ при конкретном запросе статуса экшена "get_box"
//         switch (Robot->getbox_Action.rc)
//        {

//            case -4: // (ожидание) -> Запускаем
//             // А вот тут можно найти индекс этой команды в списке и присвоить
//             // Переменной HiWonder::active_command, тогда не надо держать
//             // в голове значения индексов
//               Robot->active_command = Robot->actionlst.at(0);
//               Robot->getbox_Action.rc = 0;
//               str = "Action "; str += substr; str += "Успешно запущен";

//               // Заносим данные в структуру
//               Robot->getbox_Action = {"get_box", 0, "In progress"};
//               // И еще в структуру для "status?action=getbox"

//            break;

//            case 0: // (уже запущен) -> Выходим
//                Robot->getbox_Action.rc = -3;
//                str = "Action "; str += substr; str += "Уже запущен";
//                // Заносим данные в структуру
//                Robot->getbox_Action = {"get_box", -3, "Already In progress"};

//            break;

//            case -3: // (уже запущен) -> Выходим

//              str = "Action "; str += substr; str += "Уже запущен";
//              Robot->getbox_Action = {"get_box", -3, "Already In progress"};
//            break;

//            case -2: // (не запустился) -> Выходим

//                str = "Action "; str += substr; str += "Не запустился"; // Serial PORT Error
//                // - Проверяем октрытие SerialPort
//                Robot->getbox_Action = {"get_box", -2, "Failed"};
//            break;
//            default:
//                Robot->getbox_Action.rc = -4;
//                str = "Action "; str += substr; str += "В ожидании";
//            break;


//        }
//        GUI_Write_To_Log(value, str);


        // Фиксируем время начала выполнения.
//        QDateTime dt(QDateTime::currentDateTime());
//        QString st_time = QString::number(dt.toSecsSinceEpoch());

       // Создаем сокет для связи с камерой и, в случае успеха, отправляем запрос в камеру.
       // В ответе будет значение distance, которое сохраняем в глобальной переменной CVDistance
       // По завершении request_CV получаем объект QJsonObject   jsndataObj, из которого извлекаем distance.



//       request_CV();
//       // Запускаем захват объекта.  Теперь это значение distance отправляем в ф-цию GetBox
//       this->GetBox(CVDistance);
       //Команду манипулятору запустили. Задаем статус для ответа http-клиенту через структуру HiWonder::ActionState .

//       Robot->getbox_Action.rc = 0;
//       Robot->getbox_Action.info = "Is running";


//       Robot->getbox_Action.rc = 0;
//       QJsonValue myvalue;

       // Заносим данные в структуру
       //Robot->getbox_Action = {"get_box", 0, "In progress"};

       //myvalue = Robot->getbox_Action.name;
      // jsnActionAnswer.insert("name", myvalue);

       // А из структуры - в JSON-объект
//       jsnActionAnswer.insert("name", QJsonValue(Robot->getbox_Action.name));
//       jsnActionAnswer.insert("rc", QJsonValue(Robot->getbox_Action.rc));
//       jsnActionAnswer.insert("info", QJsonValue(Robot->getbox_Action.info));

//       // И теперь вот этот jsnActionAnswer отправляем http-клиенту в ответ на команду "get_box"

//       jsnDoc = QJsonDocument(jsnActionAnswer);

//       str = jsnDoc.toJson(QJsonDocument::Compact);

//       GUI_Write_To_Log(value, "!!!!!!!!!!! Current Answer to GetBox command is ");
//       GUI_Write_To_Log(value, str);

       //str = QString::fromStdString(s2);
      // str = QJsonDocument(jsnStatus).toJson(QJsonDocument::Compact);

//       emit Write_2_TcpClient_Signal (str);


 //  }//substr == "get_box"

//+++++++++++++++++++ action  "srvfromfile" +++++
   // Читаем построчно файл со значениями сервоприводов
   if (substr == "srvfromfile") {
       ;
   }//substr == "srvfromfile"


//++++++++++++++++   Запрос информации из service

   if (substr == "getservices") {

       int indent = 3;
       std::string s2 = jsnGetServicesAnswer.dump(indent);
       str = QString::fromStdString(s2);
       GUI_Write_To_Log(value, "!!!!!!!!!!! Current Services LIST is ");
       GUI_Write_To_Log(value, str);

       emit Write_2_TcpClient_Signal (str);

   }//substr == "getservices"
//++++++++++++++ /service?name=getactions
   if (substr == "getactions") {

       int indent = 3;
       std::string s2 = jsnGetActionsAnswer.dump(indent);
       str = QString::fromStdString(s2);
       GUI_Write_To_Log(value, "!!!!!!!!!!! Current Actions LIST is ");
       GUI_Write_To_Log(value, str);

       emit Write_2_TcpClient_Signal (str);

   }//substr == "getactions"
//+++++++++++++++++++++++++++++++++++++++++++++    /status?action=get_box
// Вот тут задаем статус {"waiting","noDetection", "inprogress", "done", "fail"}
  // if (substr == "status?action=getbox"){;}






}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MainProcess::Data_from_TcpServer_Slot(QString tcpData)
{
     int value = 0x1111;
  GUI_Write_To_Log(value, "!!!!!! Here is Data From CV device !!!!!!!!!!");
  GUI_Write_To_Log(value,tcpData);

}//Data_From_TcpClient_Slot
//++++++++++++++++++++++++++++++++++++++++++++++++++++



