import json


def obj_to_dict(obj):
    return obj.__dict__


class BaseNode(object):
    def __init__(self, name:str):
        self.name = name

    def __str__(self) -> str:
        return json.dumps(self.__dict__,  default = obj_to_dict)

    def to_json(self)->str:
        return json.dumps(self.__dict__,  default = obj_to_dict)
