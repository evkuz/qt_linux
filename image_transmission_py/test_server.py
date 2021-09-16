from iqr_camera import IQRCamera
from iqr_image_server import ImageServer
import cv2



if __name__ == '__main__':
    camera = IQRCamera(0)
    server = ImageServer("127.0.0.1", 1999, camera)
    server.start()