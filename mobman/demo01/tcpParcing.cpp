#include "mainprocess.h"
#include "positions.h"

//+++++++ Получили данные (запрос) от клиента. Парсим.
void MainProcess::Data_From_TcpClient_Slot(QString message)
{
    QByteArray dd ;
    QString str, substr;
    int value = 0xf00f;
    new_get_request = true;
    //str = "!!!!!!!!!!!!!!!!!!!!! Get COMMAND FROM QSimpleServer->Info_2_Log_Signal !!!!!!!!!!!!!!!!!!!";
    str = "From TCP Get new command : "; str += message;
    GUI_Write_To_Log(0xf00f, str);

        substr = message;
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
       str = Robot->current_status;
       //std::string s2
       //jsnStatus["state"] = "Wait";
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

//       QDateTime dt(QDateTime::currentDateTime());
//       //dt.toLocalTime();
//       str = "Current SecsSinceEpoch is ";
//       str += QString::number(dt.toSecsSinceEpoch());
//       GUI_Write_To_Log (value, str);
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

   if (substr == "clamp") { on_clampButton_clicked();}//"sit"

   if (substr == "parking")
   {
        //str = "Before parking memcpy "; Servos_To_Log(str);
        memcpy(Servos, mob_parking_position, DOF);
        //str = "After parking memcpy "; Servos_To_Log(str);
        this->send_Data(LASTONE);
   }


   if (substr == "ready")
   {
       memcpy(Servos, mob_ready_position, DOF);
       this->send_Data(LASTONE);
   }

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

   if (substr.startsWith("setservos=")){
       substr = substr.remove("setservos=");
       QStringList list1 = substr.split(QLatin1Char(','));
       for (int i=0; i<DOF; ++i)
       {
           Servos[i] = list1.at(i).toUInt();
       }//for

       this->send_Data(NOT_LAST);
   }
//+++++++++++++++++++ action  "get_box" ++++++++++++++++++++++++++++++++++++++++++++
//int jsn_answer_rc;
//QString jsn_answer_name;
//QString jsn_answer_info;
   if (substr == "get_box") {
//       jsn_answer_info = Robot->current_status;
//       str = "Current status value is ";
//       str += jsn_answer_info;
//       GUI_Write_To_Log(value, str);
//       // Проверяем статус, не запущен ли уже такой action ?
//       if (Robot->current_status == "inprogress"){jsn_answer_rc = -3;}
//       else{
//           Robot->current_status = "inprogress";
//           jsn_answer_rc = 0;
//           jsn_answer_info = "Action started";
//           jsn_answer_name = "get_box";
//       }

       request_CV();
       Robot->getbox.rc = 0;
       Robot->getbox = {"get_box", 0, "In progress"};

   }//substr == "get_box"

//+++++++++++++++++++ action  "srvfromfile" +++++
   // Читаем построчно файл со значениями сервоприводов
   if (substr == "srvfromfile") {
       ;
   }//substr == "srvfromfile"
}
