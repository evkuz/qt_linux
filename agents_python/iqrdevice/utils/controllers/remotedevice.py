import requests
from time import sleep
from typing import Optional, List
from .basecontroller import BaseController


class RemoteDevice(BaseController):
    def __init__(self, addr:str, name:str="remote"):
        BaseController.__init__(self, name)
        self.addr = addr
        self.defaultTimeout = 0.1

    def set_timeout(self, value:float):
        self.defaultTimeout = value

    @property
    def channels(self) -> List[str]:
        return ['status']

    def get_status(self, actionNames:List[str]=[])->dict:
        params = dict()
        if len(actionNames) != 0:
            params['action'] = ",".join(actionNames)
        url = self.addr + "/status"
        res = self.__send_get_request(url, params, self.defaultTimeout)
        self.fire_event("status", res)
        return res

    def run_action(self, name:str, **kwargs)->dict:
        params = kwargs
        params["name"] = name
        url = self.addr + "/action"
        return self.__send_get_request(url, params, self.defaultTimeout)

    def get_service_info(self, name:str, **kwargs)->dict:
        params = kwargs
        params["name"] = name
        url = self.addr + "/service"
        return self.__send_get_request(url, params, self.defaultTimeout)

    def reset_actions(self, actionNames:List[str])->dict:
        params = dict()
        if len(actionNames) != 0:
            params['action'] = ",".join(actionNames)
        url = self.addr + "/reset"
        return self.__send_get_request(url, params, self.defaultTimeout)

    def manual_request(self, addr_addition:str)->dict:
        if len(addr_addition) > 0 and addr_addition[0] != '/':
            addr_addition = '/' + addr_addition
        url = self.addr + addr_addition
        return self.__send_get_request(url, None, self.defaultTimeout)
    
    def wait_for_action_finished(self, actionName:Optional[str]=None)->None:
        while True:
            st = self.get_status()
            if st['rc'] != 0:
                print(st)
                break
            if actionName is not None:
                for a in st['action_list']:
                    if a['name'] == actionName:
                        break
                else:
                    break
            else:
                if len(st['action_list']) == 0:
                    break
            sleep(0.1)

    def __send_get_request(self, url:str, params:Optional[dict], timeout:float)->dict:
        try:
            resp = requests.get(url=url, params=params, timeout=30)
        except Exception as e:
            raise e
        try:
            data = resp.json()
        except Exception as e:
            return {"value": resp.text}
        return data


if __name__ == '__main__':
    dev = RemoteDevice("http://192.168.1.201:5001")
    d = dev.get_status()
    print(d)
    
    d = dev.get_service_info("getservices")
    print(d)
    
    d = dev.get_service_info("getposition")
    print(d)
    
    print(d['data']['distance'])