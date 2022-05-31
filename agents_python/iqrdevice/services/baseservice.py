from iqrdevice.responces import ServiceResponce


class BaseService:
    def __init__(self, name:str, lockable:bool=True):
        self.__name = name
        self.__lockable = lockable
        
    @property
    def Name(self) -> str:
        return self.__name

    @property
    def Lockable(self) -> bool:
        return self.__lockable

    def get_data(self, **kwargs):
        raise NotImplementedError()

    def get_info(self) -> dict:
        """Must be redefined in child classes,
        Returns:
            dict: "name": self.Name, ["parameter":"description", ...]
        """
        raise NotImplementedError()

    def make_info(self, description:str, parameters:dict={}):
        return {
            "name":self.Name,
            "description": description,
            "parameters": parameters
        }


if __name__ == "__main__":
    data = {
        "key1": 0,
        "key2": "text",
        "key3": None,
        "key4": [0,1,2,3,4,5]
    }

    resp = ServiceResponce("test", 1, "test_response", data['key4'])
    
    print(resp.to_json())