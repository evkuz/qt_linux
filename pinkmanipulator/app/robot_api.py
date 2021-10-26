import time
import threading
import json
from . import serial_communication
from . import camera
from .serial_communication import SerialCommunication
from .camera import CameraDetector
from .action import BaseAction


class RobotStatus(object):
    """
    status:{
        rc:”return code”, // return code от запроса
        // service_return:”результат вызова сервиса ”,  //если ответ на команду run?service=
        action_list:[<массив action>],
        // state:<init|run|fail> //init – ноды запускаются, run – робот активен и в работе, fail – не функционален
    }
    """
    def __init__(self, rc, action_state_list):
        self.rc = rc
        self.actions_list = action_state_list

    def toJSON(self):
        return json.dumps(self, default=lambda o: o.__dict__,
                          sort_keys=True, indent=4)
    
    def __str__(self) -> str:
        res  = "{\n"
        res += f"    \"rc\": \"{self.rc}\",\n"
        res += "    \"action_list\": \n        [\n"
        first = True
        for act in self.actions_list:
            if first:
                res+=f"{act}"
                first = False
            else:
                res+= f",\n{act}"
        res += "\n        ]\n}\n"

        return res


class CatchCubeAction(BaseAction):
    def __init__(self, camera, serial):
        BaseAction.__init__(self, "catchcube")
        self.__camera = camera
        self.__serial = serial
        self.__pixToDegreeX = 60.0 / self.__camera.FrameWidth
        self.__pixToDegreeY = 50.0 / self.__camera.FrameHeight
        self.__pixToDegreeZ = 0.015

    def move_manip(self, pos1, pos2, pos3, pos4):
        pos = self.__serial.send_command(pos1, pos2, pos3, pos4)
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return pos

    def run_action(self):
        res = 0
        tp_state = 0
        while self._isWorking:
            if tp_state == 0:
                detected, x, y, w, h = self.__camera.get_position()
                currentPos = self.__serial.get_state()
                if detected:
                    errZ = self.__camera.FrameWidth - w
                    errX = x - self.__camera.FrameWidth / 2
                    errY = y - self.__camera.FrameHeight / 2
                    pos1 = int(currentPos[0] - 0.2*(self.__pixToDegreeX * errX))
                    pos2 = int(currentPos[1] - 0.2*(self.__pixToDegreeY*errY + self.__pixToDegreeZ*errZ))
                    pos3 = int(currentPos[2] - 0.3*(self.__pixToDegreeY*errY - self.__pixToDegreeZ*errZ))
                    pos4 = 180
                    currentPos = self.move_manip(pos1, pos2, pos3, pos4)
                if currentPos[4] == 1:
                    _ = self.move_manip(
                        currentPos[0],
                        currentPos[1],
                        currentPos[2],
                        100
                    )
                    tp_state+=1
            if tp_state == 1:
                pos = self.move_manip(
                    currentPos[0],
                    currentPos[1] + 20,
                    currentPos[2] + 10,
                    100
                )
                pos = self.move_manip(10, 120, 60, 100)
                break
        return res
    
    def reset_action(self):
        pos = self.__serial.go_to_start()
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return -126


class PutCubeAction(BaseAction):
    def __init__(self, camera, serial):
        BaseAction.__init__(self, "putcube")
        self.__camera = camera
        self.__serial = serial

    def move_manip(self, pos1, pos2, pos3, pos4):
        pos = self.__serial.send_command(pos1, pos2, pos3, pos4)
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return pos

    def run_action(self):
        res = 0
        currentPos = self.__serial.get_state()
        pos = self.move_manip(
            currentPos[0],
            currentPos[1] + 20,
            currentPos[2] + 10,
            100
        )
        _ = self.move_manip(10, 120, 60, 100)
        _ = self.move_manip(10, 60, 60, 180)
        _ = self.move_manip(10, 120, 60, 180)
        pos = self.__serial.go_to_start()
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return res
    
    def reset_action(self):
        pos = self.__serial.go_to_start()
        self._set_state_info(f"position: ({pos[0]}, {pos[1]}, {pos[2]}, {pos[3]}, {pos[4]})")
        return -126


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
