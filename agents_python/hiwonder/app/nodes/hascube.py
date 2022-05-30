from iqrdevice.nodes import HasCubeNode
from typing import Any


class HwHasCubeNode(HasCubeNode):
    def __init__(self):
        super().__init__(False)

    def process_event(self, channel: str, data: Any) -> None:
        if not isinstance(data, dict):
            return
        
        if data['name'] == "catchcube":
            if data['state'] == "run":
                self.has_cube = False
            elif data['state'] == "finished":
                self.has_cube = True
        elif data['name'] == "putcube":
            if data['state'] == "finished":
                self.has_cube = False
        else:
            self.has_cube = False
