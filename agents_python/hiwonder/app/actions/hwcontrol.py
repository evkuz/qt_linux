import logging
from time import sleep
from iqrdevice.actions import BaseAction
from ..utils import HiwonderQt
from iqrdevice.utils.vision import BaseCamera, BaseDetector


class CatchCubeAction (BaseAction):
    def __init__(self, qt_device:HiwonderQt, cam:BaseCamera, detector:BaseDetector):
        BaseAction.__init__(self, "catchcube")
        self.qt_device = qt_device
        self.__cam = cam
        self.__detector = detector
        
    def get_info(self) -> dict:
        return self.make_info(
            "Will try to catch cube"
        )

    def run_action(self, **kwargs) -> int:
        self.qt_device.reset_actions()
        self.__cam.wait_for_new_frame()
        frame = self.__cam.get_last_frame()
        det_res = self.__detector.detect(frame)
        if not det_res['detected']:
            self._set_state_info(f"Cube wasn't detected!")
            return -1

        if not self.qt_device.IsConnected:
            self._set_state_info(f"Device isn't connected!")
            return -2
        
        if not self._workingFlag:
            return -126

        self.qt_device.run_action("start")
        while self._workingFlag:
            st = self.qt_device.get_state()
            if not isinstance(st, dict):
                self._set_state_info(f"Device answer isn't dict type!")
                return -3
            if not 'action_list' in st.keys():
                self._set_state_info(f"Device answer hasn't action_list!")
                logging.error(str(st))
                sleep(0.2)
                continue
            if len(st['action_list']) == 0:
                break
            sleep(0.5)

        self.__cam.wait_for_new_frame()
        frame = self.__cam.get_last_frame()
        det_res = self.__detector.detect(frame)
        if det_res['detected']:
            self._set_state_info(f"Cube wasn't taken!")
            return -5
        
        self._set_state_info(f"Cube was catched.")
        return 0

    def reset_action(self) -> int:
        self.qt_device.reset_actions()
        self.qt_device.run_action("standup")
        while True:
            st = self.qt_device.get_state()
            if not isinstance(st, dict):
                self._set_state_info(f"Device answer isn't dict type!")
                return -3
            if not 'action_list' in st.keys():
                self._set_state_info(f"Device answer hasn't action_list!")
                return -4
            if len(st['action_list']) == 0:
                break
            sleep(0.5)

        self._set_state_info(f"Stopped and returned home")
        return -126


class PutCubeAction (BaseAction):
    def __init__(self, qt_device:HiwonderQt):
        BaseAction.__init__(self, "putcube")
        self.qt_device = qt_device
        
    def get_info(self) -> dict:
        return self.make_info(
            "Will put cube to predefined position"
        )

    def run_action(self, **kwargs) -> int:
        if not self.qt_device.IsConnected:
            self._set_state_info(f"Device isn't connected!")
            return -2
        
        self.qt_device.run_action("put_box")
        while self._workingFlag:
            st = self.qt_device.get_state()
            if not isinstance(st, dict):
                self._set_state_info(f"Device answer isn't dict type!")
                return -3
            if not 'action_list' in st.keys():
                self._set_state_info(f"Device answer hasn't action_list!")
                sleep(0.2)
                continue
            if len(st['action_list']) == 0:
                break
            sleep(0.5)

        self._set_state_info(f"Cube was put.")
        return 0

    def reset_action(self) -> int:
        self.qt_device.reset_actions()
        self.qt_device.run_action("standup")
        while True:
            st = self.qt_device.get_state()
            if not isinstance(st, dict):
                self._set_state_info(f"Device answer isn't dict type!")
                return -3
            if not 'action_list' in st.keys():
                self._set_state_info(f"Device answer hasn't action_list!")
                return -4
            if len(st['action_list']) == 0:
                break
            sleep(0.5)
        self._set_state_info(f"Stopped and returned home")
        return -126


class MoveHomeAction (BaseAction):
    def __init__(self, qt_device:HiwonderQt):
        BaseAction.__init__(self, "movehome")
        self.qt_device = qt_device
        
    def get_info(self) -> dict:
        return self.make_info(
            "Will move robot to home position"
        )

    def run_action(self, **kwargs) -> int:
        if not self.qt_device.IsConnected:
            self._set_state_info(f"Device isn't connected!")
            return -2
        
        self.qt_device.run_action("standup")
        while self._workingFlag:
            st = self.qt_device.get_state()
            if not isinstance(st, dict):
                self._set_state_info(f"Device answer isn't dict type!")
                return -3
            if not 'action_list' in st.keys():
                self._set_state_info(f"Device answer hasn't action_list!")
                sleep(0.2)
                continue
            if len(st['action_list']) == 0:
                break
            sleep(0.5)

        self._set_state_info(f"Cube was put.")
        return 0

    def reset_action(self) -> int:
        self.qt_device.reset_actions()
        self._set_state_info(f"Stopped")
        return -126
