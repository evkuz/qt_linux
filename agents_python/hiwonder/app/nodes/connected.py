from iqrdevice.nodes import BaseNode
from typing import Any


class HwConnectedNode(BaseNode):
    def __init__(self, connected:bool=False, started:bool=False):
        BaseNode.__init__(self, "rdconnection")
        self.connected = connected
        self.started = started

    def process_event(self, channel: str, data: Any) -> None:
        if not isinstance(data, bool):
            return
        if channel.endswith("/connected"):
            self.connected = data
        elif  channel.endswith("/start"):
            self.started = data