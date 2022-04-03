from iqrdevice.service import BaseService
from math import atan2, pi
from iqrdevice.utils.vision import OpenCVCamera, SimpleDetector


class CamDetectorService(BaseService):
    def __init__(self, cam:OpenCVCamera, detector:SimpleDetector):
        BaseService.__init__(self, "getposition")
        self.__cam = cam
        self.__detector = detector
    
    def get_info(self) -> dict:
        return self.make_info(
            "Returns detection state and position of cube"
        )

    def get_data(self, **kwargs):
        try:
            self.__cam.wait_for_new_frame()
            frame = self.__cam.get_last_frame()
            det_res = self.__detector.detect(frame)
            if det_res['detected']:
                det_res["distance"] = self.fit_h_func(det_res['height'])
                det_res["err_angle"] = self.calc_angle(det_res["distance"], det_res['x'], det_res['width'])        
        except Exception as e:
            det_res = str(e)

        return det_res

    def fit_h_func(self, x):
        coeffs = [-7800.24289301, 8765.0404947, -3580.85567724, 630.88037114] # cube 30 [0,1]
        #coeffs = [-6.04361309e-05, 3.42606240e-02, -7.03365398e+00, 6.17398358e+02] #cube 30
        #coeffs = [-5.27230123e-05, 3.18824287e-02, -6.93440162e+00, 6.52666043e+02] #old cube
        return coeffs[0] * x * x * x + coeffs[1] * x * x + coeffs[2] * x + coeffs[3]

    def calc_angle(self, distance, x, cube_width):
        #real_width = 38 # mm
        if cube_width == 0:
            return 180

        x_mm = x * (38. / cube_width)
        return atan2(x_mm, distance)*180. / pi
