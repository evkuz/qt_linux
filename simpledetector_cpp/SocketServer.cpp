/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SocketServer.cpp
 * Author: miksarus
 *
 * Created on August 16, 2017, 4:04 PM
 */

#include "SocketServer.h"

#include <iostream> // std::cout
#include <mutex>
#include <stdio.h>
#include <string.h>
#include <thread> // std::thread

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std;

void error(const char *msg) { perror(msg); }

bool ParseCommand(char *str, ServerCommand &command) {
  bool result = false;
  int count = 0;

  try {
    int cmd_num(0);
    sscanf(str, "%d", &cmd_num);
    command = static_cast<ServerCommand>(cmd_num);
    result = true;
  } catch (...) {
    result = false;
  }

  return result;
}

SocketServer::SocketServer(const char *socketPath) {
  int pathLength = strlen(socketPath);
  _serverPath = new char[pathLength];
  strncpy(_serverPath, socketPath, pathLength);
  cout << "!!!!!" << _serverPath << endl;

  _isBusy = false;
  _isStoped = true;
  _haveWork = false;
  _thread = NULL; //

  _detectorState = false;
  _objectX = 0;
  _objectY = 0;
}

SocketServer::~SocketServer() {
  Stop();
  delete[] this->_serverPath;
}

int SocketServer::Start() {
  if (_isStoped) {
    std::lock_guard<std::mutex> lg(_locker);
    this->_serverSock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (_serverSock < 0) {
      return -1;
    }

    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;

    strncpy(serverAddr.sun_path, _serverPath, sizeof(serverAddr.sun_path) - 1);
    // unlink(_serverPath);

    if (bind(_serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) <
        0) {
      return -2;
    }

    listen(_serverSock, 5);

    if (chmod(_serverPath, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
      cerr << "Chmode error!" << endl;
    };

    _isStoped = false;

    printf("We're ready to listen\n");
    _thread = new std::thread(&SocketServer::DoWork, this);
  }

  return (_thread != NULL) ? 0 : -3;
}

void SocketServer::Stop() {
  if (!_isStoped) {
    _isStoped = true;
    if (this->_thread) {
      close(_serverSock);
      this->_thread->join();
      delete this->_thread;
      this->_thread = NULL;
    }
  }
}

void SocketServer::DoWork() {
  int client;
  char msg[20];
  struct sockaddr_un cli_addr;
  int newsockfd;

  ServerCommand command;
  char buf[80];

  snprintf(msg, 20, "SocketServer started ");
  while (!this->_isStoped) {
    client = sizeof(cli_addr);
    newsockfd =
        accept(this->_serverSock, (struct sockaddr *)&cli_addr, &client);
    if (newsockfd < 0) {
      perror("accept error");
      continue;
    }
    n = read(newsockfd, buf, 80);
    if (!ParseCommand(buf, command)) {
      perror("Unknown command!");
      close(newsockfd);
      continue;
    }

    if (command == ServerCommand::ACT_SEND_COORDS) {
      std::lock_guard<std::mutex> lk(_locker);
      sprintf(buf, "%i %i %i", this->_detectorState, this->_objectX,
              this->_objectY);
      n = strlen(buf);
    }

    write(newsockfd, buf, n);
    close(newsockfd);
  }
}

void SocketServer::SetDetectorState(bool detected, int x, int y) {
  if (_isListening) {
    std::lock_guard<std::mutex> lk(_locker);
    this->_detectorState = detected;
    this->_objectX = x;
    this->_objectY = y;
  } else {
    Stop();
  }
}
