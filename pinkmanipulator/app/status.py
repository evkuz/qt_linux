from time import time
import threading


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


class BaseAction:
    def __init__(self, name):
        self.__thread = None
        self._isWorking = False
        self.__state = ActionState(name)
        self._resetFlag = False

    @property
    def status(self):
        # if self.__thread is not None:
        #     self.__status.status = "inprogress"
        return self.__state.__copy__()

    def run(self, cmdName):
        if self.__thread is None:
            self.__thread = threading.Thread(target=self.__run_thread_work)
            self.__state.set_start()
            self._isWorking = True
            self._resetFlag = False
            self.__thread.start()

    def __run_thread_work(self):
        self.__state.set_run()
        
        res = self.run_action()
        
        self.__thread = None
        self._isWorking = False
    
    def run_action(self):
        """[return result code]

        Raises:
            NotImplementedError: [description]
        """
        raise NotImplementedError()

    def reset(self):
        self._resetFlag = True
        if self.__thread is not None:
            self._isWorking = False
        
        self.__state.set_info("reset was send")
        t = threading.Thread(target=self.__reset_thread_work)
        t.start()

    def __reset_thread_work(self):
        if self.__thread is not None: self.__thread.join()
        res = self.reset_action()
        self.__state.set_fail(res)

    def reset_action(self):
        """[return result code -126 by default]
        """
        return -126

    def set_state_info(self, info):
        self.__state.set_info(info)