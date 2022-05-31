from .baseresponce import BaseResponce


class ServiceResponce(BaseResponce):
    def __init__(self, name:str, rc:int, info:str="", data={}):
        """
        name: str - service name
        rc:   int - request result code
        info: str - text interpretation of return code
        data: dict - set of named values ar array
        """
        BaseResponce.__init__(self, rc, info)
        self.name = name
        self.data = data
    
    def set_data(self, data):
        self.data = data