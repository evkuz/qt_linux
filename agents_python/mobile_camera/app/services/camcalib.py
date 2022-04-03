from iqrdevice.service import BaseService
from iqrdevice.utils.vision import OpenCVCamera, SimpleDetector


class CamCalibService (BaseService):
    def __init__(self, cam:OpenCVCamera, detector:SimpleDetector):
        BaseService.__init__(self, "camcalib")
        self.cam = cam
        self.detector = detector

    def get_info(self) -> dict:
        return self.make_info(
            "Calibrate camera detection color ranges",
            {
                "x1":"(int) - rectungle x1 position in px",
                "y1":"(int) - rectungle y1 position in px",
                "x2":"(int) - rectungle x2 position in px",
                "y2":"(int) - rectungle y2 position in px"
            }
        )

    def get_data(self, **kwargs):
        x1, y1, x2, y2 = None, None, None, None
        if "x1" in kwargs:
            x1 = kwargs['x1']
        if "y1" in kwargs:
            y1 = kwargs['y1']
        if "x2" in kwargs:
            x2 = kwargs['x2']
        if "y2" in kwargs:
            y2 = kwargs['y2']
        
        if any((x1, y1, x2, y2)) is None:
            raise RuntimeError("Error: not all parameters was specified")

        frame = self.cam.get_last_frame()
        res_str = self.detector.calibrate(frame, int(x1), int(y1), int(x2), int(y2))
        if len(res_str) == 0:
            raise RuntimeError("Can't calibrate, maby no frame")
        self.detector.write_color_range()
        
        return res_str
