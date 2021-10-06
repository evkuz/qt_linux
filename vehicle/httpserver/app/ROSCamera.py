import cv2
import time
import threading
import rospy
from sensor_msgs.msg import CompressedImage
from subprocess import Popen


COMPRESSED_IMAGE_TOPIC = '/compressed_image'


class BaseCamera(object):
    def __init__(self):
        self.__thread = None
        self.__isWorking = False
        self.__waitForFrame = threading.Event()
        self.__waitForFrame.clear()

    @property
    def isStarted(self):
        return self.__isWorking

    def _open_device(self, nTries=3):
        pass

    def _close_device(self):
        pass

    def _get_frame(self):
        raise NotImplementedError();

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
        frame =  self.__actualFrame
        
        BaseCamera.last_access = time.time()
        return frame

    def __thread_work(self):
        self.__isWorking = True
        self._open_device()

        while self.__isWorking:
            self.__actualFrame = self._get_frame()
            self.__waitForFrame.set()
            time.sleep(0)
            #the last 5 seconds stop the thread
            if time.time() - self.last_access > 5:
                break

        self._close_device()
        self.__thread = None
        self.__isWorking = False

class ROSCamera(object):
    def __init__(self):
        self.__isOpened = False
        self.__bytesFrame = None

    def _open_device(self, nTries=3):
        if  self.__isOpened:
            return
        rospy.init_node("image_process")
        self.__sub = rospy.Subscriber("/compressed_image", CompressedImage, self.__read_frame)
        self.__r = rospy.Rate(24)
        self.__isOpened = True

    def _get_frame(self):
        if not rospy.is_shutdown():
            raise RuntimeError("rospy was closed!")
        
        self.__r.sleep()
        frame = self.__bytesFrame
        return frame

    def __read_frame(self, msg):
        self.__bytesFrame = bytes(msg.data)



if __name__ == '__main__':
    activeCamera = ROSCamera()
    while True:
        frame = activeCamera.get_frame()
        _, decodedFr = cv2.imdecode(frame, 1)
        cv2.imshow("WinName", decodedFr)

# fnum =1
# def newimage(msg):
# 	global fnum
# 	f=open('file'+str(fnum)+'.jpg','wb')
# 	f.write(bytes(msg.data))
# 	f.close()
# 	fnum+=1;
# print('start_program')
# rospy.init_node("image_process")
# COMPRESSED_IMAGE_TOPIC = '/compressed_image'
# sub = rospy.Subscriber("/compressed_image", CompressedImage, newimage)
# r = rospy.Rate(24)
# while not rospy.is_shutdown():
# 	r.sleep()