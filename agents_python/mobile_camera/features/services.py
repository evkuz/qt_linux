from iqrdevice.service import BaseService
from features import device, camera


class CamDetectorService(BaseService):
    def __init__(self, cam:camera.CameraDetector):
        BaseService.__init__(self, "getposition")
        self.__cam = cam
    
    def get_data(self, **kwargs):
        res = self.__cam.get_position()

    def get_info(self) -> dict:
        return {"name": self.Name}
