import threading
import requests
import time


class BaseDeviceState:
    def __init__(self, name):
        self.name = name
        self.state = "No information"
        self.connected = False
        self.active_action = "none"
        self.action_start_time = 0
        self.actions_list = []
   
    def __str__(self) -> str:
        res  = "{"
        res += f"\"name\": \"{self.name}\", "
        res += f"\"state\": \"{self.state}\", "
        res += f"\"connected\": \"{self.connected}\", "
        res += f"\"active_action\": \"{self.active_action}\""
        res += "}"

        return res
    
    def set_state(self, state:str):
        self.state = state

    def set_connected(self):
        self.connected = True

    def set_disconnected(self, errorText=None):
        if errorText is not None:
            self.state = "Disconnected: " + errorText
        else:
            self.state = "Disconnected"

    def set_active_action(self, actionName:str, startTime):
        self.active_action = actionName
        self.action_start_time = startTime
    
    def set_actions_list(self, actionsList):
        self.actions_list = actionsList

    def __copy__(self):
        res = type(self)(self.name)
        res.state = self.state
        res.connected = self.connected
        res.active_action = self.active_action
        res.action_start_time = self.action_start_time
        res.actions_list = self.actions_list
        return res


class BaseDevice:
    def __init__(self, addr, name, updateStateInterval=100):
        self.addr = addr
        self.name = name
        self.__updateStateInterval = updateStateInterval
        self.__thread = None
        self._isWorking = False
        self.__state = BaseDeviceState(name)
        self.__lastUpdateTime = 0

    @property
    def State(self):
        return self.__state.__copy__()
    
    @property
    def IsWorking(self):
        return self._isWorking or self.__thread is not None

    @property
    def Name(self):
        return self.name

    def start(self):
        if self.__thread is None:
            self.__thread = threading.Thread(target=self.__thread_work)
            self._isWorking = True
            self.__thread.start()

    def __thread_work(self):
        while self._isWorking:
            if time.time() - self.__lastUpdateTime > self.__updateStateInterval:
                self.update_state()
            time.sleep(0.02)

        self.__thread = None
        self._isWorking = False
    
    def update_state(self):
        params = dict()
        url = self.addr + "/run?cmd=status&"
        try:
            resp = requests.get(url=url, params=params)
            self.__lastUpdateTime = time.time()
            data = resp.json()
            self.__parse_state(data)
        except Exception as e:
            self.__state.set_disconnected(str(e))

    def send_command(self, cmdName)->int:
        params = dict()
        url = self.addr + f"/run?cmd={cmdName}&"
        try:
            resp = requests.get(url=url, params=params)
            self.__lastUpdateTime = time.time()
            data = resp.json()
            self.__parse_state(data)
            return int(data['rc'])
        except Exception as e:
            self.__state.set_disconnected(str(e))
            return -3

    def __parse_state(self, data):
        self.__state.set_connected()
        if data is None:
            self.__state.set_state("none")
            self.__state.set_actions_list([])
            return

        self.__state.set_actions_list(data['action_list'])
        status = "waiting"
        active_action = "none"
        active_action_st = 0
        for act in data['action_list']:
            if act['state'] == "run":
                active_action = act['name']
                active_action_st = act['st_time']
                status = f"busy with action: {act['name']}"
        self.__state.set_state(status)
        self.__state.set_active_action(active_action, active_action_st)


    def stop(self):
        if self.__thread is not None:
            self._isWorking = False


if __name__ == '__main__':
    dev = BaseDevice(
        "http://192.168.1.80:5001/",
        "wheeltec",
        100
    )
    dev.start()
    while True:
        print(dev.State)
        line = input()
        
        if len(line) < 2:
            break

        print(dev.send_command(line))
    
    dev.stop()