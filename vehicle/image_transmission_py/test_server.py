from iqr_camera import IQRCamera
from iqr_image_server import ImageServer
import socket


if __name__ == '__main__':
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    server_ip = s.getsockname()[0]
    camera = IQRCamera(0)
    server = ImageServer(server_ip, 1999, camera)
    server.start()