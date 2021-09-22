/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SocketServer.h
 * Author: miksarus
 *
 * Created on August 16, 2017, 4:04 PM
 */

#ifndef SOCKETClient_H
#define SOCKETClient_H

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>
#include <stdio.h>
#include <string.h>


typedef enum ServerCommand {
    ACT_NONE = 0,
    ACT_SEND_COORDS = 1
} ServerCommand;


typedef struct DetectorState {
    bool isDetected;
    int objectX;
    int objectY;
} DetectorState;


class SocketClient {
public:
    SocketClient(const char* socketPath);
    virtual ~SocketClient();

	int GetState(DetectorState *state);
private:
    char* _serverPath;
};

#endif /* SOCKETClient_H */