from .basenode import BaseNode


class CubeNode(BaseNode):
    def __init__(self, has_cube:bool=False):
        BaseNode.__init__(self, "cube")
        self.has_cube = has_cube
