from time import sleep
from iqrdevice.actions import ExecuteFileAction

script_path = "move.sh"

class MoveToAAction (ExecuteFileAction):
    def __init__(self):
        ExecuteFileAction.__init__(
            self, "movetoa",
            "/bin/bash", 
            [script_path, 'a']
         )


class MoveToBAction (ExecuteFileAction):
    def __init__(self):
        ExecuteFileAction.__init__(
            self, "movetob",
            "/bin/bash", 
            [script_path, 'b']
         )