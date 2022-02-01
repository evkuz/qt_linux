from time import sleep

from iqrdevice.action import BaseAction
from iqrdevice.utils import RemoteDevice
from ..services import CamDetectorService


class MoveToTakeCube (BaseAction):
    def __init__(self, remote_addr:str, getPosService:CamDetectorService):
        BaseAction.__init__(self, "movetotakecube")
        self.get_position = getPosService.get_data
        self.remoteDevice = RemoteDevice(remote_addr)

    def get_info(self) -> dict:
        return {"name":self.Name}

    def run_action(self, **kwargs) -> int:
        while self._workingFlag:
            pos = self.get_position()
            if pos['detected']:
                self._set_state_info(f"Cube detected: dist={pos['distance']}, angle={pos['err_angle']}")
                if abs(pos['err_angle']) < 0.3:
                    if 140 < pos['distance'] < 200:
                        break
                    else:
                        dist = (pos['distance'] - 150) * 0.001
                        self.remoteDevice.manual_request(f"/run?d={dist}")
                        self.remoteDevice.wait_for_action_finished()
                else:
                    angle = 0.5 * pos['err_angle']
                    self.remoteDevice.manual_request(f"/run?a={angle}")
                    self.remoteDevice.wait_for_action_finished()
            else:
                self._set_state_info("Cube hasn't been detected")
                sleep(0.1)
        
        self._set_state_info("Robot has arrived at the TakeCube position")
        return 0

    def reset_action(self) -> int:
        return -126

