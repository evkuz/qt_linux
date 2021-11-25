from iqrdevice.baseresponce import BaseResponce


class StatusResponce(BaseResponce):
    def __init__(self, name:str, status:str, rc:int, info:str="", action_list:list=[]):
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
    
    def set_action_list(self, action_list):
        self.action_list = action_list
