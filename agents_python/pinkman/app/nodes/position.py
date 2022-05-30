from iqrdevice.nodes import PositionNode
from typing import Any


class ManipPositionNode(PositionNode):
    def __init__(self):
        super().__init__("position")
        self.point="home"

    def process_event(self, channel: str, data: Any) -> None:
        if not isinstance(data, dict):
            return
        if data['state'] == "run":
            self.point = "unknown"
            return
        
        if data['state'] == "finished":
            if data['name'] == "movehome":
                self.point = "home"
            elif data['name'] == "catchcube":
                self.point = "hold_cube"
            elif data['name'] == "putcube":
                self.point = "after_put"
         
        if data['state'] == "reset":
            if data['name'] == "catchcube":
                self.point = "home"