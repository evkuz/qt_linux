from typing import Any
from .basenode import BaseNode


class PositionNode(BaseNode):
    def __init__(self, name: str):
        super().__init__(name)
        self.point = "unknown"

    def process_event(self, channel: str, data: Any) -> None:
        if type(data) is str:
            self.point = data
