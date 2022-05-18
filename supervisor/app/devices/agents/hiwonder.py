import logging
from ..agentspythondevice import AgentsPythonDevice

class Hiwonder(AgentsPythonDevice):
    def __init__(self, addr:str, updateStateInterval:float):
        AgentsPythonDevice.__init__(self, addr, "hiwonder", updateStateInterval)
        self.start()

    def _do_action(self, environment:dict)->None:
        references = ['xrrobot'] #['mobman_move', 'mobman_camera', 'xrrobot']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return
        if not all(environment[i]['connected'] for i in references):
            logging.warning(f"not all agents is connected: ({references})")
            return
        
        #-- XRROBOT --
        xrInB = False
        xrHasCube = False
        for n in environment['xrrobot']['nodes']:
            if n['name'] == 'position':
                if n['point'] == 'B':
                    xrInB = True
            elif n['name'] == 'hascube':
                xrHasCube = bool(n['has_cube'])
        
        #-- HIWONDER --
        hwHasCube = False
        hwAtHome = False
        hwAtGiveCube = False
        for n in environment['hiwonder']['nodes']:
            if n['name'] == 'hascube':
                hwHasCube = bool(n['has_cube'])
            elif n['name'] == 'position':
                if n['point'] == 'home':
                    hwAtHome = True
                elif n['point'] == 'givecube':
                    hwAtGiveCube = True
        
        hwFree = len(environment['hiwonder']['actions_list']) == 0

       
        if xrInB and xrHasCube and hwAtHome and not hwHasCube and hwFree:
            self.run_action("catchcube")
        elif hwHasCube and hwAtHome and hwFree:
            self.run_action("putcube")

        return
