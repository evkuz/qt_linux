import time
import threading
import json
from subprocess import Popen
from threading  import Thread
from .serial_communication import SerialCommunication
from .camera import CameraDetector


class RobotStatus(object):
    def __init__(self, status, return_code, active_command, comment=""):
        self.status = status
        self.return_code = return_code
        self.active_command = active_command
        self.comment = comment

    def __copy__(self):
        return type(self)(self.status, self.return_code, self.active_command, self.comment)

    def toJSON(self):
        return json.dumps(self, default=lambda o: o.__dict__,
                          sort_keys=True, indent=4)


class RobotApi(object):
    def __init__(self, camera:CameraDetector, port="/dev/ttyACM0"):
        self.__thread = None
        self.__status = RobotStatus("wait", 0, "none")
        self.__serial = SerialCommunication(port=port)
        self.__isWorking = False

    def reset(self):
        if self.__thread is not None:
            self.__isWorking = False

        self.__status.status = "wait"
        self.__status.active_command = "reset"
        self.__status.return_code = 0
        self.__status.comment = ""

    def move_robot(self, cmdName, x, y, dist):
        if self.__status.status != "wait":
            self.__status.return_code = -1
            return

        if self.__thread is None:
            self.__status.status = "init"
            self.__status.active_command = cmdName
            self.__status.return_code = 0
            self.__isWorking = True
            self.__thread = threading.Thread(target=self.__thread_work)
            self.__thread.start()

    @property
    def status(self):
        # if self.__thread is not None:
        #     self.__status.status = "inprogress"
        return self.__status.__copy__()

    def __thread_work(self):
        self.__serial.start_communication()
        self.__status.status = "inprogress"
        res = 0
        
        while self.__isWorking:


        if self.__status.active_command != "reset":
            self.__status.status = "done"
            self.__status.return_code = res
        
        self.__thread = None
        self.__isWorking = False

    def __read_pipe(self, out, ntrim=80):    
        out.flush()
        for line in out:
            self.__status.comment="line"
            out.flush()
