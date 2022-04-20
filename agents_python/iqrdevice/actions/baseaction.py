from time import time
import threading
from typing import Callable, Optional
from .actionstate import ActionState


class BaseAction:
    def __init__(self, name):
        self.__thread = None
        self.__isWorking = False
        self.__state = ActionState(name)
        self.__isReset = False
        
        self.on_run:Optional[Callable[[str], None]] = None
        self.on_reset:Optional[Callable[[str], None]] = None
        self.on_successfully_finished:Optional[Callable[[str], None]] = None

    @property
    def State(self):
        return self.__state.__copy__()
    
    @property
    def IsWorking(self):
        return self.__isWorking or self.__thread is not None

    @property
    def Name(self):
        return self.__state.name

    def get_info(self) -> dict:
        """Must be redefined in child classes,
        Returns:
            dict: "name": self.Name, ["parameter":"description", ...]
        """
        raise NotImplementedError()

    def make_info(self, description:str, parameters:dict={}, statuses:Optional[dict]=None):
        if statuses is None:
            statuses = {
                "none": "action hasn't been performed yet",
                "init": "action has just been performed",
                "run": "action is performong now",
                "succsess": "action finished succesfully",
                "fail": "action failed"
            }
        return {
            "name":self.Name,
            "description": description,
            "parameters": parameters,
            "statuses": statuses
        }

    def run(self, **kwargs) -> int:
        if self.__thread is None:
            self.__thread = threading.Thread(target=self.__run_thread_work, kwargs=kwargs)
            self.__state.set_start()
            self.__isWorking = True
            self.__isReset = False
            self.__thread.start()
            return 0
        return -2

    def __run_thread_work(self, **kwargs):
        self.__state.set_run()
        try:
            if self.on_run is not None:
                self.on_run(self.Name)
            res = self.run_action(**kwargs)
        except Exception as e:
            res = -10
            self.__state.set_info(str(e))

        if not self.__isReset:
            if res == 0:
                self.__state.set_success(res)
                if self.on_successfully_finished is not None:
                    self.on_successfully_finished(self.Name)
            else:
                self.__state.set_fail(res)
        
        self.__thread = None
        self.__isWorking = False
    
    def run_action(self, **kwargs) -> int:
        """[return result code]

        Raises:
            NotImplementedError: [description]
        """
        raise NotImplementedError()

    def reset(self):
        if self.IsWorking:
            self._isReset = True
            if self.__thread is not None:
                self.__isWorking = False
            
            self.__state.set_info("reset was send")
            t = threading.Thread(target=self.__reset_thread_work)
            t.start()
            return 0
        return -2

    def __reset_thread_work(self):
        if self.__thread is not None: self.__thread.join()
        self.__state.set_info("reset was sent")
        try:
            res = self.reset_action()
            if self.on_reset is not None:
                self.on_reset(self.Name)
        except Exception as e:
            res = -1
            self.__state.set_info("Reset_fail: " + str(e))

        self.__state.set_fail(res)

    def reset_action(self) -> int:
        """[return result code -126 by default]
        """
        return -126

    def _set_state_info(self, info):
        self.__state.set_info(info)

    @property
    def _workingFlag(self):
        return self.__isWorking

    @property
    def _resetFlag(self):
        return self.__isReset


if __name__ == "__main__":
    act = BaseAction("test")
    print(act.State)