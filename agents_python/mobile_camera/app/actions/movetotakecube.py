from time import sleep

from iqrdevice.action import BaseAction
from iqrdevice.utils import RemoteDevice
from ..services import CamDetectorService


TAKE_CUBE_MIN_DIST = 110
TAKE_CUBE_MAX_DIST = 220
TAKE_CUBE_CENTER_DIST = (TAKE_CUBE_MAX_DIST - TAKE_CUBE_MIN_DIST)/2



class MoveToTakeCube (BaseAction):
    def __init__(self, remote_addr:str, getPosService:CamDetectorService, manipAddr:str):
        BaseAction.__init__(self, "movetotakecube")
        self.get_position = getPosService.get_data
        self.remoteDevice = RemoteDevice(remote_addr)
        self.manipDevice = RemoteDevice(manipAddr)

    def get_info(self) -> dict:
        return self.make_info(
            "Checks cube position and sends move commands to robot untill it will be at right position"
        )

    def run_action(self, **kwargs) -> int:
        while self._workingFlag:
            pos = self.get_position()
            pos = self.get_position()
            if pos['detected']:
                self._set_state_info(f"Cube detected: dist={pos['distance']}, angle={pos['err_angle']}")
                if abs(pos['err_angle']) < 2:
                    if TAKE_CUBE_MIN_DIST < pos['distance'] < TAKE_CUBE_MAX_DIST:
                        break
                    else:
                        dist = (pos['distance'] - TAKE_CUBE_CENTER_DIST)
                        
                        dist = 0.01 if dist < 0 else -0.02
                        self.remoteDevice.manual_request(f"/run?d={dist}")
                        #self.remoteDevice.wait_for_action_finished()
                else:
                    angle = 1.2 if pos['err_angle'] > 0 else -1.2
                    self.remoteDevice.manual_request(f"/run?a={angle}")
                    sleep(2)
                    #self.remoteDevice.wait_for_action_finished()
            else:
                self._set_state_info("Cube hasn't been detected")
                sleep(0.1)
        
        self._set_state_info("Robot has arrived at the TakeCube position")
        self.manipDevice.manual_request("/run?cmd=get_box&")

        return 0

    def reset_action(self) -> int:
        return -126

