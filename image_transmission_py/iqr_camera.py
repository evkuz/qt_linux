import cv2

class IQRCamera:
    def __init__(self, camPort, width=None, height=None):
        self.port = camPort
        self.FrameWidth = width
        self.FrameHeight = height
        self.cap = None
    

    @property
    def is_opened(self):
        if self.cap is None:
            return False
        return self.cap.isOpened()


    def open(self, nTries=3):
        self.close()
        self.cap = cv2.VideoCapture()
        
        for i in range(nTries):
            self.cap.open(self.port)
            if self.cap.isOpened():
                break
        else:
            print("Can't open camera port!")
            raise RuntimeError("Can't open camera port!")
        
        if self.FrameWidth is not None:
            self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, self.FrameWidth)

        if self.FrameHeight is not None:
            self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, self.FrameHeight)


    def close(self):
        if self.cap is not None:
            self.cap.release()
            self.cap = None


    def get_frame(self, nTries=5):
        #if not self.is_opened:
        if self.cap is None:
            self.open()
        
        frame = None
        self.cap.open(self.port)
        for i in range(nTries):
            ret, frame = self.cap.read()
            if frame is not None:
                break
        else:
            print("Can't get frame!")
            raise RuntimeError("Can't read frame from camera!")
        self.cap.release()
        return frame

    