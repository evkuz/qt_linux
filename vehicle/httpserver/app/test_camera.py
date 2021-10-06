from time import time, sleep
from inspect import getsourcefile
from os.path import abspath, dirname


class TestCamera(object):
    def __init__(self):
        self.frames = [TestCamera.__read_file(i) for i in range(0,10)]
        self.frameNum = 9
        self.framesCount = len(self.frames)

    def get_frame(self):
        self.frameNum += 1
        if self.frameNum >= self.framesCount:
            self.frameNum = 0
        sleep(0)
        return self.frames[self.frameNum]

    @staticmethod
    def __read_file(file_num):
        dirPath = abspath(dirname(getsourcefile(lambda:0)))
        fileName = f"{dirPath}/test_camera/{file_num}.jpg"
        with open(fileName, 'rb') as imfile:
            data = imfile.read()
            return data