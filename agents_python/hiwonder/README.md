# About HTTP server parts

## Video streaming

The base was taken from this article: https://blog.miguelgrinberg.com/post/video-streaming-with-flask

## Robot commands API

Address server for api:

```
<server_address>/run?cmd=<name>
```

Server will read command from cmd and return robot status as JSON.
Avalialable commands:

* `start` - начало выполнения движения
* `reset` - сброс состояния до начального
* `что_угодно` - возвращает статус

Example of returned JSON:

```json
{
  "lastCmd": "reset",
  "lastCmdResult": 0,
  "status": "wait"
}
```

Status can be:

* `wait` - waiting for command
* `init` - command was recognized and will be performed but not started yet
* `inprogress` - command was performed and in progress
* `done` - execution of the command was finished





dmesg | egrep --color 'serial|ttyS'

https://linuxconfig.org/ubuntu-20-04-connect-to-wifi-from-command-line


Hiwonder_QT:

{
"name": "HIWONDER",
"rc": 0,
"info": "Request Accepted",
"state": "wait",
"action_list": [
{
"name": "clamp",
"state": "inprogress | done | fail",
"info": "Open/Close clamper",
"rc": "int - action return code"
},
{
"name": "lock",
"state": "inprogress | done | fail",
"info": "Close clamper",
"rc": "int - action return code"
},
{
"name": "unlock",
"state": "inprogress | done | fail",
"info": "Open clamper",
"rc": "int - action return code"
},
{
"name": "start",
"state": [
"waiting",
"noDetection",
"inprogress",
"done",
"fail"
],
"info": "Get the box by clamper, ascing CV about distance in advance",
"st_time": "int - timestamp of last start",
"fin_time": "int - timestamp of finish",
"result": "int - action return code"
},
{
"name": "reset",
"state": "succsess | fail",
"info": "Set device status as <Wait>",
"rc": "int - action return code"
},
{
"name": "sit",
"state": "inprogress | done | fail",
"info": "Sit position",
"rc": "int - action return code"
},
{
"name": "standup",
"state": "inprogress | done | fail",
"info": "Go to start (initital) position",
"rc": "int - action return code"
},
{
"name": "put_box",
"state": "inprogress | done | fail",
"info": "put down the object for next actions",
"rc": "int - action return code"
},
{
"name": "close",
"state": "inprogress | done | fail",
"info": "Close serial port of the robot",
"rc": "int - action return code"
},
{
"name": "getservos",
"state": "inprogress | done | fail",
"info": "Return device' servos values",
"rc": "int - action return code"
},
{
"name": "info",
"state": "inprogress | done | fail",
"info": "Print this page",
"rc": "int - action return code"
}
]
}