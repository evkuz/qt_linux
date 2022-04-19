from .basenode import BaseNode


class GripNode(BaseNode):
    def __init__(self, opened:bool=False, closed:bool=False, holds_object:bool=False):
        BaseNode.__init__(self, "grip")
        self.opened = opened
        self.closed = closed
        self.holds_object = holds_object