import cv2
import numpy as np
from .basecamera import BaseCamera
from typing import List
from .basefilter import BaseFilter


class OpenCVCamera(BaseCamera):
    def __init__(self, camPort, width:int=640, height:int=480, filters:List[BaseFilter]=[]):
        super().__init__(filters=filters)
        self.port = camPort
        self._FrameWidth = width
        self._FrameHeight = height
        self.__cap = cv2.VideoCapture()

    @property
    def isOpened(self):
        return self.__cap.isOpened()

    @property
    def FrameWidth(self) -> int:
        return self._FrameWidth

    @property
    def FrameHeight(self) -> int:
        return self._FrameHeight

    def _read_frame(self)->np.ndarray:
        _, frame = self.__cap.read()
        return frame

    def _open_device(self, nTries=3):
        if self.isOpened:
            self.__cap.release()

        for i in range(nTries):
            self.__cap.open(self.port)
            if self.__cap.isOpened():
                break
        else:
            msg = f"Can't open camera port {self.port}!"
            print(msg)
            raise RuntimeError(msg)
        if self._FrameWidth is not None:
            self.__cap.set(cv2.CAP_PROP_FRAME_WIDTH, self._FrameWidth)
        if self._FrameHeight is not None:
            self.__cap.set(cv2.CAP_PROP_FRAME_HEIGHT, self._FrameHeight)
        self.__cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.0)
        self.__cap.set(cv2.CAP_PROP_EXPOSURE, 0.60)
        self.__cap.set(cv2.CAP_PROP_AUTO_WB, 0.0)
        self.__cap.set(cv2.CAP_PROP_AUTOFOCUS, 0.25)

    def _close_device(self):
        self.__cap.release()
       
__all__ = ['OpenCVCamera']


if __name__ == '__main__':
    c = OpenCVCamera(0)
    while True:
        c.wait_for_new_frame()
        frame = c.get_last_frame()
        cv2.imshow("Camera test", frame)
        key = cv2.waitKey(1)
        if key & 0xFF == ord('q'):
            break