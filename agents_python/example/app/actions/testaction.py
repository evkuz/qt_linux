from time import sleep

from iqrdevice.action import BaseAction
from .. import device


class TestRobotAction (BaseAction):
    def __init__(self, default_duration=10):
        BaseAction.__init__(self, "testaction")
        self.default_duration = default_duration

    def get_info(self) -> dict:
        """Returns description of service if form of dict
        Returns:
            dict: "name": self.Name, ["parameter":"description", ...]
        """ 
        res = {
            "name":self.Name,
            "duration": "(float) - if it was specified the action will be long for this number of seconds"
        }
        return res

    def run_action(self, **kwargs) -> int:
        """You need to implement your action here
        This code will be performed in separated thread
        in the end you should return int value - result code

        [return result code]
        """
        duration = self.default_duration
        if 'duration' in kwargs:
             duration = float(kwargs['duration'])
        
        # The method from parent class, that change value of state's field 'info' 
        self._set_state_info(f"Action was performed with duration {duration}")
        
        # You can also use properties:
        # * self._workingFlag
        # * self._resetFlag

        sleep(duration)
        return 0

    def reset_action(self) -> int:
        """
        This code will be performed in separated thead
        you can stop something or change returned code

        [return result code -126 by default]
        """
        return -126

