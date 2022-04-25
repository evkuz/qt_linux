import subprocess

class RosRun:
    def __init__(self, start_script:str, stop_script:str):
        self.__start_script = start_script
        self.__stop_script = stop_script
        self.__process = None

    def run(self) -> bool:
        command = ["/bin/bash", self.__start_script]
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

        subprocess.run(
            ["/bin/bash", self.__stop_script],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False
        )

        return True
