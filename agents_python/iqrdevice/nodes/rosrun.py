from .basenode import BaseNode
from typing import Any


class RosRunNode(BaseNode):
    def __init__(self, run:bool=False):
        BaseNode.__init__(self, "rosrun")
        self.run = run

    def process_event(self, channel: str, data: Any)->None:
        #in case it is listening for mainbus
        if channel == "services":
            if data['name'] == "startrosrun":
                self.run = True
            elif data['name'] == "stoprosrun":
                self.run = False
        elif isinstance(data, bool):
            self.run = data
