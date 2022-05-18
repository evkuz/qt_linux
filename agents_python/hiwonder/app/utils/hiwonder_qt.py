import threading
import time
import logging
from typing import List

from iqrdevice.utils.controllers import RemoteDevice, BaseController

MINIMAL_TIMEOUT = 0.02

class HiwonderQt(BaseController):
    def __init__(self, addr:str, name:str, updateStateInterval:float=0.1):
        """_summary_

        Args:
            addr (str): device address
            name (str): device name
            updateStateInterval (float, optional): interval in seconds. Minimum value is 0.020. Defaults to 0.100.
        """
        # minimal interval is 20 milliseconds
        BaseController.__init__(self, name)
        self.__updateStateInterval = updateStateInterval if updateStateInterval > MINIMAL_TIMEOUT else MINIMAL_TIMEOUT
        self.remote_device = RemoteDevice(addr)
        # timeout in remote device is in seconds
        self.remote_device.set_timeout((self.__updateStateInterval - 0.010))
        
        self.__thread = None
        self._isWorking = False
        self._connected = False
        self.__lastUpdateTime = 0.0

    @property
    def channels(self) -> List[str]:
        return ['connected']

    @property
    def IsConnected(self):
        return self._connected

    @property
    def Address(self):
        return self.remote_device.addr
    
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
            time.sleep(MINIMAL_TIMEOUT)

        self.set_connected(False)
        self.__thread = None
        self._isWorking = False
    
    def __update_state(self):
        try:
            self.__lastUpdateTime = time.time()
            status = self.get_state()
            self.set_connected(True)
            if type(status) is not dict:
                return
            # if 'action_list' in status:
            #     action_list = status['action_list']
            #     self._state.set_actions_list(action_list)
            #     if len(action_list) == 0:
            #         status_str = "waiting"
            #     else:
            #         status_str = "busy with actions:"
            #         status_str += ",".join([act['name'] for act in action_list])
            #     self._state.set_state(status_str)
        except Exception as e:
            self.set_connected(False)

    def set_connected(self, value:bool):
        if value != self._connected:
            self._connected = value
            self.fire_event("connected", value)

    def run_action(self, actionName:str, **kwargs)->bool:
        #url = self.addr + f"/run?cmd={cmdName}&"
        try:
            data = self._run_action(actionName, **kwargs)
            if type(data) is not dict:
                rc = 1
            else:
                rc = int(data['rc'])
            return rc == 0
        except Exception as e:
            logging.error(str(e))
            return False
    
    def get_info(self, serviceName:str, **kwargs)->dict:
        #url = self.addr + f"/run?cmd={cmdName}&"
        try:
            data = self._get_service_info(serviceName, **kwargs)
            if type(data) is not dict:
                logging.warning(f"service {serviceName} result is not dict")
                return {
                    'rc': 0,
                    'name':serviceName,
                    'info': '',
                    'data': data
                }
            return data
        except Exception as e:
            logging.error(str(e))
            return {
                'rc': -1,
                'name':serviceName,
                'info': str(e),
                'data': None
            }

    def reset_actions(self)->bool:
        #url = self.addr + f"/run?cmd={cmdName}&"
        try:
            data = self._send_reset()
            if type(data) is not dict:
                rc = 1
            else:
                rc = int(data['rc'])
            return rc == 0
        except Exception as e:
            logging.error(str(e))
            return False

    def stop(self):
        if self.__thread is not None:
            self._isWorking = False

    def __del__(self):
        self.stop()

    def get_state(self)->dict:
        return self.remote_device.manual_request("/run?cmd=status&")

    def _run_action(self, actionName:str, **kwargs)->dict:
        _ = self.remote_device.manual_request(f"/run?cmd={actionName}&")
        return {
            "name": actionName,
            "rc": 0,
            "info": ''
        }

    def _get_service_info(self, serviceName:str, **kwargs):
        return {
            "name": serviceName,
            "rc": -1,
            "info": "No service available for this agent",
            "data": {}
        }

    def _send_reset(self)->dict:
        _ = self.remote_device.manual_request("/run?cmd=start&")
        return {
            "name": "reset",
            "rc": 0,
            "info": 'success',
            "data": {}
        }
