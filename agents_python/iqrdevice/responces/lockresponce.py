from .baseresponce import BaseResponce


class LockResponce(BaseResponce):
    def __init__(self, locked:bool, rc:int, info:str=""):
        """
        name: str - action name
        rc:   int - request result code
        info: str - text interpretation of return code
        """
        BaseResponce.__init__(self, rc, info)
        self.state = "locked" if locked else "unlocked"
