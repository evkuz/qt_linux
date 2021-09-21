import cv2

class IQRCamera:
    def __init__(self, camPort, width=None, height=None):
        self.port = camPort
        self.FrameWidth = width
        self.FrameHeight = height
        self.cap = cv2.VideoCapture()
    

    def __open(self, nTries=3):
        if self.cap.isOpened():
            self.cap.release()
        
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


    def get_frame(self, nTries=5):
        self.__open(nTries)
        
        frame = None
        for i in range(nTries):
            _, frame = self.cap.read()
            if frame is not None:
                break
        else:
            print("Can't get frame!")
            raise RuntimeError("Can't read frame from camera!")
        
        self.cap.release()

        return frame

    