from .basedevice import BaseDevice


class AgentsPythonDevice(BaseDevice):
    def __init__(self, addr:str, name:str, updateStateInterval:int=100):
        """_summary_

        Args:
            addr (str): device address
            name (str): device name
            updateStateInterval (int, optional): interval in milliseconds. Minimum value is 20. Defaults to 100.
        """
        BaseDevice.__init__(self, addr, name, updateStateInterval)
        
    def _update_state(self)->dict:
        return self.remote_device.get_status()

    def _run_action(self, actionName:str, **kwargs)->dict:
        return self.remote_device.run_action(name=actionName, **kwargs)

    def _get_service_info(self, serviceName:str, **kwargs):
        return self.remote_device.get_service_info(
                    name=serviceName,
                    timeout=1.,
                    **kwargs
                )

    def _send_reset(self)->dict:
        return self.remote_device.reset_actions([])


if __name__ == '__main__':
    dev = AgentsPythonDevice(
        "http://192.168.1.80:5001/",
        "wheeltec",
        100
    )
    dev.start()
    while True:
        print(dev.State)
        line = input()
        
        if len(line) < 2:
            break

        #print(dev.send_command(line))
    
    dev.stop()