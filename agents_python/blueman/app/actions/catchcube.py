from time import sleep
from iqrdevice.action import BaseAction
from ..utils import SerialCommunication, CameraDetector


class CatchCubeAction (BaseAction):
    def __init__(self, arduino_device:SerialCommunication, cam:CameraDetector):
        BaseAction.__init__(self, "catchcube")
        self.__manip = arduino_device
        self.__camera = cam
        self.__pixToDegreeX = 60.0 / self.__camera.FrameWidth
        self.__pixToDegreeY = 50.0 / self.__camera.FrameHeight
        self.__pixToDegreeZ = 0.015

    def get_info(self) -> dict:
        return self.make_info(
            "Will try to catch cube with robot"
        )

    def run_action(self, **kwargs) -> int:
        res = 0
        tp_state = 0
        detectedSteps = 0
        notDetectedSteps = 0
        currentPos, dist = self.__manip.get_position()

        while self._workingFlag:
            if tp_state == 0:
                detected, x, y, w, h = self.__camera.get_position()
                if detected:
                    errZ = self.__camera.FrameWidth - w
                    errX = x - self.__camera.FrameWidth / 2
                    errY = y - self.__camera.FrameHeight / 2
                    newPos = [
                        int(currentPos[0] - 0.2*(self.__pixToDegreeX * errX)),
                        int(currentPos[1] + 0.2*(self.__pixToDegreeY*errY + self.__pixToDegreeZ*errZ)),
                        int(currentPos[2] + 0.3*(self.__pixToDegreeY*errY - self.__pixToDegreeZ*errZ)),
                        currentPos[3],
                        30
                    ]
                    currentPos, dist = self.move_manip(newPos)
                    
                    detectedSteps += 1
                    notDetectedSteps = 0
                else:
                    notDetectedSteps += 1
                if dist < 4:
                    currentPos[4] = 180
                    _ = self.move_manip(currentPos)
                    tp_state+=1
                if detectedSteps > 50 or notDetectedSteps > 100:
                    self._set_state_info("Can't get cube for too long!")
                    res = -10
                    _ = self.__manip.move_home(80)
                    break
            if tp_state == 1:
                pos = [
                    currentPos[0],
                    currentPos[1] + 20,
                    currentPos[2] + 10,
                    currentPos[3],
                    180
                ]
                _ = self.move_manip(pos)

                pos = [currentPos[0], 120, 60, currentPos[3], 30]
                _ = self.move_manip(pos)
                break
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
