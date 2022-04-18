import logging
from ..agentspythondevice import AgentsPythonDevice

class XRRobot(AgentsPythonDevice):
    def __init__(self, addr:str, updateStateInterval:float):
        AgentsPythonDevice.__init__(self, addr, "xrrobot", updateStateInterval)
        self.start()

    def _do_action(self, environment:dict, cubes:dict)->dict:
        references = ['pinkman', 'hiwonder_qt']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return {}
        if len(cubes) == 0:
            return {}
        return {}
        


