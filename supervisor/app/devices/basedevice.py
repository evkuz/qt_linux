import threading
import requests
import time

from .remotedevice import RemoteDevice
from .basestatus import BaseStatus


class BaseDevice:
    def __init__(self, addr, name, updateStateInterval=100):
        self.__updateStateInterval = updateStateInterval if updateStateInterval > 20 else 20
        self.remote_device = RemoteDevice(addr)
        self.remote_device.set_timeout((self.__updateStateInterval - 10)/1000)
        self.name = name
        
        self.__thread = None
        self._isWorking = False
        self.__state = BaseStatus(name)
        self.__lastUpdateTime = 0

    @property
    def State(self):
        return self.__state.__copy__()
    
    @property
    def IsWorking(self):
        return self._isWorking or self.__thread is not None

    @property
    def Name(self):
        return self.name

    def start(self):
        if self.__thread is None:
            self.__thread = threading.Thread(target=self.__thread_work)
            self._isWorking = True
            self.__thread.start()

    def __thread_work(self):
        while self._isWorking:
            if time.time() - self.__lastUpdateTime > self.__updateStateInterval:
                self.update_state()
            time.sleep(0.02)

        self.__thread = None
        self._isWorking = False
    
    def update_state(self):
        # url = "/run?cmd=status&"
        try:
            self.__lastUpdateTime = time.time()
            data = self.remote_device.get_status()
            self.__parse_state(data)
        except Exception as e:
            self.__state.set_disconnected(str(e))

    def run_action(self, actionName:str, **kwargs)->bool:
        #url = self.addr + f"/run?cmd={cmdName}&"
        try:
            data = self.remote_device.run_action(name=actionName, **kwargs)
            if type(data) is not dict:
                rc = 1
            else:
                rc = int(data['rc'])
            return rc == 0
        except Exception as e:
            raise e
    
    def get_info(self, serviceName:str, **kwargs):
        #url = self.addr + f"/run?cmd={cmdName}&"
        try:
            data = self.remote_device.get_service_info(
                name=serviceName,
                timeout=1.,
                **kwargs
            )
            if type(data) is not dict:
                return None
            else:
                if int(data['rc']) != 0:
                    return None
            return data['data']
        except Exception as e:
            raise e

    def stop_actions(self)->bool:
        #url = self.addr + f"/run?cmd={cmdName}&"
        try:
            data = self.remote_device.reset_actions([])
            if type(data) is not dict:
                rc = 1
            else:
                rc = int(data['rc'])
            return rc == 0
        except Exception as e:
            raise e

    def __parse_state(self, data):
        self.__state.set_connected()
        if data is None:
            self.__state.set_state("none")
            self.__state.set_actions_list([])
            return

        self.__state.set_actions_list(data['action_list'])
        if len(data['action_list']) == 0:
            status = "waiting"
            active_action = "none"
        else:
            status = "busy with actions:"
            status += ",".join([act['name'] for act in data['action_list']])
        self.__state.set_state(status)


    def stop(self):
        if self.__thread is not None:
            self._isWorking = False


if __name__ == '__main__':
    dev = BaseDevice(
        "http://192.168.1.80:5001/",
        "wheeltec",
        100
    )
    dev.start()
    while True:
        print(dev.State)
        line = input()
        
        if len(line) < 2:
            break

        #print(dev.send_command(line))
    
    dev.stop()