from time import sleep

from iqrdevice.service import BaseService
from ..utils import CameraDetector


class CamCalibService (BaseService):
    def __init__(self, cam:CameraDetector):
        BaseService.__init__(self, "camcalib")
        self.cam = cam

    def get_info(self) -> dict:
        res = {
            "name":self.Name,
            "parameters": {
                "x1":"(int) - rectungle x1 position in px",
                "y1":"(int) - rectungle y1 position in px",
                "x2":"(int) - rectungle x2 position in px",
                "y2":"(int) - rectungle y2 position in px"
            }
        }
        return res

    def get_data(self, **kwargs):
        x1, y1, x2, y2 = None, None, None, None
        if "x1" in kwargs:
            x1 = int(kwargs['x1'])
        if "y1" in kwargs:
            y1 = int(kwargs['y1'])
        if "x2" in kwargs:
            x2 = int(kwargs['x2'])
        if "y2" in kwargs:
            y2 = int(kwargs['y2'])
        
        if any((x1, y1, x2, y2)) is None:
            raise RuntimeError("Error: not all parameters was specified")

        res_str = self.cam.calibrate_colors(x1, y1, x2, y2)
        if len(res_str) == 0:
            raise RuntimeError("Can't calibrate, maby no frame")

        return res_str
