#include "jsoninfo.h"

JsonInfo::JsonInfo()
{
    currentStatus = {DEV_NAME, RC_SUCCESS,  "OK", DEV_STATE_WAIT, ""};
    action_command = "nothing";
}
//+++++++++++++++++++++


void JsonInfo::init_json()
{

//    jsnList = {
//        {"action_list", {
//             {
//                 {"name", "clamp"},
//                 {"state", "inprogress | done | fail"},
//                 {"info", "Open/Close clamper"},
//                 {"rc", "int - action return code"}
//             },
//             {
//                 {"name", "lock"},
//                 {"state", "inprogress | done | fail"},
//                 {"info", "Close clamper"},
//                 {"rc", "int - action return code"}
//             }
//          }//list
//        }//action_clamp
//     };


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
        {"info", "Request Accepted"},
        {"state", "Wait"},
        {"action_list", {
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
    std::string s2 = jsnList.dump(indent);
    jsnData = QString::fromStdString(s2);
    jsnDoc = QJsonDocument::fromJson(jsnData.toUtf8(), &jsonError);
//    if(jsonError.error != QJsonParseError::NoError){
//            str = "Error: "; str += jsonError.errorString();
//            GUI_Write_To_Log(value, str);
//     }       //return;

    //Get the main JSON object and get the data in it
    jsnObj = jsnDoc.object();


}// init_json
