from iqrdevice.service import BaseService
from features import device, camera


class CamDetectorService(BaseService):
    def __init__(self, cam:camera.CameraDetector):
        BaseService.__init__(self, "getposition")
        self.__cam = cam
    
    def get_data(self, **kwargs):
        pos = self.__cam.get_position()
        res = {
            "detected": pos[0],
            "x": pos[1],
            "distance": pos[2]
        }
        return res

    def get_info(self) -> dict:
        return {"name": self.Name}
