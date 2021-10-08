import cv2
import time
import threading
import numpy as np


class CameraDetector(object):
    def __init__(self, camPort, width=None, height=None):
        self.last_access = 0  # time of last client access to the camera
        self.color_range = ((0, 20, 240), (200, 250, 255))

        self.port = camPort
        self.FrameWidth = width
        self.FrameHeight = height
        self.__actualFrameBytes = None
        self.__actualFrame = None
        self.__thread = None
        self.__cap = cv2.VideoCapture()
        self.__isWorking = False
        self.__waitForFrame = threading.Event()
        self.__waitForFrame.clear()
        self.__waitForPosition = threading.Event()
        self.__waitForPosition.clear()
        self.position = (False, 0, 0, 0)

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
        frame = self.__actualFrameBytes
        CameraDetector.last_access = time.time()
        return frame
    
    def get_position(self):
        if not self.isStarted:
            self.start()

        self.__waitForPosition.wait()
        self.__waitForPosition.clear()
        res = self.position
        CameraDetector.last_access = time.time()
        return res

    def __detect(self, frame):
        blurred = cv2.GaussianBlur(frame, (51, 51), 0)

        # create NumPy arrays from the boundaries
        lower = np.array(self.color_range[0], dtype="uint8")
        upper = np.array(self.color_range[1], dtype="uint8")
        # find the colors within the specified boundaries and apply
        # the mask
        mask = cv2.inRange(blurred, lower, upper)
        contours, _ = cv2.findContours(
            mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

        bestCountorArea = 0
        x, y, w, h = (0, 0, 0, 0)
        for contour in contours:
            area = cv2.contourArea(contour)
            if(area > 100) and area > bestCountorArea:
                bestCountorArea = area
                x, y, w, h = cv2.boundingRect(contour)

        area = w * h
        ox = int(x + w / 2)
        oy = int(y + h / 2)

        border_coef = w / h
        cw = w / self.FrameWidth
        ch = h / self.FrameHeight
        detected = False

        if border_coef > 0.5 and border_coef < 1.6 and w < 0.8 and w > 0.030 and h < 0.9 and h > 0.09:
            output = cv2.rectangle(
                frame, 
                (x, y),
                (x + w, y + h),
                (0, 255, 0),
                2
            )
            detected = True
        else:
            output = cv2.rectangle(
                frame, 
                (x, y),
                (x + w, y + h),
                (0, 0, 255),
                2
            )
			
        return output, (detected, ox, oy, area)

    def calibrate_colors(self, x1, y1, x2, y2):
        if not self.isStarted:
            self.start()
        self.__waitForFrame.wait()
        
        color_lower = [255,255,255]
        color_upper = [0,0,0]
        if self.__actualFrame is None:
            return
        roi = self.__actualFrame[x1:x2-x1,y1:y2-y1].copy()
        for i in range(roi.shape[1]):
            for j in range(roi.shape[0]):
                b = roi[i, j, 0] # B
                g = roi[i, j, 1] # G
                r = roi[i, j, 2] # R
			
                if b < color_lower[0]:
                    color_lower[0] = b
                if b > color_upper[0]:
                    color_upper[0] = b

                if g < color_lower[1]:
                    color_lower[1] = g;
                if g > color_upper[1]:
                    color_upper[1] = g;

                if r < color_lower[2]:
                    color_lower[2] = r;
                if r > color_upper[2]:
                    color_upper[2] = r;

        minValue = 3;
        maxValue = 255 - minValue;

        if color_lower[0] <= minValue:
            color_lower[0] = 0
        else:
            color_lower[0] -= minValue
        
        if color_lower[1] <= minValue:
            color_lower[1] = 0
        else:
            color_lower[1] -= minValue

        if color_lower[2] <= minValue:
            color_lower[2] = 0
        else:
            color_lower[2] -= minValue


        if color_upper[0] >= maxValue:
            color_upper[0] = 255
        else:
            color_upper[0] += minValue
        
        if color_upper[1] >= maxValue:
            color_upper[1] = 255
        else:
            color_upper[1] += minValue

        if color_upper[2] >= maxValue:
            color_upper[2] = 255
        else:
            color_upper[2] += minValue

        self.color_range = (color_lower, color_upper)
        print(f"B: {color_lower[0]} - {color_upper[0]}\nG: {color_lower[1]} - {color_upper[1]}\nR: {color_lower[2]} - {color_upper[2]}")

    def __thread_work(self):
        self.__isWorking = True
        self.__open_device()

        while self.__isWorking:
            _, frame = self.__cap.read()
            frame, self.position = self.__detect(frame)
            (flag, encodedImage) = cv2.imencode(".jpeg", frame)
            self.__actualFrameBytes = encodedImage
            self.__actualFrame = frame.copy()
            self.__waitForFrame.set()
            self.__waitForPosition.set()
            time.sleep(0)
            # the last 5 seconds stop the thread
            if time.time() - self.last_access > 5:
                break

        self.__close_device()
        self.__thread = None
        self.__isWorking = False
