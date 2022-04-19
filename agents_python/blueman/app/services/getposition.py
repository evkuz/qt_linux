from iqrdevice.services import BaseService
from ..utils import CameraDetector


class GetPositionService(BaseService):
    def __init__(self, cam:CameraDetector):
        BaseService.__init__(self, "getposition")
        self.__cam = cam
    
    def get_data(self, **kwargs):
        pos = self.__cam.get_position()
        res = {
            "detected": pos[0],
            "x": pos[1],
            "y": pos[2],
            "width": pos[3],
            "height": pos[4],
        }
        return res

    def get_info(self) -> dict:
        return self.make_info(
            "Returns cube detection result"
        )
