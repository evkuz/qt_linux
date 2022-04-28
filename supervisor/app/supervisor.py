import threading
import json
import time
import logging
from typing import Optional, Dict

from .devices import BaseDevice
from .devices import agents


class Supervisor(object):
    def __init__(self, updateStateInterval:float):
        self._updateInterval = updateStateInterval
        self._devices:Dict[str, BaseDevice] = {}
        self._state:Dict[str,dict] = {'supervisor':{}}
        self.set_tech_process_state(False)

        self.__thread = None
        self._isWorking = False
        self.__lastUpdateTime = 0.0
        
        self._initialize()
        self.start()
        
        
    @property
    def State(self)->dict:
        return dict(self._state)
    
    def set_tech_process_state(self, value:bool)->None:
        self.__tech_process = value
        self._state['supervisor']['techprocess'] = value

    @property
    def ListVideoDevices(self)->dict:
        video_devices = ['pinkman', 'hiwonder', 'mobman_camera', 'blueman']
        res = {}
        for name in video_devices:
            if name in self._devices:
                res[name] = self._devices[name].Address
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
                if self.__tech_process:
                    self._do_actions()
            time.sleep(20)

        self.__thread = None
        self._isWorking = False

    def stop(self):
        if self.__thread is not None:
            self._isWorking = False

    def __del__(self):
        self.stop()


    def _initialize(self):
        self._devices['pinkman'] = agents.Pinkman("http://192.168.1.177:5001", self._updateInterval)
        self._devices['xrrobot'] = agents.XRRobot("http://192.168.1.98:5001", self._updateInterval)
        self._devices['hiwonder'] = agents.Hiwonder("http://192.168.1.175:5001", self._updateInterval)
        self._devices['hiwonder_qt'] = agents.HiwonderQt("http://192.168.1.175:8383", self._updateInterval)
        self._devices['mobman_move'] = agents.MobmanMove("http://192.168.1.205:5001", self._updateInterval)
        self._devices['mobman_camera'] = agents.MobmanCamera("http://192.168.1.201:5001", self._updateInterval)
        self._devices['mobman_qt'] = agents.MobmanQt("http://192.168.1.201:8383", self._updateInterval)
        self._devices['blueman'] = agents.Blueman("http://192.168.1.158:5001", self._updateInterval)
        self._devices['wheeltec'] = agents.Wheeltec("http://192.168.1.80:5001", self._updateInterval)

    def _update_devices_states(self):
        for name, device in self._devices.items():
            self._state[name] = device.State
    
    def _do_actions(self):
        for device_name, device in self._devices.items():
            device.do_action(self._state)
            self._state[device_name] = device.State

    
    def run_action(self, device:str, command:str)->dict:
        if not device in self._devices:
            return {'rc' : -9, 'info' : "No device was found!"}
        if self._devices[device].run_action(command):
            return {'rc' : 0, 'info' : "success"}
        return {'rc' : -1, 'info' : "fail"}

    def reset_actions(self, device:Optional[str])->dict:
        if device is None:
            for d in self._devices.values():
                d.reset_actions()
            return {'rc' : 0, 'info' : "success"}
        elif device not in self._devices:
            return {'rc' : -9, 'info' : "No device was found!"}
        
        if self._devices[device].reset_actions():
            return {'rc' : 0, 'info' : "success"}
        return {'rc' : -1, 'info' : "fail"}

    def get_info(self, device:str, service:str)->dict:
        if not device in self._devices:
            return {'rc' : -9, 'info' : "No device was found!"}
        return self._devices[device].get_info(service)