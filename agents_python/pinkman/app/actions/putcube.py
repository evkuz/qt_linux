from time import sleep
from iqrdevice.action import BaseAction
from ..utils import SerialCommunication


class PutCubeAction (BaseAction):
    def __init__(self, arduino_device:SerialCommunication):
        BaseAction.__init__(self, "putcube")
        self.__manip = arduino_device

    def get_info(self) -> dict:
        return self.make_info(
            "Will put cube to predefined position"
        )

    def run_action(self, **kwargs) -> int:
        res = 0
        if self._workingFlag:
            _ = self.move_manip([90, 120, 70, 120])
        if self._workingFlag:
            _ = self.move_manip([165, 120, 70, 120])
        if self._workingFlag:
            _ = self.move_manip([165, 50, 115, 120])
        if self._workingFlag:
            _ = self.move_manip([165, 50, 115, 180])
        if self._workingFlag:
            _ = self.move_manip([165, 120, 70, 180])
        
        return res

    def reset_action(self) -> int:
        self.__manip.stop_moving()
        self.__manip.move_home(80, True)
        self._set_state_info(f"Stopped and returned home")
        return -126

    def move_manip(self, pos:list):
        self.__manip.move(pos, 80, True)
        pos, dist = self.__manip.get_position()
        self._set_state_info(f"Current position: ({pos})")
        return pos, dist
