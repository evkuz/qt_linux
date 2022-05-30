from iqrdevice.services import BaseService
from iqrdevice.nodes import HasCubeNode
from typing import Optional


class SetHasCubeService(BaseService):
    def __init__(self, hascube_node:Optional[HasCubeNode]=None):
        BaseService.__init__(self, "sethascube")
        self.__node = hascube_node
    
    def get_data(self, **kwargs)->dict:
        if 'set' in kwargs:
            val = kwargs['set'].lower()
            if val == 'true' or val == '1':
                has_cube = True
            elif val == 'false' or val == '0':
                has_cube = False
            else:
                raise Exception("Invalid 'set' parameter!")
            if self.__node is not None:
                self.__node.has_cube = has_cube

            return {"has_cube": has_cube}

        raise Exception("You must specify 'set' parameter!")

    def get_info(self) -> dict:
        return self.make_info(
            "Sets has cube value to 'hascube' node",
            parameters={
                'set':'available values: true|false|1|0'
            }
        )
