from iqrdevice.action import BaseAction, ActionResponce
from iqrdevice.status import StatusResponce
from iqrdevice.service import BaseService, ServiceResponce
from iqrdevice.history import History, HistResponce


class ListActionsService(BaseService):
    def __init__(self, device):
        BaseService.__init__(self, "getactions")
        self.__device = device
    
    def get_data(self, **kwargs):
        return self.__device.get_list_actions()

    def get_info(self) -> dict:
        return {"name": self.Name}


class ListServicesService(BaseService):
    def __init__(self, device):
        BaseService.__init__(self, "getservices")
        self.__device = device
    
    def get_data(self, **kwargs):
        return self.__device.get_list_services()

    def get_info(self) -> dict:
        return {"name": self.Name}


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
        self.set_state_init()
    
    def set_name(self, name):
        self.__name = name

    def set_state_fail(self):
        self.__state = "fail"
    
    def set_state_run(self):
        self.__state = "run"
    
    def set_state_init(self):
        self.__state = "init"


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
                    -2, "Not all specified actions are exist",
                    action_list
                )

        return StatusResponce(
                    self.__name,
                    self.__state,
                    0, "success",
                    action_list
                )

    def reset_action(self, actions=[]) -> ServiceResponce:
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
        for a in self.__actions:
            if a.Name == action:
                rc = a.run(**params)
                if rc == 0:
                    return ActionResponce(action, 0, "action started")
                else:
                    return ActionResponce(action, -3, "action is already running")

        return ActionResponce(action, -1,  "action with this name wasn't found")                
                
    
    def get_service_info(self, service:str, params:dict={}) -> ServiceResponce:
        for srv in self.__services:
            if srv.Name == service:
                data = srv.get_data(**params)
                if data is None:
                    return ServiceResponce(
                        service,
                        -2, 
                        "can't get responce from service"
                    )
                else:
                    return ServiceResponce(service,0, "success", data)

        return ServiceResponce(service, -1, "service with this name wasn't found")          


