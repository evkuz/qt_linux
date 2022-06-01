from iqrdevice.nodes import PositionNode
from typing import Any


class VehiclePositionNode(PositionNode):
    def __init__(self):
        super().__init__("position")
        self.point="somewere"

    def process_event(self, channel: str, data: Any) -> None:
        if not isinstance(data, dict):
            return
        if channel == "actions":
            if data['state'] == "run" or data['state'] == "reset":
                self.point = "somewere"
                return
            
            if data['state'] == "finished":
                if data['name'] == "movetoa":
                    self.point = "A"
                elif data['name'] == "movetob":
                    self.point = "B"
        if channel == "services":
            if data['name'] == "startrosrun":
                if data['data']['status'] == 'OK':
                    self.point = "A"