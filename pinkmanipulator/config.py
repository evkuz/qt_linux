import os

app_dir = os.path.abspath(os.path.dirname(__file__))

class BaseConfig:
    ##### Настройка работы камеры
    CAMERA_NUM = 0
    COM_PORT = "/dev/ttyACM0"


class DevelopementConfig(BaseConfig):
    DEBUG = True
    CAMERA_NUM = 0
    COM_PORT = "/dev/ttyACM0"


class TestingConfig(BaseConfig):
    DEBUG = True
    CAMERA_NUM = 0
    COM_PORT = "/dev/ttyACM0"


class ProductionConfig(BaseConfig):
    DEBUG = False
    CAMERA_NUM = 0
    COM_PORT = "/dev/ttyACM0"