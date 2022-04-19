from .baseresponce import BaseResponce


class StatusResponce(BaseResponce):
    def __init__(self, name:str, status:str, locked:bool, rc:int, info:str="", action_list:list=[], nodes:list=[]):
        """
        name: str - device name
        rc:   int - request result code
        info: str - text interpretation of return code
        state: str - global device status: init | run | fail
        action_list: list - list of action states
        """
        BaseResponce.__init__(self, rc, info)
        self.name = name
        self.status = status
        self.action_list = action_list
        self.locked = locked
        self.nodes=nodes
    
    def set_action_list(self, action_list):
        self.action_list = action_list
