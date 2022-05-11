import os
from time import sleep,time
from typing import List
from iqrdevice.actions import BaseAction
from iqrdevice.utils.vision import BaseCamera, BaseDetector
from iqrdevice.utils.controllers import ArduinoManipulator
import logging


class CatchCubeAction (BaseAction):
    def __init__(self, arduino_device:ArduinoManipulator, cam:BaseCamera, detector:BaseDetector, opened_val:int, closed_val:int, min_catch_dist:float=2.9):
        BaseAction.__init__(self, "catchcube")
        self.logger = logging.getLogger("CatchCubeAction")
        self.__cam = cam
        self.__detector = detector
        self.__manip = arduino_device
        self._opened_val = opened_val
        self._closed_val = closed_val
        self._min_catch_dist = min_catch_dist

    def get_info(self) -> dict:
        return self.make_info(
            "Will try to catch cube with robot"
        )

    def run_action(self, **kwargs) -> int:
        fileName=f"PM_points_{time()}.txt"
        with open(fileName, 'w') as lfile:
            lfile.write("x\ty\tw\terrx\terry\terrz\tpos1\tpos2\tpos3\tpos4\tcur1\tcur2\tcur3\tcur4\n")

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
                        x = det_res['x']
                        y = det_res['y']
                        w = det_res['width']

                        errZ = (0.7 - w)*self.__cam.FrameWidth
                        errX = (x - 0.5) * self.__cam.FrameWidth
                        errY = (y - 0.75) * self.__cam.FrameHeight
                        
                        newPos = self.calc_next_position([errX,errY,errZ], currentPos)
                        
                        lfile.write(f"{x}\t{y}\t{w}\t{errX}\t{errY}\t{errZ}\t{newPos[0]}\t{newPos[1]}\t{newPos[2]}\t{newPos[3]}\t{currentPos[0]}\t{currentPos[1]}\t{currentPos[2]}\t{currentPos[3]}\n")
                        
                        currentPos, dist = self.move_manip(newPos)

                        detectedSteps += 1
                        notDetectedSteps = 0
                    else:
                        notDetectedSteps += 1
                    # i changing min_catch dist becouse cube will turn a bit after grip closed
                    if dist < (self._min_catch_dist-0.1):
                        currentPos[3] = self._closed_val
                        _ = self.move_manip(currentPos)
                        tp_state+=1
                    if detectedSteps > 200 or notDetectedSteps > 250:
                        self._set_state_info("Can't get cube for too long!")
                        res = -10
                        _ = self.__manip.move_home(80)
                        break
                if tp_state == 1:
                    pos = [
                        currentPos[0],
                        currentPos[1] + 20,
                        currentPos[2] + 10,
                        currentPos[3]
                    ]
                    _ = self.move_manip(pos)

                    pos = [currentPos[0], 120, 60, currentPos[3]]
                    currentPos, dist = self.move_manip(pos)
                    if dist > self._min_catch_dist:
                        self.__cam.wait_for_new_frame()
                        frame = self.__cam.get_last_frame()
                        det_res = self.__detector.detect(frame)
                        if not 'y' in det_res or det_res['y'] < 0.65:
                            self._set_state_info("Cube wasn't held by grip")
                            res = -11
                            _ = self.__manip.move_home(90)
                    break
        if res == 0:
            newName = "SUCCESS_" + fileName
            os.rename(fileName, newName)
        return res

    def reset_action(self) -> int:
        self.__manip.stop_moving()
        self.__manip.move_home(80, True)
        self._set_state_info(f"Stopped and returned home")
        return -126

    def move_manip(self, pos:list):
        self.__manip.move(pos, 95, True)
        pos, dist = self.__manip.get_position()
        self._set_state_info(f"Current position: ({pos})")
        return pos, dist

    def calc_next_position(self, err:list, manipPos:list):
        self.logger.info(f"Errors: {err}" )
        self.logger.info(f"CUR_POS: {manipPos}")
        c_0_x = -0.0140625 * err[0]
        c_1_y = -0.02 * err[1]
        c_1_z = -0.00875 * err[2]
        c_2_y = -0.026 * err[1]
        c_2_z = 0.00425 * err[2]

        newPos = [
            int(manipPos[0] + c_0_x),
            int(manipPos[1] + c_1_y + c_1_z),
            int(manipPos[2] + c_2_y + c_2_z),
            self._opened_val
        ]
        self.logger.info(f"NEW_POS: {newPos}")
        return newPos
