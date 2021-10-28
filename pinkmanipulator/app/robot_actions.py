from .action import BaseAction
from subprocess import Popen


class CatchCubeAction(BaseAction):
    def __init__(self, camera, serial):
        BaseAction.__init__(self, "catchcube")
        self.__camera = camera
        self.__serial = serial
        self.__pixToDegreeX = 60.0 / self.__camera.FrameWidth
        self.__pixToDegreeY = 50.0 / self.__camera.FrameHeight
        self.__pixToDegreeZ = 0.015

    def move_manip(self, pos1, pos2, pos3, pos4):
        pos = self.__serial.send_command(pos1, pos2, pos3, pos4)
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return pos

    def run_action(self):
        res = 0
        tp_state = 0
        while self._isWorking:
            if tp_state == 0:
                detected, x, y, w, h = self.__camera.get_position()
                currentPos = self.__serial.get_state()
                if detected:
                    errZ = self.__camera.FrameWidth - w
                    errX = x - self.__camera.FrameWidth / 2
                    errY = y - self.__camera.FrameHeight / 2
                    pos1 = int(currentPos[0] - 0.2*(self.__pixToDegreeX * errX))
                    pos2 = int(currentPos[1] - 0.2*(self.__pixToDegreeY*errY + self.__pixToDegreeZ*errZ))
                    pos3 = int(currentPos[2] - 0.3*(self.__pixToDegreeY*errY - self.__pixToDegreeZ*errZ))
                    pos4 = 180
                    currentPos = self.move_manip(pos1, pos2, pos3, pos4)
                if currentPos[4] == 1:
                    _ = self.move_manip(
                        currentPos[0],
                        currentPos[1],
                        currentPos[2],
                        100
                    )
                    tp_state+=1
            if tp_state == 1:
                pos = self.move_manip(
                    currentPos[0],
                    currentPos[1] + 20,
                    currentPos[2] + 10,
                    100
                )
                pos = self.move_manip(currentPos[0], 120, 60, 100)
                break
        return res
    
    def reset_action(self):
        pos = self.__serial.go_to_start()
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return -126


class PutCubeAction(BaseAction):
    def __init__(self, camera, serial):
        BaseAction.__init__(self, "putcube")
        self.__camera = camera
        self.__serial = serial

    def move_manip(self, pos1, pos2, pos3, pos4):
        pos = self.__serial.send_command(pos1, pos2, pos3, pos4)
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return pos

    def run_action(self):
        res = 0
        #currentPos = self.__serial.get_state()
        if self._isWorking:
            _ = self.move_manip(180, 120, 60, 100)
        if self._isWorking:
            _ = self.move_manip(180, 49, 90, 100)
        if self._isWorking:
            _ = self.move_manip(180, 49, 90, 180)
        if self._isWorking:
            _ = self.move_manip(180, 120, 74, 180)
        if self._isWorking:
            _ = self.move_manip(91, 120, 60, 180)
        if self._isWorking:
            pos = self.__serial.go_to_start()
            self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return res
    
    def reset_action(self):
        pos = self.__serial.go_to_start()
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return -126
