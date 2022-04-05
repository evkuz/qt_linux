from iqrdevice.service import BaseService
from ..utils import RunMVRasp


class StartMVRaspService(BaseService):
    def __init__(self, rmv:RunMVRasp):
        BaseService.__init__(self, "startmvrasp")
        self.__rmv = rmv
    
    def get_data(self, **kwargs):
        if self.__rmv.run():
            return 'OK'
        return "FAIL"
        
    def get_info(self) -> dict:
        return self.make_info(
            "Starts mvrasp script"
        )


class StopMVRaspService(BaseService):
    def __init__(self, rmv:RunMVRasp):
        BaseService.__init__(self, "stopmvrasp")
        self.__rmv = rmv
    
    def get_data(self, **kwargs):
        if self.__rmv.stop():
            return 'OK'
        return "FAIL"
        
    def get_info(self) -> dict:
        return self.make_info(
            "Stops mvrasp script"
        )
