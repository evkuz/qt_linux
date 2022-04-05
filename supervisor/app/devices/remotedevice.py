import requests
import json
from time import sleep


class RemoteDevice:
    def __init__(self, addr):
        self.addr = addr
        self.defaultTimeout = 0.1

    def set_timeout(self, value:float):
        self.defaultTimeout = value

    def get_status(self, actionNames=[]):
        params = dict()
        if len(actionNames) != 0:
            params['action'] = ",".join(actionNames)
        url = self.addr + "/status"
        return self.__send_get_request(url, params, self.defaultTimeout)

    def run_action(self, name, **kwargs):
        params = kwargs
        params["name"] = name
        url = self.addr + "/action"
        return self.__send_get_request(url, params, self.defaultTimeout)

    def get_service_info(self, name, timeout=None, **kwargs):
        if timeout is None:
            timeout = self.defaultTimeout
        params = kwargs
        params["name"] = name
        url = self.addr + "/service"
        return self.__send_get_request(url, params, timeout)

    def reset_actions(self, actionNames:list):
        params = dict()
        if len(actionNames) != 0:
            params['action'] = ",".join(actionNames)
        url = self.addr + "/reset"
        return self.__send_get_request(url, params, self.defaultTimeout)

    def manual_request(self, addr_addition:str):
        if len(addr_addition) > 0 and addr_addition[0] != '/':
            addr_addition = '/' + addr_addition
        url = self.addr + addr_addition
        return self.__send_get_request(url, None, self.defaultTimeout)
    
    def wait_for_action_finished(self, actionName=None):
        while True:
            st = self.get_status()
            if type(st) is not dict:
                raise RuntimeError("Unexpected result of 'get_status'")

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

    def __send_get_request(self, url, params, timeout):
        try:
            resp = requests.get(url=url, params=params, timeout=timeout)
        except Exception as e:
            raise e
        try:
            data = resp.json()
        except Exception as e:
            return resp.text
        return data


if __name__ == '__main__':
    dev = RemoteDevice("http://192.168.1.201:5001")
    d = dev.get_status()
    print(d)
    
    d = dev.get_service_info("getservices")
    print(d)
    
    d = dev.get_service_info("getposition")
    print(d)
    
    #print(d['data']['distance'])
