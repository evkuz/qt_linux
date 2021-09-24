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

