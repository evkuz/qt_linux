/*
response += "{\"name\": "; response += "\"mobman\", ";
response += "\"rc\":"; response += "777"; response +=", ";  //must be some integer
response += "\"info\": ";
response += "\"text interpretation of return code\", ";
response += "\"state\": ";
//    response += "\"status\":\"";
response += "\""; response += data2Client; //global device status
response += "\", ";

response += "\"action_list\": [{";
//response += "{\n\t";
response += "\"name\": ";
response += "\"get_box\", "; // action == 'start'
response += "\"state\": ";
response += "\"succsess\", ";
response += "\"info\": ";
response += "\"Get the cube and put it somewhere\", ";
response += "\"st_time\": ";
//response += "\"";
response += QString::number(dt.toSecsSinceEpoch()); response +=", ";
// response += "\"";
response += "\"fin_time\": ";
//response += "\"";
response +=  QString::number(dt.toSecsSinceEpoch() + 1000000); response +=", ";
//response += "\"";
response += "\"result\": ";
// response += "\"";
response += "0";
//  response += "\" ";
response += "},";

response += "{";//\n\t";
response += "\"name\": ";
response += "\"reset\", "; // action == 'reset'
response += "\"state\": ";
response += "\"done\", ";
response += "\"info\": ";
response += "\"Change state to DONE after moving cube\", ";
response += "\"st_time\": ";
//response += "\"";
response +=  QString::number(dt.toSecsSinceEpoch() + 2000000);
//response += "\", ";
response +=", ";
response += "\"fin_time\": ";
//response += "\"";
response +=  QString::number(dt.toSecsSinceEpoch() + 5000000);
response += ", ";
response += "\"result\": ";
//response += "\"";
response += "0";
//response += " \n";
response += "}";

response += "]"; //end of action_list

response += "\n}"; //main bracket
*/
