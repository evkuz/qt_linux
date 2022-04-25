import logging
import subprocess
from .baseaction import BaseAction
from typing import Optional, List
import os
import signal

def kill_process(pid):
    ps_output = subprocess.run(['pgrep', '-P', str(pid)], stdout=subprocess.PIPE, encoding='utf8')
    child_process_ids = [int(line) for line in ps_output.stdout.splitlines()]
    for p in child_process_ids:
        os.kill(p, signal.SIGKILL)

class ExecuteFileAction (BaseAction):
    def __init__(self, name:str, program:str, args:List[str]=[]):
        """_summary_

        Args:
            name (str): action_name, will be in actionslist
            program (str): name or path of program that will be performed
            args (list, optional): _description_. Defaults to [].
            blocking_actions (List[BaseAction], optional): actions that cannot be performed simultainiosly with this action. Defaults to [].
        """
        BaseAction.__init__(self, name)
        self.program = program
        self.additional_args = args
        self.__process = None

    def get_info(self) -> dict:
        """Returns description of service if form of dict
        Returns:
            dict: "name": self.Name, ["parameter":"description", ...]
        """ 
        descr = f"Performs {self.program}"
        if len(self.additional_args) > 0:
            descr += f" with args: {self.additional_args}"

        return self.make_info(descr)

    def run_action(self, **kwargs) -> int:
        """[return result code]
        """
        res = 0
        command = [self.program]
        if len(self.additional_args) > 0:
            command += self.additional_args
    
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
            try:
                kill_process(self.__process.pid)
            except Exception as e:
                logging.error((str(e)))

        return -126

    def process_line(self, line):
        self.line = line

if __name__ == '__main__':
    a = ExecuteFileAction("test", "/bin/bash", ["-c", "cat ~/.bashrc"])
    res = a.run_action()
    print(a.State)
    exit(res)