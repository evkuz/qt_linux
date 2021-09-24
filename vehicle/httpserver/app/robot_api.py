import time
import threading
import json


class RobotStatus(object):
    def __init__(self, status, lastCmd, lastCmdResult):
        self.status = status
        self.lastCmd = lastCmd
        self.lastCmdResult = lastCmdResult

    def __copy__(self):
        return type(self)(self.status, self.lastCmd, self.lastCmdResult)

    def toJSON(self):
        return json.dumps(self, default=lambda o: o.__dict__,
                          sort_keys=True, indent=4)


class RobotApi(object):
    def __init__(self):
        self.__thread = None
        self.__waitForCommand = True
        self.__doneEvent = threading.Event()
        self.__doneEvent.clear()
        self.__status = RobotStatus("wait", "none", 0)

    def reset(self):
        self.__waitForCommand = True
        self.__doneEvent.clear()
        self.__status.status = "wait"
        self.__status.lastCmd = "reset"
        self.__status.lastCmdResult = 0

    def move_robot(self, cmdName):
        if self.__thread is None:
            self.__status.status = "init"
            self.__status.lastCmd = cmdName
            self.__status.lastCmdResult = 0
            self.__thread = threading.Thread(target=self.__thread_work)
            self.__thread.start()

    @property
    def status(self):
        return self.__status.__copy__()

    def __thread_work(self):
        print("TEST MSG: MOVE ROBOT")
        self.__status.status = "inprogress"
        
        time.sleep(5)

        self.__status.status = "done"
        self.__status.lastCmdResult = 5
        self.__doneEvent.set()

        self.__thread = None
