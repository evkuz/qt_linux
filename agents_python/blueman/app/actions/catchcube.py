from time import sleep
from typing import List
from iqrdevice.actions import BaseAction
from iqrdevice.utils.vision import BaseCamera, BaseDetector
from iqrdevice.utils.controllers import ArduinoManipulator
import logging


class CatchCubeAction (BaseAction):
    def __init__(self, arduino_device:ArduinoManipulator, cam:BaseCamera, detector:BaseDetector, opened_val:int, closed_val:int):
        BaseAction.__init__(self, "catchcube")
        self.logger = logging.getLogger("CatchCubeAction")
        self.__cam = cam
        self.__detector = detector
        self.__manip = arduino_device
        self._opened_val = opened_val
        self._closed_val = closed_val
        
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
                self.__cam.wait_for_new_frame()
                frame = self.__cam.get_last_frame()
                det_res = self.__detector.detect(frame)
                if det_res['detected']:
                    errZ = (0.7 - det_res['w'])*self.__cam.FrameWidth
                    errX = (det_res['x'] - 0.5) * self.__cam.FrameWidth
                    errY = (det_res['y'] - 0.4) * self.__cam.FrameHeight

                    newPos = self.calc_next_position([errX,errY,errZ], currentPos)
                    currentPos, dist = self.move_manip(newPos)
                    
                    detectedSteps += 1
                    notDetectedSteps = 0
                else:
                    notDetectedSteps += 1
                if dist < 2.9:
                    currentPos[4] = self._closed_val
                    _ = self.move_manip(currentPos)
                    tp_state+=1
                if detectedSteps > 150 or notDetectedSteps > 200:
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
                    self._closed_val
                ]
                _ = self.move_manip(pos)

                pos = [currentPos[0], 120, 60, currentPos[3], self._closed_val]
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

    def calc_next_position(self, errs:List[float], manipPos:List[int]):
        self.logger.info(f"Errors: {errs}" )
        self.logger.info(f"CUR_POS: {manipPos}")
        
        __pixToDegreeX = 60.0 / 640
        
        newPos = [
            int(manipPos[0] - 0.15*(__pixToDegreeX * errs[0])),
            int(manipPos[1] - 0.*errs[1] - 0.002*errs[2]),
            int(manipPos[2] + 0.04*errs[1] - 0.*errs[2]),
            manipPos[3],
            self._opened_val
        ]
        self.logger.info(f"NEW_POS: {newPos}")
        return newPos
