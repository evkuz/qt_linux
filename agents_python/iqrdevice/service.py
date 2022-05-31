from time import time
import threading
from iqrdevice.baseresponce import BaseResponce


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


class BaseService:
    def __init__(self, name):
        self.__name = name
        
    @property
    def Name(self) -> str:
        return self.__name

    def get_data(self, **kwargs):
        raise NotImplementedError()

    def get_info(self) -> dict:
        """Must be redefined in child classes,
        Returns:
            dict: "name": self.Name, ["parameter":"description", ...]
        """
        raise NotImplementedError()

    def make_info(self, description:str, parameters:dict={}):
        return {
            "name":self.Name,
            "description": description,
            "parameters": parameters
        }


if __name__ == "__main__":
    data = {
        "key1": 0,
        "key2": "text",
        "key3": None,
        "key4": [0,1,2,3,4,5]
    }

    resp = ServiceResponce("test", 1, "test_response", data['key4'])
    
    print(resp.to_json())