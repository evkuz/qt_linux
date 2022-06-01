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
        if not all(environment[i]['connected'] for i in references):
            logging.warning(f"not all agents is connected: ({references})")
            return
        
        wtOnPlace = False
        wtHasCube = False
        for n in environment['wheeltec']['nodes']:
            if n['name'] == 'position':
                if n['point'] == 'B':
                    wtOnPlace = True
            elif n['name'] == 'hascube':
                wtHasCube = bool(n['has_cube'])
            # elif n['name'] == 'rosrun':
            #     wtRos = bool(n['run'])
        
        pmHasCube = False
        pmFree = len(environment['pinkman']['actions_list']) == 0
        pmPosition = ""
        for n in environment['pinkman']['nodes']:
            if n['name'] == 'position':
                pmPosition = n['point']
            elif n['name'] == 'hascube':
                pmHasCube = bool(n['has_cube'])
        
        xrOnPlace = False
        xrHasCube = False
        for n in environment['xrrobot']['nodes']:
            if n['name'] == 'position':
                if n['point'] == 'A':
                    xrOnPlace = True
            elif n['name'] == 'hascube':
                xrHasCube = bool(n['has_cube'])
        xrFree = len(environment['xrrobot']['actions_list']) == 0
        
        if wtOnPlace and wtHasCube and pmFree and pmPosition == 'home':
            self.run_action("catchcube")
        elif pmPosition == "hold_cube" and pmFree and pmHasCube and xrOnPlace and not xrHasCube:
            self.run_action("putcube")
        elif pmFree and pmPosition == "after_put" and not xrFree:
            self.run_action("movehome")

        return
