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

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>
#include <stdio.h>
#include <string.h>


typedef enum ServerCommand {
    ACT_NONE = 0,
    ACT_SEND_COORDS = 1
} ServerCommand;


class SocketServer {
public:
    SocketServer(const char* socketPath);
    virtual ~SocketServer();

	int Start();
    void Stop();
    bool IsRunning() { std::lock_guard<std::mutex> lg(_locker); return !_isStoped;}
    void SetDetectorState(bool detected, int x, int y);

private:
    void DoWork();

	bool _isStoped = true;

	std::thread *_thread;
	std::mutex _locker;
    bool _detectorState;
    int _objectX;
    int _objectY;

private:
    char* _serverPath;
    int _serverSock;
};

#endif /* SOCKETSERVER_H */