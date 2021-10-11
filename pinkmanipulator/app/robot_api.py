import time
import threading
import json
from subprocess import Popen
from threading  import Thread
from serial_communication import SerialCommunication
from camera import CameraDetector


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
        self.__camera = camera
        self.__status = RobotStatus("wait", 0, "none")
        self.__serial = SerialCommunication(port=port)
        self.__serial.open_device()
        self.__thread = None
        self.__isWorking = False
        self.__pixToDegreeX = 60.0 / self.__camera.FrameWidth
        self.__pixToDegreeY = 50.0 / self.__camera.FrameHeight
        self.__pixToDegreeZ = 0.01

    def reset(self):
        if self.__thread is not None:
            self.__isWorking = False

        self.__status.status = "wait"
        self.__status.active_command = "reset"
        self.__status.return_code = 0
        self.__status.comment = ""

    def catch_cube(self, cmdName):
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
        
        self.__status.status = "inprogress"
        res = 0
        
        while self.__isWorking:
            detected, x, y, w, h = self.__camera.get_position()
            currentPos = self.__serial.get_state()
            if detected:
                pos1 = currentPos[0] + 0.1*(self.__pixToDegreeX * (x - self.__camera.FrameWidth / 2))
                pos2 = currentPos[1] + 0.1*(self.__pixToDegreeY * (y - self.__camera.FrameHeight / 2))
                pos3 = currentPos[2] #+ self.__pixToDegreeZ * (self.__camera.FrameWidth - w)
                pos4 = 180
                currentPos = self.__serial.send_command(pos1, pos2, pos3, pos4)
            if currentPos[4] == 1:
                self.__serial.send_command(
                    currentPos[0],
                    currentPos[1],
                    currentPos[2],
                    20
                )
                break

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

if __name__ == '__main__':
    ports = SerialCommunication.serial_ports()
    for p in ports:
        print(p)
    
    camera = CameraDetector(1, showWindow=True)

    rob = RobotApi(camera=camera, port=ports[0])
    time.sleep(1)
    rob.catch_cube("start")
    
    while True:
        line = input()
        if line == "q":
            break
        if line == "r":
            rob.reset()
        if line == "s":
            rob.catch_cube("start")

        print(rob.status.status)
