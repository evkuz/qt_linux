from time import sleep
from iqrdevice.action import BaseAction
from ..utils import SerialCommunication
from . import GRIP_CLOSED, GRIP_OPENED


class OpenGripAction (BaseAction):
    def __init__(self, arduino_device:SerialCommunication):
        BaseAction.__init__(self, "opengrip")
        self.__manip = arduino_device

    def get_info(self) -> dict:
        return self.make_info(
            "Open grip"
        )

    def run_action(self, **kwargs) -> int:
        res = 0
        currentPos, dist = self.__manip.get_position()
        currentPos[-1] = GRIP_OPENED
        if self._workingFlag:
            _ = self.move_manip(currentPos)
        
        return res

    def reset_action(self) -> int:
        self.__manip.stop_moving()
        self._set_state_info(f"Stopped")
        return -126

    def move_manip(self, pos:list):
        self.__manip.move(pos, 80, True)
        pos, dist = self.__manip.get_position()
        self._set_state_info(f"Current position: ({pos})")
        return pos, dist


class CloseGripAction (BaseAction):
    def __init__(self, arduino_device:SerialCommunication):
        BaseAction.__init__(self, "closegrip")
        self.__manip = arduino_device

    def get_info(self) -> dict:
        return self.make_info(
            "Close grip"
        )

    def run_action(self, **kwargs) -> int:
        res = 0
        currentPos, dist = self.__manip.get_position()
        currentPos[-1] = GRIP_CLOSED
        if self._workingFlag:
            _ = self.move_manip(currentPos)
        
        return res

    def reset_action(self) -> int:
        self.__manip.stop_moving()
        self._set_state_info(f"Stopped")
        return -126

    def move_manip(self, pos:list):
        self.__manip.move(pos, 80, True)
        pos, dist = self.__manip.get_position()
        self._set_state_info(f"Current position: ({pos})")
        return pos, dist
