from .basecontroller import BaseController
from typing import List
import subprocess
import os
import signal


def kill_process(pid):
    ps_output = subprocess.run(['pgrep', '-P', str(pid)], stdout=subprocess.PIPE, encoding='utf8')
    child_process_ids = [int(line) for line in ps_output.stdout.splitlines()]
    for p in child_process_ids:
        os.kill(p, signal.SIGKILL)

class RosRun(BaseController):
    def __init__(self, start_script:str, stop_script:str):
        BaseController.__init__(self, "RosRun")
        self.__start_script = start_script
        self.__stop_script = stop_script
        self.__process = None

    @property
    def channels(self)->List[str]:
        return ['main']

    def run(self) -> bool:
        if self.__process is not None:
            return False
            
        command = ["/bin/bash", self.__start_script]
        try:
            self.__process = subprocess.Popen(
                command,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
            self.fire_event("main", True)
        except Exception as e:
            print("Error:", e)  
            self.stop()
            return False
        
        return True

    def stop(self) -> bool:
        if self.__process is not None:
            kill_process(self.__process.pid)
            self.__process.kill()
            self.__process = None

        subprocess.run(
            ["/bin/bash", self.__stop_script],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False
        )
        self.fire_event("main", False)

        return True
