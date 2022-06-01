import cv2
import numpy as np
from .basefilter import BaseFilter


class FlipFilter(BaseFilter):
    def __init__(self, flipX, flipY):
        super().__init__()
        self.__flipX = flipX
        self.__flipY = flipY

    def process_frame(self, frame: np.ndarray) -> np.ndarray:
        if self.__flipX:
            frame = cv2.flip(frame, 0)
        if self.__flipY:
            frame = cv2.flip(frame, 1)
        return frame


__all__ = [ 'FlipFilter' ]
