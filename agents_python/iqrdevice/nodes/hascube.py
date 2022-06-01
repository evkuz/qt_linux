from .basenode import BaseNode
from typing import Any


class HasCubeNode(BaseNode):
    def __init__(self, has_cube:bool=False):
        BaseNode.__init__(self, "hascube")
        self.has_cube = has_cube

    def process_event(self, channel: str, data: Any)->None:
        #in case it is listening for mainbus
        if channel == "services":
            if data['name'] == "hascube":
                self.has_cube = bool(data['name']['data']['has_cube'])
