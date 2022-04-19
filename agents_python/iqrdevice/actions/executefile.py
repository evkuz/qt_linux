import subprocess
from .baseaction import BaseAction
from typing import Optional
import os
import signal

def kill_process(pid):
    ps_output = subprocess.run(['pgrep', '-P', str(pid)], stdout=subprocess.PIPE, encoding='utf8')
    child_process_ids = [int(line) for line in ps_output.stdout.splitlines()]
    for p in child_process_ids:
        os.kill(p, signal.SIGKILL)

class ExecuteFileAction (BaseAction):
    def __init__(self, name:str, program:str, scriptPath:Optional[str]=None, args:list=[]):
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
        descr = ""
        if self.scriptPath is None:
            descr = f"Performs {self.program}"
        else:
            descr = "Perform script {self.scriptPath} with {self.program}"

        if len(self.additional_args) > 0:
            descr += f" with args: {self.additional_args}"

        return self.make_info(descr)

    def run_action(self, **kwargs) -> int:
        """[return result code]
        """
        res = 0
        command = [self.program]
        if self.scriptPath is not None:
            command.append(self.scriptPath)
        if len(self.additional_args) > 0:
            command += self.additional_args
    
        print(command)
        self.__process = subprocess.Popen(command)
        self._set_state_info(f"Process was started with PID {self.__process.pid}")
        
        res = 0
        try:
            res = self.__process.wait()
        except Exception as e:
            print("Error:", e)  
            res = -1
        
        self._set_state_info("Process finished")
        self.__process = None
        return res

    def reset_action(self) -> int:
        """[return result code -126 by default]
        """
        if self.__process is not None:
            kill_process(self.__process.pid)
            self.__process.terminate()

        return -126

    def process_line(self, line):
        self.line = line

if __name__ == '__main__':
    a = ExecuteFileAction("test", "/bin/bash", None,["-c", "cat ~/.bashrc"])
    res = a.run_action()
    print(a.State)
    exit(res)