import os
from iqrdevice.config import BaseConfig

app_dir = os.path.abspath(os.path.dirname(__file__))

class DevelopementConfig(BaseConfig):
    DEBUG = True
    CAMERA_NUM = 0
    APP_DIR = app_dir
    SOCKET_PATH = "/run/lock/my.socket"
    QT_DEVICE_ADDR = "http://192.168.1.175:8383/"
    
class ProductionConfig(BaseConfig):
    DEBUG = False
    CAMERA_NUM = 0
    APP_DIR = app_dir
    SOCKET_PATH = "/home/ubuntu/iqr_lit/hiwonder/iqr.socket"
    QT_DEVICE_ADDR = "http://192.168.1.175:8383/"
