import subprocess
import os
import signal

def kill_process(pid):
    ps_output = subprocess.run(['pgrep', '-P', str(pid)], stdout=subprocess.PIPE, encoding='utf8')
    child_process_ids = [int(line) for line in ps_output.stdout.splitlines()]
    for p in child_process_ids:
        os.kill(p, signal.SIGKILL)


class RunMVRasp:
    def __init__(self):
        self.__script_path = "runmvrasp.sh"
        self.__process = None

    def run(self) -> bool:
        if self.__process is not None:
            return False
            
        command = ["/bin/bash", self.__script_path]
        try:
            self.__process = subprocess.Popen(
                command,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
        except Exception as e:
            print("Error:", e)
            self.stop()
            return False
        
        return True

    def stop(self) -> bool:
        if self.__process is not None:
            kill_process(self.__process.pid)
            self.__process.terminate()
            self.__process = None

        return True
