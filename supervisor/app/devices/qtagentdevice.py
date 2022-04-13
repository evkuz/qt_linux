from .basedevice import BaseDevice


class QTAgentDevice(BaseDevice):
    def __init__(self, addr:str, name:str, updateStateInterval:int=100):
        """_summary_

        Args:
            addr (str): device address
            name (str): device name
            updateStateInterval (int, optional): interval in milliseconds. Minimum value is 20. Defaults to 100.
        """
        BaseDevice.__init__(self, addr, name, updateStateInterval)
        
    def _update_state(self)->dict:
        return self.remote_device.manual_request("/run?cmd=status&")

    def _run_action(self, actionName:str, **kwargs)->dict:
        ans = self.remote_device.manual_request(f"/run?cmd={actionName}&")
        return {
            "name": actionName,
            "rc": 0,
            "info": ''
        }

    def _get_service_info(self, serviceName:str, **kwargs):
        return {
            "name": serviceName,
            "rc": -1,
            "info": "No service available for this agent",
            "data": {}
        }

    def _send_reset(self)->dict:
        ans = self.remote_device.manual_request("/run?cmd=start&")
        return {
            "name": "reset",
            "rc": 0,
            "info": 'success',
            "data": {}
        }
