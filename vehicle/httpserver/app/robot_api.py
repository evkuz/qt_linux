from .status import RobotStatus
from .robot_actions import TransferCubeAction, MoveBackAction


class RobotApi(object):
    def __init__(self):
        self.__actions_list = [
            TransferCubeAction(),
            #MoveBackAction()
        ]

    def __make_status(self, rc):
        act_states = []
        for act in self.__actions_list:
            act_states.append(act.State)
        return RobotStatus(rc, act_states)

    def reset(self):
        for act in self.__actions_list:
            if act.IsWorking:
                act.reset()
        return self.__make_status(0)
    
    def run_action(self, action_name):
        rc = -1
        for act in self.__actions_list:
            if act.Name == action_name:
                rc = act.run()
                break
        return self.__make_status(rc)

    @property
    def status(self):
        return self.__make_status(0)


if __name__ == '__main__':
    print(RobotApi().status)
