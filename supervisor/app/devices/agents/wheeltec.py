import logging
from ..agentspythondevice import AgentsPythonDevice

class Wheeltec(AgentsPythonDevice):
    def __init__(self, addr:str, updateStateInterval:float):
        AgentsPythonDevice.__init__(self, addr, "wheeltec", updateStateInterval)
        self._pinkmanHasCube = False
        self._bluemanHasCube = False
        self.start()

    def _do_action(self, environment:dict)->None:
        references = ['pinkman', 'blueman']
        if not all(i in environment for i in references):
            logging.warning(f"not all agents was added to environment ({references})")
            return
        if not all(environment[i]['connected'] for i in references):
            logging.warning(f"not all agents is connected: ({references})")
            return
        
        #-- PINKMAN --
        pmTookCube = False
        pmHasCube = False
        for n in environment['pinkman']['nodes']:
            if n['name'] == 'hascube':
                pmHasCube = bool(n['has_cube'])
        
        if self._pinkmanHasCube != pmHasCube:
            self._pinkmanHasCube = pmHasCube
            if not pmHasCube:
                pmTookCube = True
        pmFree = len(environment['pinkman']['actions_list']) == 0
        
        #-- BLUEMAN --
        bmGiveCube = False
        bmHasCube = False
        for n in environment['blueman']['nodes']:
            if n['name'] == 'hascube':
                bmHasCube = bool(n['has_cube'])
        
        if self._bluemanHasCube != bmHasCube:
            self._bluemanHasCube = bmHasCube
            if not bmHasCube:
                bmGiveCube = True

        bmFree = len(environment['blueman']['actions_list']) == 0

        #-- WHEELTEC --
        wtInA = False
        wtInB = False
        wtHasCube = False
        for n in environment['wheeltec']['nodes']:
            if n['name'] == 'position':
                if n['point'] == 'A':
                    wtInA = True
                elif n['point'] == 'B':
                    wtInB = True
            elif n['name'] == 'hascube':
                wtHasCube = bool(n['has_cube'])
                

        if bmGiveCube and wtInA:
            self.get_info("sethascube", set=True)
        elif wtInA and wtHasCube and bmFree:
            self.run_action("movetob")
        elif wtInB and pmTookCube:
            self.get_info("sethascube", set=False)
        elif wtInB and not wtHasCube and pmFree:
            self.run_action("movetoa")

        return
