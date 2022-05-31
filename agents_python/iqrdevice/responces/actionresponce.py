from iqrdevice.responces import BaseResponce


class ActionResponce(BaseResponce):
    def __init__(self, actName:str, rc:int, info:str=""):
        """
        name: str - action name
        rc:   int - request result code
        info: str - text interpretation of return code
        """
        BaseResponce.__init__(self, rc, info)
        self.name = actName    