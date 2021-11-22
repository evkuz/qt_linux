from time import time
from action import ActionState
from baseresponce import BaseResponce


class HistResponce(BaseResponce):
    def __init__(self, rc:int, info:str="", data:list=[]):
        """
        rc:   int - request result code
        info: str - text interpretation of return code
        data: list - list of objects or values
        """
        BaseResponce.__init__(self, rc, info)
        self.data = data
    
    def set_data(self, data):
        self.data = data


class History:
    def __init__(self):
        self.__actions=[]
        self.__system=[]
        
    def add_action(self, action:ActionState):
        self.__actions.append(action)

    def add_system(self, line:str):
        self.__system.append(line)

    def get_actions(self, names:list=[], n:int=20) -> HistResponce:
        if len(names) == 0:
            #TODO:
            raise NotImplementedError()
        else:
            #TODO:
            raise NotImplementedError()

    def get_system(self, n=20) -> HistResponce:
        #TODO:
        raise NotImplementedError()


if __name__ == "__main__":
    data =  [
        ActionState("test1"),
        ActionState("test2"),
        ActionState("test3"),
        ActionState("test2")
    ]

    resp = HistResponce(1, "test_response", data)
    
    print(resp.to_json())