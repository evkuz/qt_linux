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
//++++++++++++++++++++++++++++++++++++++++++++++++

CV device JSON answer
{
"rc": 0,
"info": "success",
"name": "getposition",
"data": {
"detected": true,
"x": -75,
"y": -58,
"width": 105,
"height": 104,
"err_angle": -7.128930216417201,
"distance": 217.02240483137268
}
}

//++++++++++++++++++++++++++++++++++++++++++++++++
{"rc": 0, "info": "success", "name": "getposition", "data": {"detected": true, "x": -46.0, "y": -70.0, "width": 158, "height": 154, "err_angle": -4.2654448889627625, "distance": 148.3335127743527}}
//+++++++++++++++++++++++++++++++++++++++++++++++
{
\"rc\": 0,
\"info\": "success",
\"name\": "getposition",
\"data\": {
\"detected\": true,
\"x\": -75,
\"y\": -58,
\"width\": 105,
\"height\": 104,
\"err_angle\": -7.128930216417201,
\"distance\": 217.02240483137268
}
}
*/
