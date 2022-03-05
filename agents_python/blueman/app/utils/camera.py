import cv2
import time
import threading
import numpy as np
import copy


class CameraDetector(object):
    def __init__(self, camPort, width=640, height=480, showWindow=False):
        self.last_access = 0  # time of last client access to the camera
        self.color_range = ((140, 157, 232), (163, 171, 248))

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
        self.position = (False, 0, 0, 0, 0)
        
        self.__showWindow = showWindow

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
        
        self.__cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.0)
        self.__cap.set(cv2.CAP_PROP_EXPOSURE, 0.60)
        self.__cap.set(cv2.CAP_PROP_AUTO_WB, 0.0)
        self.__cap.set(cv2.CAP_PROP_AUTOFOCUS, 0.25)


    def __close_device(self):
        self.__cap.release()

    def start(self):
        if self.__thread is not None:
            return
        print("starting..")
        self.last_access = time.time()
        self.__thread = threading.Thread(target=self.__thread_work)
        self.__isWorking = True
        self.__thread.start()

    def stop(self):
        if self.__thread is None:
            return
        self.__isWorking = False
        self.__thread.join()

    def get_byte_frame(self):
        if self.__thread is None:
            self.start()

        self.__waitForFrame.wait()
        self.__waitForFrame.clear()
        frame = copy.deepcopy(self.__actualFrameBytes)
        self.last_access = time.time()
        return frame

    def get_frame(self):
        if self.__thread is None:
            self.start()

        self.__waitForFrame.wait()
        self.__waitForFrame.clear()
        frame = self.__actualFrame
        self.last_access = time.time()
        return frame
    
    def get_position(self):
        if self.__thread is None:
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
        
        detected = False
        ox, oy = (0, 0)

        if bestCountorArea > 500:
            ox = int(x + w / 2)
            oy = int(y + h / 2)

            border_coef = w / h
            cw = w / self.FrameWidth
            ch = h / self.FrameHeight

            #if border_coef > 0.1 and border_coef < 4 and cw < 0.85 and cw > 0.08 and ch < 0.9 and ch > 0.08:
            if cw < 0.85 and cw > 0.08 and ch < 0.9 and ch > 0.08:    
                detected = True

        if detected:
            output = cv2.rectangle(
                frame, 
                (x, y),
                (x + w, y + h),
                (0, 255, 0),
                2
            )
        else:
            output = cv2.rectangle(
                frame, 
                (x, y),
                (x + w, y + h),
                (0, 0, 255),
                2
            )
        return output, (detected, ox, oy, w, h)

    def calibrate_colors(self, x1, y1, x2, y2):
        if self.__thread is None:
            self.start()

        self.__waitForFrame.wait()

        color_lower = [255,255,255]
        color_upper = [0,0,0]
        if self.__actualFrame is None:
            return ""
        frameCpy = self.__actualFrame.copy()
        roi = frameCpy[y1:y2,x1:x2,:]
        for i in range(roi.shape[0]):
            for j in range(roi.shape[1]):
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
        res = f"(({color_lower[0]}, {color_lower[1]}, {color_lower[2]}), ({color_upper[0]}, {color_upper[1]}, {color_upper[2]}))"
        return res


    def __thread_work(self):
        self.__isWorking = True
        self.__open_device()
     
        while self.__isWorking:
            _, frame = self.__cap.read()
            frame, self.position = self.__detect(frame.copy())
            (flag, encodedImage) = cv2.imencode(".jpeg", frame)
            self.__actualFrameBytes = encodedImage
            self.__actualFrame = frame.copy()
            self.__waitForFrame.set()
            self.__waitForPosition.set()
            if self.__showWindow:
                cv2.imshow("Camera test", self.__actualFrame)
                key = cv2.waitKey(1)
                if key & 0xFF == ord('q'):
                    break
                self.last_access = time.time()

            time.sleep(0)
            # the last 5 seconds stop the thread
            if time.time() - self.last_access > 10:
                break

        self.__close_device()
        self.__thread = None
        self.__isWorking = False
        
        if self.__showWindow:
            cv2.destroyAllWindows()
        

if __name__ == '__main__':
    c = CameraDetector(2, showWindow=True)
    # winName = "Camera demonstration"
    # cv2.namedWindow(winName)
    #c.start()
    while True:
        c.get_frame()
        line = input()
        if len(line) == 0:
            continue

        if line == 'q':
            break
        par = line.split(" ")
        if len(par) == 4:
            c.calibrate_colors(
                int(par[0]),
                int(par[1]),
                int(par[2]),
                int(par[3])
            )

    c.stop()