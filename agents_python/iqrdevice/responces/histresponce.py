from .baseresponce import BaseResponce


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
