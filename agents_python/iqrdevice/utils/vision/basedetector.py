from .basecamera import BaseCamera


class BaseDetector(object):
    def __init__(self):
        pass
    
    @property
    def EmptyResult(self):
        return {
            'detected' : False,
            'x' : 0.5,
            'y' : 0.5,
            'width' : 0,
            'height' : 0
        }

    def calibrate(self, frame, x1:int, y1:int, x2:int, y2:int)->str:
        """makes detector calibration

        Args:
            frame (_type_): frame that detector calibrates on
            x1 (int): top left X value
            y1 (int): top left Y value
            x2 (int): bottom right X value
            y2 (int): bottom right Y value

        Raises:
            NotImplementedError: must be reimplemented in derived
        Returns:
            str: information about calibration
        """
        raise NotImplementedError()

    def detect(self, image, draw:bool=False)->dict:
        """Detect object on image

        Args:
            image: image frame to detect object on
            draw (bool, optional): if set to True, output dictionary will contain key 'output' with drowen detection results. Defaults to True.

        Raises:
            NotImplementedError: This method must be implemented in derived

        Returns:
            dict: result dictionary that must contain keys: 
                * 'detected' - bool value
             Addition mandatory keys in case of 'detected' is True
                * 'x' - float value of object center x in range [0,1]
                * 'y' - float value of object center y in range [0,1]
                * 'width' - float value of object width in range [0,1]
                * 'height' - float value of object height in range [0,1]
        """
        raise NotImplementedError()

    def draw_result(self, frame, detection_result:dict):
        """Draws given result of detection on given frame

        Args:
            frame: frame to draw result
            detection_result (dict): result of detection

        Raises:
            NotImplementedError: Must be implemented in derived
        """
        raise NotImplementedError()


__all__ = [ 'BaseDetector' ]
