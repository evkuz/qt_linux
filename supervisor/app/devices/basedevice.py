import threading
import requests
import time

from .remotedevice import RemoteDevice
from .basestatus import BaseStatus


class BaseDevice:
    def __init__(self, addr:str, name:str, updateStateInterval:int=100):
        """_summary_

        Args:
            addr (str): device address
            name (str): device name
            updateStateInterval (int, optional): interval in milliseconds. Minimum value is 20. Defaults to 100.
        """
        # minimal interval is 20 milliseconds
        self.__updateStateInterval = updateStateInterval if updateStateInterval > 20 else 20
        self.remote_device = RemoteDevice(addr)
        # timeout in remote device is in seconds
        self.remote_device.set_timeout((self.__updateStateInterval - 10)/1000)
        self.name = name
        
        self.__thread = None
        self._isWorking = False
        self._state = BaseStatus(name)
        self.__lastUpdateTime = 0

    @property
    def State(self):
        return self._state.__copy__()
    
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
                self.__update_state()
            time.sleep(0.02)

        self.__thread = None
        self._isWorking = False
    
    def __update_state(self):
        # url = "/run?cmd=status&"
        try:
            self.__lastUpdateTime = time.time()
            status = self._update_state()
            self._state.set_connected()
            if type(status) is not dict:
                self._state.set_state("Error: can't parse state")
                return
            if 'action_list' in status:
                action_list = status['action_list']
                self._state.set_actions_list(action_list)
                if len(action_list) == 0:
                    status = "waiting"
                else:
                    status = "busy with actions:"
                    status += ",".join([act['name'] for act in action_list])
                self._state.set_state(status)
        except Exception as e:
            self._state.set_disconnected(str(e))

    def run_action(self, actionName:str, **kwargs)->bool:
        #url = self.addr + f"/run?cmd={cmdName}&"
        try:
            data = self._run_action(name=actionName, **kwargs)
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
            data = self._get_service_info(serviceName, **kwargs)
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
            data = self._send_reset()
            if type(data) is not dict:
                rc = 1
            else:
                rc = int(data['rc'])
            return rc == 0
        except Exception as e:
            raise e

    def stop(self):
        if self.__thread is not None:
            self._isWorking = False

    def __del__(self):
        self.stop()

    def _update_state(self)->dict:
        raise NotImplementedError()

    def _run_action(self, actionName:str, **kwargs)->dict:
        raise NotImplementedError()

    def _get_service_info(self, serviceName:str, **kwargs):
        raise NotImplementedError()

    def _send_reset(self)->dict:
        raise NotImplementedError()


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