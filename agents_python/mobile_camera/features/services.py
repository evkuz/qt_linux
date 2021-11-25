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
            "y": pos[2],
            "width": pos[3],
            "height": pos[4],
            "distance_y": self.fit_y_func(pos[2]),
            "distance_h": self.fit_h_func(pos[4])
        }
        return res

    def get_info(self) -> dict:
        return {"name": self.Name}

    def fit_y_func(self, x):
        coeffs = [2.07066421e-03, 5.98578944e-01, 5.92908052e+01, 2.12814072e+03]
        return coeffs[0] * x * x * x + coeffs[1] * x * x + coeffs[2] * x + coeffs[3]
    
    def fit_h_func(self, x):
        coeffs = [-5.27230123e-05, 3.18824287e-02, -6.93440162e+00, 6.52666043e+02]
        return coeffs[0] * x * x * x + coeffs[1] * x * x + coeffs[2] * x + coeffs[3]
    
