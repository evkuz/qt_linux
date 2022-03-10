import subprocess

class RosRun:
    def __init__(self, scriptPath:str):
        self.scriptPath = scriptPath
        self.__process = None

    def run(self) -> bool:
        command = ["/bin/bash", self.scriptPath]
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
            self.__process.terminate()
            self.__process = None
            return True
        else:
            subprocess.run(
                ['rosnode', 'kill', '-a'],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                check=False
            )

        return True
