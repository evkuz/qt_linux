from time import sleep
import pathlib
from iqrdevice.actions import ExecuteFileAction


script_path = pathlib.Path(__file__).parent.parent.resolve().joinpath("utils/move.sh")


class MoveToAAction (ExecuteFileAction):
    def __init__(self):
        ExecuteFileAction.__init__(
            self, "movetoa",
            "/bin/bash", 
            script_path.as_posix(),
            ['a']
         )


class MoveToBAction (ExecuteFileAction):
    def __init__(self):
        ExecuteFileAction.__init__(
            self, "movetob",
            "/bin/bash", 
            script_path.as_posix(),
            ['b']
         )