import json
from typing import Any


def obj_to_dict(obj):
    return obj.__dict__


class BaseNode(object):
    def __init__(self, name:str):
        self.name = name

    def process_event(self, channel:str, data:Any)->None:
        raise NotImplementedError()

    def __str__(self) -> str:
        return json.dumps(self.__dict__,  default = obj_to_dict)

    def to_json(self)->str:
        return json.dumps(self.__dict__,  default = obj_to_dict)

    def to_dict(self)->dict:
        d = self.__dict__
        return {key: d[key] for key in d if not key.startswith('_')}
        