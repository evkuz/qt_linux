import subprocess

class RunMVRasp:
    def __init__(self):
        self.__script_path = "runmvrasp.sh"
        self.__process = None

    def run(self) -> bool:
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
            self.__process.kill()
            self.__process = None

        return True
