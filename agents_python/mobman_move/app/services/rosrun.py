from iqrdevice.service import BaseService
from ..utils import RosRun


class StartRosRunService(BaseService):
    def __init__(self, rr:RosRun):
        BaseService.__init__(self, "startrosrun")
        self.__rr = rr
    
    def get_data(self, **kwargs):
        if self.__rr.run():
            return 'OK'
        return "FAIL"
        
    def get_info(self) -> dict:
        return self.make_info(
            "Starts RosRun script"
        )


class StopRosRunService(BaseService):
    def __init__(self, rr:RosRun):
        BaseService.__init__(self, "stoprosrun")
        self.__rr = rr
    
    def get_data(self, **kwargs):
        if self.__rr.stop():
            return 'OK'
        return "FAIL"
        
    def get_info(self) -> dict:
        return self.make_info(
            "Stops RosRun script"
        )
