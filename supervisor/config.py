import os

app_dir = os.path.abspath(os.path.dirname(__file__))

class BaseConfig:
    vehicleAddr = "http://192.168.1.98:5000/"
    manipulatorAddr = "http://192.168.1.41:8384"
    manipulatorAddr = "http://159.93.44.84:8383/"
    manipulatorCameraAddr = "http://159.93.44.84:5555/"
    updateStateInterval = 400
    tpInterval = 500


class DevelopementConfig(BaseConfig):
    DEBUG = True


class TestingConfig(BaseConfig):
    DEBUG = True


class ProductionConfig(BaseConfig):
    DEBUG = False
