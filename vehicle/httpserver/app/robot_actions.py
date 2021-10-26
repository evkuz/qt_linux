from .action import BaseAction
from subprocess import Popen


class RunRosScriptAction(BaseAction):
    def __init__(self, name, script_path):
        BaseAction.__init__(self, name)
        self.__process = None
        self.__script_path = script_path

    def run_action(self):
        res = 0
        command = ['python2.7', self.__script_path]
        self.__process = Popen(command)
        res = 0
        try:
            res = self.__process.wait()
        except Exception as e:
            print("Error:", e)
            res = -1
        return res
    
    def reset_action(self):
        if self.__process is not None:
            self.__process.terminate()
        return -126

class TransferCubeAction(RunRosScriptAction):
    def __init__(self):
        super().__init__("transfercube", "/home/xrrobot/sss/spoint.py")

class MoveBackAction(RunRosScriptAction):
    def __init__(self):
        super().__init__("moveback", "/home/xrrobot/sss/spoint.py")