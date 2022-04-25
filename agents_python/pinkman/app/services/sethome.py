from iqrdevice.service import BaseService
from ..utils import SerialCommunication


class SetHomeService(BaseService):
    def __init__(self, arduino_device:SerialCommunication):
        BaseService.__init__(self, "sethome")
        self.__manip = arduino_device
    
    def get_data(self, **kwargs):
        if 'pos' in kwargs:
            pos = [int(x) for x in kwargs['pos'].split(',')]
            if len(pos) < self.__manip.n_servos:
                raise Exception("Wrong number of positions")
        
            self.__manip.set_home_position(pos)
            return 'OK'
        raise Exception("You must specify 'pos' parameter!")

    def get_info(self) -> dict:
        return self.make_info(
            "Sets home position for manipulator",
            parameters={
                'pos':'servo angles separated with comma, for example: pos=90,120,35,90',
            }
        )
