import os
from iqrdevice.config import BaseConfig

app_dir = os.path.abspath(os.path.dirname(__file__))

class DevelopementConfig(BaseConfig):
    DEBUG = True
    CAMERA_NUM = 0
    APP_DIR = app_dir
    
class ProductionConfig(BaseConfig):
    DEBUG = False
    CAMERA_NUM = 0
    APP_DIR = app_dir