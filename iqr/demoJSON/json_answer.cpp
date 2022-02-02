/*
 *
 *
// 0x20 (пробел), 0x09 (табуляцию), 0x0A (перевод строки) и 0x0D (возврат каретки).
// Пробелы допускаются до и после «структурных символов» (structural characters) []{}:,

response += "{\"name\": "; response += "\"mobman\", ";
response += "\"rc\":"; response += "777"; response +=", ";  //must be some integer
response += "\"info\": ";
response += "\"text interpretation of return code\", ";
response += "\"state\": ";
response += "\""; response += data2Client; //global device status
response += "\", ";

response += "\"action_list\": [{";
response += "\"name\": ";
response += "\"get_box\", "; // action == 'start'
response += "\"state\": ";
response += "\"succsess\", ";
response += "\"info\": ";
response += "\"Get the cube and put it somewhere\", ";
response += "\"st_time\": ";
response += QString::number(dt.toSecsSinceEpoch()); response +=", ";
response += "\"fin_time\": ";
response +=  QString::number(dt.toSecsSinceEpoch() + 1000000); response +=", ";
response += "\"result\": ";
response += "0";
response += "},";

response += "{";//\n\t";
response += "\"name\": ";
response += "\"reset\", "; // action == 'reset'
response += "\"state\": ";
response += "\"done\", ";
response += "\"info\": ";
response += "\"Change state to DONE after moving cube\", ";
response += "\"st_time\": ";
response +=  QString::number(dt.toSecsSinceEpoch() + 2000000);
response +=", ";
response += "\"fin_time\": ";
response +=  QString::number(dt.toSecsSinceEpoch() + 5000000);
response += ", ";
response += "\"result\": ";
response += "0";
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
преобразование строки параметров
?a=12&a=12432354&b=123&c=1241234
в массив ['a', '12'] ['a', '12432354'] ['b', '123'] ['c', '1241234']
        lst = str(request.args)
        lst2 = lst.find('[')
        a = lst[lst2:-1].replace('\'','\"').replace('(','[').replace(')',']')
        aa = json.loads(a)
*/
