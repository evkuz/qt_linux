from iqrdevice.nodes import BaseNode
from typing import Any


class HwConnectedNode(BaseNode):
    def __init__(self, connected:bool=False):
        BaseNode.__init__(self, "hascube")
        self.connected = connected

    def process_event(self, channel: str, data: Any) -> None:
        if not channel.endswith("/connected"):
            return
        if not isinstance(data, bool):
            return
        
        self.connected = data