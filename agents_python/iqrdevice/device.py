from re import S
from iqrdevice.action import BaseAction, ActionResponce
from iqrdevice.status import StatusResponce
from iqrdevice.service import BaseService, ServiceResponce
from iqrdevice.history import History, HistResponce
from typing import Callable


class ListActionsService(BaseService):
    def __init__(self, device):
        BaseService.__init__(self, "getactions")
        self.__device = device
    
    def get_data(self, **kwargs):
        return self.__device.get_list_actions()

    def get_info(self) -> dict:
        return self.make_info(
            "Returns list of available actions"
        )


class ListServicesService(BaseService):
    def __init__(self, device):
        BaseService.__init__(self, "getservices")
        self.__device = device
    
    def get_data(self, **kwargs):
        return self.__device.get_list_services()

    def get_info(self) -> dict:
        return self.make_info(
            "Returns list of available services"
        )


class IQRDevice:
    def __init__(self, name):
        self.__name = name
        self.__state = "none"
        self.__services = [
            ListActionsService(self),
            ListServicesService(self)
        ]
        self.__actions = []
        self.__history = History()
        self.__lock_key = None
        self.set_state_init()
    
    def set_name(self, name):
        self.__name = name

    def set_state_fail(self):
        self.__state = "fail"
    
    def set_state_run(self):
        self.__state = "run"
    
    def set_state_init(self):
        self.__state = "init"

    def set_lock(self, key:str)->bool:
        if self.__lock_key is not None:
            if key != self.__lock_key:
                return False
        self.__lock_key = key
        return True

    def set_unlock(self, key:str)->bool:
        if self.__lock_key is not None:
            if key != self.__lock_key:
                return False
        self.__lock_key = None
        return True

    @property
    def is_locked(self)->bool:
        return self.__lock_key is not None

    def get_nodes_state(self)->dict:
        return {}

    def get_list_services(self):
        return [x.get_info() for x in self.__services]

    def get_list_actions(self):
        return [x.get_info() for x in self.__actions]


    def register_action(self, action:BaseAction):
        self.__actions.append(action)

    def register_service(self, service:BaseService):
        self.__services.append(service)


    def get_status(self, actions=[]) -> StatusResponce:
        action_list = []
        if len(actions) == 0:
            for a in self.__actions:
                if a.IsWorking:
                    action_list.append(a.State)
        else:
            for a in self.__actions:
                if a.Name in actions:
                    action_list.append(a.State)
            if len(action_list) < len(actions):
                return StatusResponce(
                    self.__name,
                    self.__state,
                    self.is_locked,
                    -2, "Not all specified actions are exist",
                    action_list
                )

        return StatusResponce(
                    self.__name,
                    self.__state,
                    self.is_locked,
                    0, "success",
                    action_list,
                    self.get_nodes_state()
                )

    def reset_action(self, actions=[], key=None) -> ServiceResponce:
        if self.__lock_key is not None:
            if key != self.__lock_key:
                return ServiceResponce("reset", -7,  "Device control was locked. Your lock key isn't valid.")

        reset_statuses = {}
        if len(actions) == 0:
            for a in self.__actions:
                if a.IsWorking:
                    reset_statuses[a.Name] = a.reset()
        else:
            for a in self.__actions:
                if a.Name in actions:
                    reset_statuses[a.Name] = a.reset()

            if len(reset_statuses.keys()) <  len(actions):
                return ServiceResponce(
                    "reset",
                    -1,
                    "Not all specified actions are exist",
                    reset_statuses
                )

        return ServiceResponce(
                    "reset",
                    0, "success",
                    reset_statuses
                )

    def get_hist(self, type:str, names:list=[], n:int=20) -> HistResponce:
        if type == "action":
            return self.__history.get_actions(names, n)
        elif type == "system":
            return self.__history.get_system(n)
        else:
            return HistResponce(-1, "wrong type")  

    def run_action(self, action:str, params:dict={}) -> ActionResponce:
        if self.__lock_key is not None:
            if 'key' not in params or params['key'] != self.__lock_key:
                 return ActionResponce(action, -7,  "Device control was locked. Your lock key isn't valid.")

        for a in self.__actions:
            if a.Name == action:
                rc = a.run(**params)
                if rc == 0:
                    return ActionResponce(action, 0, "action started")
                else:
                    return ActionResponce(action, -3, "action is already running")

        return ActionResponce(action, -1,  "action with this name wasn't found")                
                
    
    def get_service_info(self, service:str, params:dict={}) -> ServiceResponce:
        data = None
        srv = None
        for s in self.__services:
            if s.Name == service:
                srv = s
                break
        else:
            return ServiceResponce(service, -1, "service with this name wasn't found")
        
        if self.__lock_key is not None:
            if 'key' not in params or params['key'] != self.__lock_key:
                 if srv.Lockable:
                     return ServiceResponce(service, -7,  "Device control was locked. Your lock key isn't valid.")
        try:
            data = srv.get_data(**params)
        except Exception as e:
            return ServiceResponce(service, -2, str(e))

        if data is None:
            return ServiceResponce(
                service,
                -2, 
                "can't get responce from service"
            )
        else:
            return ServiceResponce(service, 0, "success", data)
