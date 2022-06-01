import json


def obj_to_dict(obj):
    return obj.__dict__


class BaseStatus:
    def __init__(self, name):
        self.name = name
        self.state = "No information"
        self.connected = False
        self.actions_list = []
        self.nodes = {}
   
    def __str__(self) -> str:
        return json.dumps(self.__dict__,  default = obj_to_dict)

    def to_json(self):
        return json.dumps(self.__dict__,  default = obj_to_dict)
    
    def set_state(self, state:str):
        self.state = state

    def set_connected(self):
        self.connected = True

    def set_disconnected(self, errorText=None):
        if errorText is not None:
            self.state = "Disconnected: " + errorText
        else:
            self.state = "Disconnected"
    
    def set_actions_list(self, actionsList):
        self.actions_list = actionsList

    def __copy__(self):
        res = type(self)(self.name)
        res.state = self.state
        res.connected = self.connected
        res.actions_list = self.actions_list
        res.nodes = dict(self.nodes)
        return res

