from time import sleep
from iqrdevice.action import BaseAction
from ..utils import SerialCommunication


class PutCubeAction (BaseAction):
    def __init__(self, arduino_device:SerialCommunication):
        BaseAction.__init__(self, "putcube")
        self.__serial = arduino_device

    def get_info(self) -> dict:
        return self.make_info(
            "Will try to catch cube with robot"
        )

    def run_action(self, **kwargs) -> int:
        res = 0
        #currentPos = self.__serial.get_state()
        if self._workingFlag:
            _ = self.move_manip(180, 120, 60, 100)
        if self._workingFlag:
            _ = self.move_manip(180, 49, 90, 100)
        if self._workingFlag:
            _ = self.move_manip(180, 49, 90, 180)
        if self._workingFlag:
            _ = self.move_manip(180, 120, 74, 180)
        if self._workingFlag:
            _ = self.move_manip(91, 120, 60, 180)
        if self._workingFlag:
            pos = self.__serial.go_to_start()
            self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return res

    def reset_action(self) -> int:
        pos = self.__serial.go_to_start()
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return -126

    def move_manip(self, pos1, pos2, pos3, pos4):
        pos = self.__serial.send_command(pos1, pos2, pos3, pos4)
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return pos
