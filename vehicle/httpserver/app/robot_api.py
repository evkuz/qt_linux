import time
import threading
import json
from subprocess import PIPE, Popen
from threading  import Thread
from subprocess import check_call
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
        self.__waitForCommand = True
        self.__status = RobotStatus("wait", 0, "none")

    def reset(self):
        self.__waitForCommand = True
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
        
        time.sleep(1)

        self.__status.status = "inprogress"

        filePath = "/home/xrrobot/sss/spoint.py"
        #filePath = "/home/miksarus/projects/iqr_lit/vehicle/httpserver/app/testrobotmove.py"
        command = ['python2.7', filePath]
        res = 0
        try:
            res = check_call(command)
        except Exception as e:
            print("Error:", e)
            res = -1

        # p = Popen(command, stdout=PIPE)
        
        # Thread(target=self.__read_pipe, args=(p.stdout,)).start()
        
        # try:
        #     res = p.wait() #check_call()
        # except Exception as e:
        #     print("Error:", e)

        self.__status.status = "done"
        self.__status.return_code = res

        self.__thread = None

    def __read_pipe(self, out, ntrim=80):    
        out.flush()
        for line in out:
            self.__status.comment="line"
            out.flush()