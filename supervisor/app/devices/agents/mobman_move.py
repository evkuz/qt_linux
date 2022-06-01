import logging
from ..agentspythondevice import AgentsPythonDevice

class MobmanMove(AgentsPythonDevice):
    def __init__(self, addr:str, updateStateInterval:float):
        AgentsPythonDevice.__init__(self, addr, "mobman_move", updateStateInterval)
        self.start()

    def _do_action(self, environment:dict)->None:
        references = ['hiwonder_qt', 'blueman', 'mobman_camera', 'mobman_qt']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return

        return
