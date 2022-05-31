from iqrdevice.services import BaseService
from ..utils import SocketServer
import time

class RestartSocketServerService(BaseService):
    def __init__(self, socket_server:SocketServer):
        BaseService.__init__(self, "restartsocket")
        self.__socket_server = socket_server
    
    def get_info(self) -> dict:
        return self.make_info(
            "Restarts socket server"
        )

    def get_data(self, **kwargs):
        self.__socket_server.stop()
        self.__socket_server.start()
        
        time.sleep(0.2)
        if self.__socket_server.is_started:
            return "OK"
        return r"FAIL"
