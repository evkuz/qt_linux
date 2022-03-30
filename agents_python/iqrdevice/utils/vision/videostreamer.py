import time
import threading
import logging
from .basecamera import BaseCamera
from .basedetector import BaseDetector
import cv2


class VideoStreamer(object):
    def __init__(self, camera:BaseCamera, detector=None):
        """Constructor

        Args:
            camera (BaseCamera): camera to get frames from
            detector (BaseDetector, optional): detector the results of wich will be drawen on images. Defaults to None.
        """
        self.__camera = camera
        self.__detector = detector
        self.__detector_result = { 'detected': False }
        self.__thread = None
        self.__isWorking = False
        self.__needToDetect = threading.Event()
        self.last_access = 0
        self.auto_close_time = 10

    def __del__(self):
        self.__stop()

    def __start(self):
        if self.__detector is None:
            return
        if self.__thread is not None:
            return
        self.last_access = time.time()
        self.__thread = threading.Thread(target=self.__thread_work)
        self.__isWorking = True
        self.__thread.start()

    def __stop(self):
        if self.__thread is None:
            return
        self.__isWorking = False
        self.__thread.join()

    def __thread_work(self):
        if self.__detector is None:
            return
        self.__isWorking = True
        try:
            logging.info("Streamer detector thread was started")
            while self.__isWorking:
                if self.__needToDetect.is_set():
                    self.__needToDetect.clear()
                    self.__detector.detect(self.__camera.get_last_frame(), False)
                time.sleep(0) # this should help to switch thread
                if time.time() - self.last_access > self.auto_close_time:
                    break
        except Exception as e:
            logging.error(str(e))
        finally:
            self.__thread = None
            self.__isWorking = False
            logging.info("Streamer detector thread was stopped")

    def __update_detection(self):
        self.__needToDetect.set()

    def draw_detector_result(self, frame):
        if self.__detector is None:
            return frame
        frame = self.__detector.draw_result(frame, self.__detector_result)
        return frame

    def get_next_frame(self):
        if self.__thread is None:
            self.__start()
        self.last_access = time.time()
        
        self.__camera.wait_for_new_frame()
        self.__update_detection()
        frame = self.__camera.get_last_frame()
        frame = self.draw_detector_result(frame)
        (flag, encoded_frame) = cv2.imencode(".jpeg", frame)

        return encoded_frame

