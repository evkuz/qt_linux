from iqrdevice.nodes import PositionNode
from typing import Any


class HwPositionNode(PositionNode):
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
            if data['name'] in ["catchcube", "putcube", "movehome"]:
                self.point = "home"
            else:
                self.point = "not predefined"
         
        if data['state'] == "reset":
            self.point = "home"