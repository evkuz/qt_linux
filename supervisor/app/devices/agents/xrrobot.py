import logging
from ..agentspythondevice import AgentsPythonDevice

class XRRobot(AgentsPythonDevice):
    def __init__(self, addr:str, updateStateInterval:float):
        AgentsPythonDevice.__init__(self, addr, "xrrobot", updateStateInterval)
        self._pinkmanHasCube = False
        self._hiwonderHasCube = False
        self.start()

    def _do_action(self, environment:dict)->None:
        references = ['pinkman', 'hiwonder']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return
        if not all(environment[i]['connected'] for i in references):
            logging.warning(f"not all agents is connected: ({references})")
            return
        
        #-- PINKMAN --
        pmGiveCube = False
        pmHasCube = False
        for n in environment['pinkman']['nodes']:
            if n['name'] == 'hascube':
                pmHasCube = bool(n['has_cube'])
        
        if self._pinkmanHasCube != pmHasCube:
            self._pinkmanHasCube = pmHasCube
            if not pmHasCube:
                pmGiveCube = True
        pmFree = len(environment['pinkman']['actions_list']) == 0
        
        #-- XRROBOT --
        xrInA = False
        xrInB = False
        xrHasCube = False
        for n in environment['xrrobot']['nodes']:
            if n['name'] == 'position':
                if n['point'] == 'A':
                    xrInA = True
                elif n['point'] == 'B':
                    xrInB = True
            elif n['name'] == 'hascube':
                xrHasCube = bool(n['has_cube'])
        #xrFree = len(environment['xrrobot']['actions_list']) == 0

        #-- HIWONDER --
        hwTookCube = False
        hwHasCube = False
        for n in environment['hiwonder']['nodes']:
            if n['name'] == 'hascube':
                hwHasCube = bool(n['has_cube'])
        
        if self._hiwonderHasCube != hwHasCube:
            self._hiwonderHasCube = hwHasCube
            if not hwHasCube:
                hwTookCube = True


        if pmGiveCube and xrInA:
            self.get_info("sethascube", set=True)
        elif xrInA and xrHasCube and pmFree:
            self.run_action("movetob")
        elif xrInB and hwTookCube:
            self.get_info("sethascube", set=False)
        elif xrInB and not xrHasCube:
            self.run_action("movetoa")
