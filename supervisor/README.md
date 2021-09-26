# Supervisor web application

## Running application

To run server you need to install NPM:

```bash
sudo apt install npm
sudo npm install -g live-server
sudo npm install cors --save
```

Then you need to start server in this directory:

```bash
live-server --cors
```


## Usefull notes

Because of CORS (https://learn.javascript.ru/xhr-crossdomain) we can't make XMLHTTPRequest from JavaScript without special answer from server. So i had to add header to server answer (on json response):

```
Access-Control-Allow-Origin: *
```