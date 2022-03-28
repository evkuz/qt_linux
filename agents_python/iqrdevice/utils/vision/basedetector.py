from .basecamera import BaseCamera


class BaseDetector(object):
    def __init__(self):
        pass

    def calibrate(self, frame, x1:int, y1:int, x2:int, y2:int):
        """makes detector calibration

        Args:
            frame (_type_): frame that detector calibrates on
            x1 (int): top left X value
            y1 (int): top left Y value
            x2 (int): bottom right X value
            y2 (int): bottom right Y value

        Raises:
            NotImplementedError: must be reimplemented in derived
        """
        raise NotImplementedError()

    def detect(self, frame):
        raise NotImplementedError()


__all__ = [ 'BaseDetector' ]
