import threading
import json
import time
import logging
from .devices import agents


class Supervisor(object):
    def __init__(self, updateStateInterval:int):
        self._updateInterval = updateStateInterval
        self._devices = {}
        self._state = {}
        self._cubes = {}

        self.__thread = None
        self._isWorking = False
        self.__lastUpdateTime = 0
        
        self._initialize()
        self.start()
        
    @property
    def State(self)->dict:
        return dict(self._state)
    
    @property
    def ListDevices(self)->dict:
        res = {}
        for name, device in self._devices.items():
            res[name] = device.Address
        return res

    @property
    def IsWorking(self):
        return self._isWorking or self.__thread is not None

    def start(self):
        if self.__thread is None:
            self.__thread = threading.Thread(target=self.__thread_work)
            self._isWorking = True
            self.__thread.start()

    def __thread_work(self):
        while self._isWorking:
            if time.time() - self.__lastUpdateTime > self._updateInterval:
                self._update_devices_states()
                self.__lastUpdateTime = time.time()
                self._do_actions()
            time.sleep(0)

        self.__thread = None
        self._isWorking = False

    def stop(self):
        if self.__thread is not None:
            self._isWorking = False

    def __del__(self):
        self.stop()


    def _initialize(self):
        self._devices['pinkman'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)
        self._devices['xrrobot'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)
        self._devices['hiwonder'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)
        self._devices['hiwonder_qt'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)
        self._devices['mobman_move'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)
        self._devices['mobman_camera'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)
        self._devices['mobman_qt'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)
        self._devices['blueman'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)
        self._devices['wheeltec'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)

    def _update_devices_states(self):
        for name, device in self._devices.items():
            self._state[name] = device.State
    
    def _do_actions(self):
        for device_name, device in self._devices.items():
            changed_cubes = device.do_action(self._state, self._cubes)
            for name, state in changed_cubes.items():
                self._cubes[name] = state
            self._state[device_name] = device.State

    
    def run_action(self, device:str, command:str)->dict:
        if not device in self._devices:
            return {'rc' : -9, 'info' : "No device was found!"}
        if self._devices[device].run_action(command):
            return {'rc' : 0, 'info' : "success"}
        return {'rc' : -1, 'info' : "fail"}

    def reset_actions(self, device:str)->dict:
        if not device in self._devices:
            return {'rc' : -9, 'info' : "No device was found!"}
        if self._devices[device].reset_actions():
            return {'rc' : 0, 'info' : "success"}
        return {'rc' : -1, 'info' : "fail"}

    def get_info(self, device:str, service:str)->dict:
        if not device in self._devices:
            return {'rc' : -9, 'info' : "No device was found!"}
        return self._devices[device].get_info(service)