from time import time
import threading
import json
from iqrdevice.baseresponce import BaseResponce, obj_to_dict
from typing import Optional


class ActionResponce(BaseResponce):
    def __init__(self, actName:str, rc:int, info:str=""):
        """
        name: str - action name
        rc:   int - request result code
        info: str - text interpretation of return code
        """
        BaseResponce.__init__(self, rc, info)
        self.name = actName    
    

class ActionState: 
    # name = "<имя>"
    # state = "none"    # init|run|succsess|fail
    # info = ""         # информация из callback
    # st_time = 0       # время запуска
    # fin_time = 0      # время завершения или последнее время call back
    # result = 0        # если action success – результат action

    def __init__(self, name):
        self.name = name
        self.state = "none"
        self.info = ""
        self.st_time = 0
        self.fin_time = 0
        self.result = 0

    def __copy__(self):
        res = type(self)(self.name)
        res.info = self.info
        res.state = self.state
        res.st_time = self.st_time
        res.fin_time = self.fin_time
        res.result = self.result
        return res

    def set_start(self):
        self.st_time = time()
        self.state = "init"
        self.info = ""
        self.result = 0
        self.fin_time = 0
    
    def set_run(self):
        self.state = "run"
    
    def set_success(self, code=0):
        self.state = "success"
        self.fin_time = time()
        self.result = code

    def set_fail(self, err_code=-1):
        self.state = "fail"
        self.fin_time = time()
        self.result = err_code

    def set_info(self, info):
        self.info = info
    
    def __str__(self) -> str:
        return json.dumps(self.__dict__,  default = obj_to_dict)
    
    def to_json(self):
        return json.dumps(self.__dict__,  default = obj_to_dict)


class BaseAction:
    def __init__(self, name):
        self.__thread = None
        self.__isWorking = False
        self.__state = ActionState(name)
        self.__isReset = False

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
            res = self.run_action(**kwargs)
        except Exception as e:
            res = -10
            self.__state.set_info(str(e))

        if not self.__isReset:
            if res == 0:
                self.__state.set_success(res)
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