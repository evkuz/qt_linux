import cv2
import time
import threading


class Camera(object):
    last_access = 0  # time of last client access to the camera

    def __init__(self, camPort, width=None, height=None):
        self.port = camPort
        self.FrameWidth = width
        self.FrameHeight = height
        self.__actualFrame = None
        self.__thread = None
        self.__cap = cv2.VideoCapture()
        self.__isWorking = False
        self.__waitForFrame = threading.Event()
        self.__waitForFrame.clear()

    @property
    def isOpened(self):
        return self.__cap.isOpened()

    @property
    def isStarted(self):
        return self.__isWorking

    def __open_device(self, nTries=3):
        if self.isOpened:
            self.__cap.release()

        for i in range(nTries):
            self.__cap.open(self.port)
            if self.__cap.isOpened():
                break
        else:
            msg = f"Can't open camera port {self.port}!"
            print(msg)
            raise RuntimeError(msg)

        if self.FrameWidth is not None:
            self.__cap.set(cv2.CAP_PROP_FRAME_WIDTH, self.FrameWidth)

        if self.FrameHeight is not None:
            self.__cap.set(cv2.CAP_PROP_FRAME_HEIGHT, self.FrameHeight)

    def __close_device(self):
        self.__cap.release()

    def start(self):
        if self.__thread is not None:
            return

        self.__thread = threading.Thread(target=self.__thread_work)
        self.__thread.start()

    def stop(self):
        if self.__thread is None:
            return
        self.__isWorking = False
        self.__thread.join()

    def get_frame(self):
        if not self.isStarted:
            self.start()
        
        self.__waitForFrame.wait()
        self.__waitForFrame.clear()
        frame = self.__actualFrame.copy()
        # cv2.putText(
        #     frame,
        #     f"time={Camera.last_access}",
        #  			(100, 20),
        #     cv2.FONT_HERSHEY_COMPLEX,
        #     0.5,
        #     (0, 0, 0),
        #     1
        # )
        Camera.last_access = time.time()
        return frame

    def __thread_work(self):
        self.__isWorking = True
        self.__open_device()

        while self.__isWorking:
            _, frame = self.__cap.read()
            self.__actualFrame = frame.copy()
            self.__waitForFrame.set()
            time.sleep(0)
            #the last 5 seconds stop the thread
            if time.time() - self.last_access > 5:
                break

        self.__close_device()
        self.__thread = None
        self.__isWorking = False
