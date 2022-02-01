from subprocess import Popen
from ..action import BaseAction


class ExecuteFileAction (BaseAction):
    def __init__(self, name:str, program:str, scriptPath:str, args:list=[]):
        BaseAction.__init__(self, name)
        self.program = program
        self.scriptPath = scriptPath
        self.additional_args = args
        self.__process = None

    def get_info(self) -> dict:
        """Returns description of service if form of dict
        Returns:
            dict: "name": self.Name, ["parameter":"description", ...]
        """ 
        res = {
            "name":self.Name,
            "duration": "(float) - if it was specified the action will be long for this number of seconds"
        }
        return res

    def run_action(self, **kwargs) -> int:
        """[return result code]
        """
        res = 0
        command = [self.program, self.scriptPath] + self.additional_args
        self.__process = Popen(command)
        res = 0
        try:
            res = self.__process.wait()
        except Exception as e:
            print("Error:", e)
            res = -1

        self.__process = None
        return res

    def reset_action(self) -> int:
        """[return result code -126 by default]
        """
        if self.__process is not None:
            self.__process.terminate()
        return -126

