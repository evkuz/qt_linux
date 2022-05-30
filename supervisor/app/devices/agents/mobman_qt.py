import logging
from ..qtagentdevice import QTAgentDevice

class MobmanQt(QTAgentDevice):
    def __init__(self, addr:str, updateStateInterval:float):
        QTAgentDevice.__init__(self, addr, "mobman_qt", updateStateInterval)
        self.start()

    def _do_action(self, environment:dict)->None:
        references = ['mobman_camera', 'blueman', 'mobman_move']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return
        
        return
