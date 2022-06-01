import logging
from ..agentspythondevice import AgentsPythonDevice

class MobmanCamera(AgentsPythonDevice):
    def __init__(self, addr:str, updateStateInterval:float):
        AgentsPythonDevice.__init__(self, addr, "mobman_camera", updateStateInterval)
        self.start()

    def _do_action(self, environment:dict)->None:
        references = ['mobman_move', 'hiwonder_qt']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return

        return
        


