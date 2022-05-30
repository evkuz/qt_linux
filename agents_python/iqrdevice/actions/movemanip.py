from time import sleep
from iqrdevice.actions import BaseAction
from iqrdevice.utils.controllers import ArduinoManipulator


class MoveManipulatorAction (BaseAction):
    def __init__(self, arduino_device:ArduinoManipulator):
        BaseAction.__init__(self, "movemanip")
        self.__manip = arduino_device

    def get_info(self) -> dict:
        return self.make_info(
            "Will move manipulator",
            parameters={
                'pos':'servo angles separated with comma, for example: pos=90,120,35,90,80',
                'speed': "moving speed in percentage (1,100)"
            }
        )

    def run_action(self, **kwargs) -> int:
        speed = 70
        if 'speed' in kwargs:
            speed = int(kwargs['speed'])

        if 'pos' in kwargs:
            pos = [int(x) for x in kwargs['pos'].split(',')]
            if len(pos) < self.__manip.n_servos:
                self._set_state_info("Error: wrong number of positions")
                return -2
            self.__manip.move(pos, speed, True)
            pos, dist = self.__manip.get_position()
            self._set_state_info(f"Current position: ({pos})")
            return 0
        else:
            return -2

    def reset_action(self) -> int:
        self.__manip.stop_moving()
        self._set_state_info(f"Manually stopped")
        return -126

class MoveHomeAction (BaseAction):
    def __init__(self, arduino_device:ArduinoManipulator):
        BaseAction.__init__(self, "movehome")
        self.__manip = arduino_device

    def get_info(self) -> dict:
        return self.make_info(
            "Will move manipulator to home position",
            parameters={
                'speed': "moving speed in percentage (1,100)"
            }
        )

    def run_action(self, **kwargs) -> int:
        speed = 70
        if 'speed' in kwargs:
            speed = int(kwargs['speed'])

        self.__manip.move_home(speed, True)
        pos, dist = self.__manip.get_position()
        self._set_state_info(f"Current position: ({pos})")
        return 0
        
    def reset_action(self) -> int:
        self.__manip.stop_moving()
        self._set_state_info(f"Manually stopped")
        return -126
