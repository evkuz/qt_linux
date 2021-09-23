from time import time, sleep
import cv2


class TestCamera(object):
    def __init__(self):
        self.frames = [cv2.imread(f"test_camera/{i}.jpg", 1) for i in range(0,10)]
        self.frameNum = 9
        self.framesCount = len(self.frames)

    def get_frame(self):
        self.frameNum += 1
        if self.frameNum >= self.framesCount:
            self.frameNum = 0
        sleep(1)
        return self.frames[self.frameNum]