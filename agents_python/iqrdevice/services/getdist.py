from iqrdevice.services import BaseService
from iqrdevice.utils.controllers import ArduinoManipulator


class GetDistService(BaseService):
    def __init__(self, arduino_device:ArduinoManipulator):
        BaseService.__init__(self, "getdist")
        self.__manip = arduino_device
    
    def get_data(self, **kwargs):
        pos, d = self.__manip.get_position()
        return {'distance': d }

    def get_info(self) -> dict:
        return self.make_info(
            "Returns distance in cm from sonar",
        )
