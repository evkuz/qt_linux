class RobotStatus(object):
    """
    status:{
        rc:”return code”, // return code от запроса
        // service_return:”результат вызова сервиса ”,  //если ответ на команду run?service=
        action_list:[<массив action>],
        // state:<init|run|fail> //init – ноды запускаются, run – робот активен и в работе, fail – не функционален
    }
    """
    def __init__(self, rc, action_state_list):
        self.rc = rc
        self.actions_list = action_state_list
   
    def __str__(self) -> str:
        res  = "{\n"
        res += f"    \"rc\": \"{self.rc}\",\n"
        res += "    \"action_list\": \n        [\n"
        first = True
        for act in self.actions_list:
            if first:
                res+=f"{act}"
                first = False
            else:
                res+= f",\n{act}"
        res += "\n        ]\n}\n"

        return res