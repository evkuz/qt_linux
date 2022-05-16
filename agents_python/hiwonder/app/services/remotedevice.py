from iqrdevice.services import BaseService
from ..utils import HiwonderQt
import time

class StartRMCommunicationService(BaseService):
    def __init__(self, remote_device:HiwonderQt):
        BaseService.__init__(self, "startrmcommunication")
        self.__remote_device = remote_device
    
    def get_info(self) -> dict:
        return self.make_info(
            "Starts/restarts communication with hiwonder_qt"
        )

    def get_data(self, **kwargs):
        if self.__remote_device.IsWorking:
            self.__remote_device.stop()
        self.__remote_device.start()
        
        time.sleep(0.2)
        if self.__remote_device.IsWorking:
            return "OK"
        return r"FAIL"

class StopRMCommunicationService(BaseService):
    def __init__(self, remote_device:HiwonderQt):
        BaseService.__init__(self, "stoprmcommunication")
        self.__remote_device = remote_device
    
    def get_info(self) -> dict:
        return self.make_info(
            "Stops communication with hiwonder_qt"
        )

    def get_data(self, **kwargs):
        if not self.__remote_device.IsWorking:
            return "OK"
        self.__remote_device.stop()
        time.sleep(0.2)
        if not self.__remote_device.IsWorking:
            return "OK"
        return r"FAIL"