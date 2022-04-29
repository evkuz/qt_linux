#include "jsoninfo.h"

JsonInfo::JsonInfo()
{

}
//+++++++++++++++++++++

void JsonInfo::init_json()
{
    jsnInfo = {
       {"name", DEV_NAME},
       {"rc", RC_UNDEFINED}, //RC_SUCCESS
       {"info", "Request Accepted"},
       {"state", "Wait"},
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
           {"info", "Get the box by clamper, ascing CV about distance in advance"},
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
            },

          } //list
        }//action_list-field

    }; // jsnInfo
}// init_json
