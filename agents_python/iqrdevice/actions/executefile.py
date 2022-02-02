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
        return self.make_info(
            f"Perform script {self.scriptPath} with {self.program}"
        )

    def run_action(self, **kwargs) -> int:
        """[return result code]
        """
        res = 0
        command = [self.program, self.scriptPath] + self.additional_args
        self.__process = Popen(command)
        self._set_state_info(f"Process was started with PID {self.__process.pid}")
        res = 0
        try:
            res = self.__process.wait()
        except Exception as e:
            print("Error:", e)
            res = -1
        self._set_state_info(f"Process finished")
        self.__process = None
        return res

    def reset_action(self) -> int:
        """[return result code -126 by default]
        """
        if self.__process is not None:
            self.__process.terminate()
        return -126

