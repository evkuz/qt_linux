import json
from iqrdevice.responces import obj_to_dict
from time import time


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