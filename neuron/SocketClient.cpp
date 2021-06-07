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

#include "SocketClient.h"

#include <chrono>
#include <iostream> // std::cout
#include <mutex>
#include <stdio.h>
#include <string.h>
#include <thread> // std::thread
#include <utility>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std;

SocketClient::SocketClient(const char *socketPath) {
  int pathLength = strlen(socketPath);
  _serverPath = new char[pathLength];
  strncpy(_serverPath, socketPath, pathLength);
}

SocketClient::~SocketClient() { delete[] this->_serverPath; }

int SocketClient::GetState(DetectorState *state) {
  int sockfd, servlen, n;
  struct sockaddr_un serv_addr;
  char buffer[82];

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sun_family = AF_UNIX;
  strcpy(serv_addr.sun_path, this->_serverPath);
  servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
  if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
    cerr << "Error creating socket" << endl;
    return -1;
  }

  if (connect(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0) {
    cerr << "Error connecting. Path:" << serv_addr.sun_path << endl;
    return -2;
  }

  sprintf(buffer, "%i", ServerCommand::ACT_SEND_COORDS);
  write(sockfd, buffer, strlen(buffer));

  n = read(sockfd, buffer, 80);
  int isDetected(0);
  int x(0), y(0);
  try {
    sscanf(buffer, "%i %i %i", &isDetected, &x, &y);
  } catch (...) {
    cerr << "Error reading values" << endl;
    return -3;
  }

  state->isDetected = (isDetected > 0);
  state->objectX = x;
  state->objectY = y;

  close(sockfd);
  return 0;
}