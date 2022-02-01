from time import sleep

from iqrdevice.action import BaseAction
from .. import device


class MoveToAAction (BaseAction):
    def __init__(self, default_duration=15):
        BaseAction.__init__(self, "movetoa")
        self.default_duration = default_duration

    def get_info(self) -> dict:
        return {"name":self.Name}

    def run_action(self, **kwargs) -> int:
        # The method from parent class, that change value of state's field 'info' 
        self._set_state_info(f"Action was performed with duration {self.default_duration}")
        
        sleep(self.default_duration)
        return 0

    def reset_action(self) -> int:
        return -126

class MoveToBAction (BaseAction):
    def __init__(self, default_duration=20):
        BaseAction.__init__(self, "movetob")
        self.default_duration = default_duration

    def get_info(self) -> dict:
        return {"name":self.Name}

    def run_action(self, **kwargs) -> int:
        # The method from parent class, that change value of state's field 'info' 
        self._set_state_info(f"Action was performed with duration {self.default_duration}")
        
        sleep(self.default_duration)
        return 0

    def reset_action(self) -> int:
        return -126