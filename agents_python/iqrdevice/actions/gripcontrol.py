from time import sleep
from iqrdevice.actions import BaseAction
from iqrdevice.utils.controllers import ArduinoManipulator


class OpenGripAction (BaseAction):
    def __init__(self, arduino_device:ArduinoManipulator, opened_val:int):
        BaseAction.__init__(self, "opengrip")
        self.__manip = arduino_device
        self.__opened_val = opened_val

    def get_info(self) -> dict:
        return self.make_info(
            "Open grip"
        )

    def run_action(self, **kwargs) -> int:
        res = 0
        currentPos, dist = self.__manip.get_position()
        currentPos[-1] = self.__opened_val
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
    def __init__(self, arduino_device:ArduinoManipulator, closed_val:int):
        BaseAction.__init__(self, "closegrip")
        self.__manip = arduino_device
        self.__closed_val = closed_val

    def get_info(self) -> dict:
        return self.make_info(
            "Close grip"
        )

    def run_action(self, **kwargs) -> int:
        res = 0
        currentPos, dist = self.__manip.get_position()
        currentPos[-1] = self.__closed_val
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
