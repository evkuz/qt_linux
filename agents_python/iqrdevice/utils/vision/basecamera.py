import time
import threading
import logging
import numpy as np
from typing import List
from .basefilter import BaseFilter


class BaseCamera(object):
    """Base camera class. Implements basic functions of camera with frame read loop.

    Args:
        object (_type_): _description_
    """
    def __init__(self, filters:List[BaseFilter]=[], auto_close_time:int=10):
        """creates new base camera object

        Args:
            filters (List[BaseFilter], optional): list of frame filters in order they will be applied. Defaults to [].
            auto_close_time (int, optional): number of seconds that camera device will be closed after if you didn't call get_actual_frame method. Defaults to 10.
        """
        self.__filters = filters
        self.auto_close_time = auto_close_time
        self.__emptyFrame = np.zeros((640,480,3), dtype='uint8')
        self.__actualFrame = self.__emptyFrame
        self.last_access = 0
        self.__thread = None
        self.__isWorking = False
        self.__waitForFrame = threading.Condition()
        self.__lock = threading.Lock()
    
    def __del__(self):
        self.__stop()
        
    def add_filter(self, filter:BaseFilter):
        self.__filters.append(filter)

    @property
    def Filters(self):
        return self.__filters

    def __start(self):
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
        self.__isWorking = True
        logging.info("Camera thread was started")
        try:
            self._open_device()
            while self.__isWorking:
                with self.__waitForFrame:
                    frame = self._read_frame()
                    for f in self.__filters:
                        frame = f.process_frame(frame)    
                    with self.__lock:
                        self.__actualFrame = frame.copy()
                    self.__waitForFrame.notifyAll()
                time.sleep(0) # this should help to switch thread
                if time.time() - self.last_access > self.auto_close_time:
                    break
            self._close_device()
        except Exception as e:
            logging.error(str(e))
        finally:
            self.__thread = None
            self.__isWorking = False
            logging.info("Camera thread was stopped")

    @property
    def isStarted(self):
        return self.__isWorking
        
    def wait_for_new_frame(self, timeout=None):
        """waites for new frame read from camera

        Args:
            timeout (_type_, optional): float number of seconds. Defaults to None.

        Raises:
            RuntimeError: if timeout was specified and exceeded
        """
        if self.__thread is None:
            self.__start()
       
        with self.__waitForFrame:
            if not self.__waitForFrame.wait(timeout=timeout):
                raise RuntimeError("Can't get frame at given time!")

    def get_last_frame(self):
        """return last read frame

        Returns:
            np.ndarray: frame
        """
        if self.__thread is None:
            self.__start()
        self.last_access = time.time()
        with self.__lock:
            if self.__actualFrame is not None:
                return self.__actualFrame.copy()
            return self.__emptyFrame.copy()

    def _read_frame(self):
        raise NotImplementedError()
    
    def _open_device(self, nTries=3):
        raise NotImplementedError()

    def _close_device(self):
        raise NotImplementedError()


__all__ = [ 'BaseCamera' ]
