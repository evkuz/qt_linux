import logging
from ..agentspythondevice import AgentsPythonDevice

class Pinkman(AgentsPythonDevice):
    def __init__(self, addr:str, updateStateInterval:float):
        AgentsPythonDevice.__init__(self, addr, "pinkman", updateStateInterval)
        self.start()

    def _do_action(self, environment:dict)->None:
        references = ['wheeltec', 'xrrobot']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return
        
        return
