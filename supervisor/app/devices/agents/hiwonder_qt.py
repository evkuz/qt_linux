import logging
from ..qtagentdevice import QTAgentDevice

class HiwonderQt(QTAgentDevice):
    def __init__(self, addr:str, updateStateInterval:float):
        QTAgentDevice.__init__(self, addr, "hiwonder_qt", updateStateInterval)
        self.start()

    def _do_action(self, environment:dict)->None:
        references = ['mobman_move', 'mobman_camera', 'hiwonder']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return

        return
        
#clamp
#start
