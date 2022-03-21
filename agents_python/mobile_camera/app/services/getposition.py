from iqrdevice.service import BaseService
from .. import device
from ..utils import camera
from math import atan2, pi


class CamDetectorService(BaseService):
    def __init__(self, cam:camera.CameraDetector):
        BaseService.__init__(self, "getposition")
        self.__cam = cam
    
    def get_info(self) -> dict:
        return self.make_info(
            "Returns detection state and position of cube"
        )

    def get_data(self, **kwargs):
        pos = self.__cam.get_position()
        distance = self.fit_h_func(pos[4])
        err_angle = self.calc_angle(distance, pos[1], pos[3])
        res = {
            "detected": pos[0],
            "x": pos[1],
            "y": pos[2],
            "width": pos[3],
            "height": pos[4],
            "err_angle": err_angle,
            "distance": distance
        }
        return res

    def fit_h_func(self, x):
        coeffs = [-6.04361309e-05, 3.42606240e-02, -7.03365398e+00, 6.17398358e+02] #cube 30
        #coeffs = [-5.27230123e-05, 3.18824287e-02, -6.93440162e+00, 6.52666043e+02] #old cube
        return coeffs[0] * x * x * x + coeffs[1] * x * x + coeffs[2] * x + coeffs[3]

    def calc_angle(self, distance, x, cube_width):
        #real_width = 38 # mm
        if cube_width == 0:
            return 180

        x_mm = x * (38. / cube_width)
        return atan2(x_mm, distance)*180. / pi
