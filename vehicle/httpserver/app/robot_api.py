import time
import threading
import json
from subprocess import Popen
from threading  import Thread
import os


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
    def __init__(self):
        self.__thread = None
        self.__process = None
        self.__status = RobotStatus("wait", 0, "none")

    def reset(self):
        if self.__thread is not None:
            if self.__process is not None:
                self.__process.terminate()

        self.__status.status = "wait"
        self.__status.active_command = "reset"
        self.__status.return_code = 0
        self.__status.comment = ""

    def move_robot(self, cmdName):
        if self.__status.status != "wait":
            self.__status.return_code = -1
            return

        if self.__thread is None:
            self.__status.status = "init"
            self.__status.active_command = cmdName
            self.__status.return_code = 0
            self.__thread = threading.Thread(target=self.__thread_work)
            self.__thread.start()

    @property
    def status(self):
        return self.__status.__copy__()

    def __thread_work(self):
        print("TEST MSG: MOVE ROBOT")
        
        time.sleep(0.2)

        self.__status.status = "inprogress"

        filePath = "/home/xrrobot/sss/spoint.py"
        #filePath = "/home/miksarus/projects/iqr_lit/vehicle/httpserver/app/testrobotmove.py"
        command = ['python2.7', filePath]
        
        self.__process = Popen(command)
        res = 0
        try:
            res = self.__process.wait()
        except Exception as e:
            print("Error:", e)
            res = -1
   
        # Thread(target=self.__read_pipe, args=(p.stdout,)).start()

        if self.__status.active_command != "reset":
            self.__status.status = "done"
            self.__status.return_code = res

        self.__process = None
        self.__thread = None

    def __read_pipe(self, out, ntrim=80):    
        out.flush()
        for line in out:
            self.__status.comment="line"
            out.flush()