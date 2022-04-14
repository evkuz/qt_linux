import logging
from ..agentspythondevice import AgentsPythonDevice

class Pinkman(AgentsPythonDevice):
    def __init__(self, addr:str, updateStateInterval:int):
        AgentsPythonDevice.__init__(self, addr, "pinkman", updateStateInterval)
        self.start()

    def catch_cube(self):
        self.run_action("catchcube")
    
    def put_cube(self):
        self.run_action("putcube")

    def open_grip(self):
        self.run_action("opengrip")
    
    def close_grip(self):
        self.run_action("closegrip")

    def _do_action(self, environment:dict, cubes:dict)->dict:
        references = ['wheeltec', 'xrrobot']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return {}
        if len(cubes) == 0:
            logging.warning("No cubes to act with")
            return {}
        return {}
        


