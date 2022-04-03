
# -*- coding: utf-8 -*-
import socket
import os
import logging
import threading
from iqrdevice.utils.vision import BaseDetector, BaseCamera


ACT_NONE = 0
ACT_SEND_COORDS = 1


class SocketServer(object):
    def __init__(self, socket_path:str, cam:BaseCamera, detector:BaseDetector):
        self.__socket_path = socket_path
        self.__camera = cam
        self.__detector = detector
        self.__thread = None
        self.__isWorking = False
        self.__locker = threading.Lock()
        self.__state = (0,0,0)

    def __del__(self):
        self.stop()

    @property
    def is_started(self):
        return self.__isWorking

    def start(self):
        """Starts separated thread for socket server
        """
        if self.__thread is not None:
            return
        self.__isWorking = True
        self.__thread = threading.Thread(target=self.__thread_work)
        self.__thread.start()

    def stop(self):
        """Stops separated thread
        """
        if self.__thread is None:
            return
        self.__isWorking = False
        self.__thread.join()
        try:
            os.remove(self.__socket_path)
        except Exception as e:
            logging.error(str(e))
            

    def __thread_work(self):
        try:
            logging.info("Socket server thread was started")
            with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as ssocket:
                self.__ssocket = ssocket
                if os.path.exists(self.__socket_path):
                    os.remove(self.__socket_path)
                logging.info("Binding socket...")
                ssocket.settimeout(0.1)
                ssocket.bind(self.__socket_path)
                ssocket.listen()
                while self.__isWorking:
                    try:
                        conn, _addr = ssocket.accept()
                    except Exception:
                        continue
                    
                    data = conn.recv(4)
                    if len(data) > 0:
                        answer = ""
                        command = int(data.decode("utf-8"))
                        logging.info(f"got command: {command}")
                        if command == ACT_SEND_COORDS:
                            answer = self.get_detector_state()

                        conn.send(bytes(answer, encoding="utf-8"))
                    conn.close()
        except Exception as e:
            logging.error(str(e))
        finally:
            self.__ssocket = None
            self.__thread = None
            self.__isWorking = False
            logging.info("Socket server thread was stopped")

    def get_detector_state(self)->str:
        self.__camera.wait_for_new_frame()
        frame = self.__camera.get_last_frame()
        det_res = self.__detector.detect(frame, False)
        d = 1 if det_res['detected'] else 0
        x = 0 if not det_res['detected'] else det_res['x']
        y = 0 if not det_res['detected'] else det_res['y']

        return f"{d} {x} {y}"

    # def set_state(self, detected:bool, x:float=0.5, y:float=0.5):
    #     with self.__locker:
    #         self.__state = (
    #             1 if detected else 0,
    #             x,
    #             y
    #         )


if __name__ == '__main__':
    pass
    # import random
    
    # root = logging.getLogger()
    # root.setLevel(logging.INFO)
    # server = SocketServer("/run/lock/my.socket")
    # server.start()

    # detected = False

    # while True:
    #     l = input()
    #     if l.startswith("q\n"):
    #         break
    #     detected = not detected
    #     server.set_state(
    #         detected,
    #         random.uniform(0, 1),
    #         random.uniform(0, 1)
    #     )

