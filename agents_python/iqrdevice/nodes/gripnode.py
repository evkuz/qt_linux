import sys
import os

if __package__:
    from .basenode import BaseNode
else:
    sys.path.append(os.path.dirname(__file__) + '/..')
    from basenode import BaseNode

from typing import Tuple, List

class GripNode(BaseNode):
    def __init__(self, opened:bool=False, closed:bool=False, holds_object:bool=False):
        BaseNode.__init__(self, "grip")
        self.opened = opened
        self.closed = closed
        self.holds_object = holds_object


class ManipGripNode(GripNode):
    def __init__(self, grip_closed:int, grip_opened:int, min_dist:float):
        GripNode.__init__(self)
        self.__closed_val = grip_closed
        self.__opened_val = grip_opened
        self.__min_dist = min_dist

    def set_from_manip_pos(self, pos:Tuple[float, List[int]]) -> None:
        if abs(pos[1][-1] - self.__closed_val) < 5:
            self.closed = True
        else:
            self.closed = False

        if abs(pos[1][-1] - self.__opened_val) < 5:
            self.opened = True
        else:
            self.opened = False

        if pos[0] < self.__min_dist and self.closed:
            self.holds_object = True
        else:
            self.holds_object = False


if __name__ == "__main__":
    gr = GripNode()

    print(str(gr))