import time
import threading

class RobotApi(object):
    def __init__(self):
        self.__thread = None
        self.__waitForCommand = True
        self.__moveStatus = -1
        self.__doneEvent = threading.Event()
        self.__doneEvent.clear()


    def reset(self):
        self.__waitForCommand = True
        self.__doneEvent.clear()
        self.__moveStatus = -1


    def move_robot(self):
        if self.__thread is None:
            self.__thread = threading.Thread(target=self.__thread_work)
            self.__thread.start()


    @property
    def status(self):
        if self.__waitForCommand:
            return ("waiting", 0)
        elif not self.__doneEvent.is_set():
            return ("working", 0)
        else:
            return ("done", self.__moveStatus)


    def __thread_work(self):
        self.__waitForCommand = False
        print("TEST MSG: MOVE ROBOT")
        
        time.sleep(5)
        self.__moveStatus = 5
        self.__doneEvent.set()
        self.__thread = None
        