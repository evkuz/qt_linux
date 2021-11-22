import json


def obj_to_dict(obj):
    return obj.__dict__


class BaseResponce:
    def __init__(self, rc:int, info:str=""):
        self.rc = rc
        self.info = info
    
    def __str__(self) -> str:
        return json.dumps(self.__dict__,  default = obj_to_dict)

    def to_json(self):
        return json.dumps(self.__dict__,  default = obj_to_dict)
