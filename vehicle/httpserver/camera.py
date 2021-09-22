import cv2
import time
import threading


class Camera(object):
    last_access = 0  # time of last client access to the camera
    
    def __init__(self, camPort, width=None, height=None):
            self.port = camPort
            self.FrameWidth = width
            self.FrameHeight = height
            self.actualFrame = None
            self._test_run()
    
    def __open(self, cap, nTries=3):
        if cap.isOpened():
            cap.release()
        
        for i in range(nTries):
            cap.open(self.port)
            if cap.isOpened():
                break
        else:
            msg = f"Can't open camera port {self.port}!"
            print(msg)
            raise RuntimeError(msg)
        
        if self.FrameWidth is not None:
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, self.FrameWidth)

        if self.FrameHeight is not None:
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, self.FrameHeight)

    def _test_run(self):
        cap = cv2.VideoCapture()
        self.__open(cap)
        frame = None
        for i in range(5):
            _, frame = cap.read()
        cap.release()
        self.actualFrame = frame

    def get_frame(self):
        frame = self.actualFrame.copy()
        cv2.putText(
            frame,
            f"time={Camera.last_access}",
			(100,20),
            cv2.FONT_HERSHEY_COMPLEX,
            0.5,
            (0,0,0),
            1
        )
        (flag, encodedImage) = cv2.imencode(".jpeg", frame)
        Camera.last_access = time.time()
        return encodedImage

    def start(self):
        self.__class__.thread = threading.Thread(target=self.__class__.thread)

    @classmethod
    def _thread(cls):
        # with cv2.VideoCapture() as cap:
            
        #     while True:
        #         frame = cap.read()
        #         (flag, encodedImage) = cv2.imencode(".jpg", frame)

        #         # the last 10 seconds stop the thread
        #         if time.time() - cls.last_access > 20:
        #             break
        cls.thread = None