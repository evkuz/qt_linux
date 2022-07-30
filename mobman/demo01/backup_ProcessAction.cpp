//void MainProcess::ProcessAction(HiWonder::ActionState *actionName)
// ProcessAction
void ProcessAction(int indexOfCommand, QJsonObject &theObj) // Отрабатывает команду по заданному индексу из списка QList<QString> theList
{
    QString str;
    int theindex, value;
    value = 0x1122;
    // Фиксируем время начала выполнения.
            QDateTime dt(QDateTime::currentDateTime());
            QString st_time = QString::number(dt.toSecsSinceEpoch());

    // Этот же ответ при конкретном запросе статуса экшена "get_box"
    // Проверяем текущее значение RC (return code)

    str = "Before switch the actionName->rc value is ";
    str += QString::number(actionName->rc);
    GUI_Write_To_Log(value, str);


    str = "Before switch the Current active command is ";
    str += Robot->active_command;
    str += " And the actionName->name is ";
    str += actionName->name;

    GUI_Write_To_Log(value, str);



      switch (actionName->rc)
     {

         case -4: // (ожидание) -> Запускаем, это "get_box". В списке actionlst это index == 1
          // А вот тут можно найти индекс этой команды в списке и присвоить
          // Переменной HiWonder::active_command, тогда не надо держать
          // в голове значения индексов - т.е. вместо
          // Robot->actionlst.at(0); будет переменная со значеием Robot->actionlst.at(0)

            theindex = getIndexCommand(actionName->name, tcpCommand); // Robot->actionlst => "get_box" ==0
            // Индекс можно передавать как параметр из tcpParsing.
            Robot->active_command = actionName->name; // Robot->actionlst.at(theindex); - вот тут индекс сместился !!!!

            str = "Inside switch case -4:  the Current active command is ";
            str += Robot->active_command;
            GUI_Write_To_Log(value, str);

            // -4 -> 0

            // Заносим данные в структуру ( у каждогоо экшена должна быть своя.)
           // Robot->getbox_Action = {"get_box", 0, "In progress"};
            // И еще в структуру для "status?action=getbox"

            // А теперь все действия, относящиейся к "get_box" или какой там экшен запускается

            // 1. Проверяем, открыт ли SerialPort ? Если нет то
            //    - Пишем в лог.
            //    - Ставим actionName->rc == -2: // (не запустился)
            if (!Robot->SerialIsOpened){
                str = "Serial port is UNAVAILABLE !!! CAN'T move the ARM !!! Action is FAILED !!!";

                actionName->rc = -2;
                actionName->info = str;
                GUI_Write_To_Log(value, str);
            }

            else {
                actionName->rc = 0; // Now state "Is running"
                str = "Action "; str += actionName->name; str += " have started";
                GUI_Write_To_Log(value, str);

                str = "Action "; str += actionName->name; str += " have index "; str += QString::number(theindex);
                GUI_Write_To_Log(value, str);

                // Теперь запускаем манипулятор

                switch (theindex)
               {
                case 1: // "get_box"

                           // Создаем сокет для связи с камерой и, в случае успеха, отправляем запрос в камеру.
                           // В ответе будет значение distance, которое сохраняем в глобальной переменной CVDistance
                           // По завершении request_CV получаем объект QJsonObject   jsndataObj, из которого извлекаем distance.
                           GUI_Write_To_Log(value, "From procesAction before request_CV");
                           request_CV();
                break;
                case 2:  // parking
                           memcpy(Servos, mob_parking_position, DOF);
                           this->send_Data(LASTONE);
                break;

                case 3: //"ready"
                         memcpy(Servos, mob_ready_position, DOF);
                         this->send_Data(LASTONE);
                break;


                case 10:   //"formoving"
                        //memcpy(Servos, mob_moving_position, DOF);
                          memcpy(Servos, mob_2_moving_position, DOF);
                        this->send_Data(LASTONE);
                break;
                case 11:   //"putbox" - раскладываем на 4 команды :
                           // 1. хват в позицию mob_put_23 2. открыть хват 3. Поднять привод [3] 4. в позицию "formoving"

                        //memcpy(Servos, mob_put_23, DOF);
                        memcpy(Servos, mob_2_put_23, DOF);
                        this->send_Data(NOT_LAST);
                        // Так это последняя команда...
                        //on_clampButton_clicked();

                        // Открываем
                        Servos[0] = 45;
                        this->send_Data(NOT_LAST);

                        //Поднимаем крайний привод, чтобы снова не схватить кубик при движении обратно
                        Servos[3] = 65;
                        this->send_Data(NOT_LAST);

                        memcpy(Servos, mob_moving_position, DOF);
                        this->send_Data(LASTONE);

                break;





//           // Запускаем захват объекта.  Теперь это значение distance отправляем в ф-цию GetBox
                 default:
                            ;
                 break;
                } // switch (theindex)



//          this->GetBox(CVDistance);
//           //Команду манипулятору запустили. Задаем статус для ответа http-клиенту через структуру HiWonder::ActionState .
//           // Заносим данные в структуру - где ?
            } //else

//           // А из структуры - в JSON-объект
//           jsnActionAnswer.insert("name", QJsonValue(Robot->getbox_Action.name));
//           jsnActionAnswer.insert("rc", QJsonValue(Robot->getbox_Action.rc));
//           jsnActionAnswer.insert("info", QJsonValue(Robot->getbox_Action.info));

//           // И теперь вот этот jsnActionAnswer отправляем http-клиенту в ответ на команду "get_box"

//           jsnDoc = QJsonDocument(jsnActionAnswer);

//           str = jsnDoc.toJson(QJsonDocument::Compact);

//           GUI_Write_To_Log(value, "!!!!!!!!!!! Current Answer to GetBox command is ");
//           GUI_Write_To_Log(value, str);

//           //str = QString::fromStdString(s2);
//          // str = QJsonDocument(jsnStatus).toJson(QJsonDocument::Compact);

//           emit Write_2_TcpClient_Signal (str);
         break; //case -4:

         case 0: // (уже запущен)=="Is running" -> Выходим

             //actionName->rc = -3;
             str = "Action "; str += actionName->name; str += " is already running";
             // Заносим данные в структуру
             //Robot->getbox_Action = {"get_box", -3, "RC=0, Already In progress"};
             GUI_Write_To_Log(value, str);
             actionName->rc = -3;
             //request_CV();

         break;

         case -3: // (уже запущен) -> Выходим

           str = "Action "; str += actionName->name; str += " is STILL running. Try RESET action";
           // Robot->getbox_Action = {"get_box", -3, "Already In progress"};
           actionName->info =  "Already In progress";
           GUI_Write_To_Log(value, str);

         break;

         case -2: // (не запустился) -> Выходим

             str = "Action "; str += actionName->name; str += "Не запустился"; // Serial PORT Error
             // - Проверяем октрытие SerialPort
             //Robot->getbox_Action = {"get_box", -2, "Failed"};
             actionName->info = "Failed";
         break;

         default:
             actionName->rc = -4;
             str = "Default case : Action "; str += actionName->name; str += " В ожидании";
             GUI_Write_To_Log(value, str);
         break;


     }// switch (actionName->rc)

     str = "The function ProcesAction is finished !!!";
     GUI_Write_To_Log(value, str);

}// ProcessAction
