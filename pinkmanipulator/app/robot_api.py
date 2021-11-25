import time
from . import camera
from .camera import CameraDetector
from .action import BaseAction
from .status import RobotStatus
from .serial_communication import SerialCommunication
from .robot_actions import CatchCubeAction, PutCubeAction


class RobotApi(object):
    def __init__(self, camera:CameraDetector, port="/dev/ttyACM0"):
        self.__camera = camera
        self.__serial = SerialCommunication(port=port)
        self.__serial.open_device()
        
        self.__actions_list = [
            CatchCubeAction(self.__camera, self.__serial),
            PutCubeAction(self.__camera, self.__serial)
        ]

    def __make_status(self, rc):
        act_states = []
        for act in self.__actions_list:
            act_states.append(act.State)
        return RobotStatus(rc, act_states)

    def reset(self):
        for act in self.__actions_list:
            if act.IsWorking:
                act.reset()
        return self.__make_status(0)
    
    def run_action(self, action_name):
        rc = -1
        for act in self.__actions_list:
            if act.Name == action_name:
                rc = act.run()
                break
        return self.__make_status(rc)

    @property
    def status(self):
        return self.__make_status(0)


if __name__ == '__main__':
    ports = SerialCommunication.serial_ports()
    for p in ports:
        print(p)
    
    camera = CameraDetector(2, showWindow=True)

    rob = RobotApi(camera=camera, port=ports[0])
    time.sleep(1)
    rob.run_action("catchcube")
    
    while True:
        line = input()
        if line == "q":
            break
        if line == "r":
            rob.reset()
        if line == "p":
            rob.run_action("putcube")
        if line == "s":
            rob.run_action("catchcube")

        print(rob.status)
