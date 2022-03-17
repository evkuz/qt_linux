import logging
logging.basicConfig(
    filename='pinkman.log',
    level=logging.INFO,
    filemode="w",
    format = "%(asctime)s - %(levelname)s - %(funcName)s: %(lineno)d - %(message)s"
)

import sys
import pathlib

module_path = pathlib.Path(__file__).parent.parent.parent.resolve()
sys.path.append(str(module_path))

import os
from iqrdevice import device, app

# создание экземпляра приложения
app.config.from_object(os.environ.get('FLASK_ENV') or 'config.DevelopementConfig')


from . import utils
cam = utils.CameraDetector(app.config['CAMERA_NUM'])
arduino_device = utils.SerialCommunication("Pinkman_arduino")
arduino_device.open_device(app.config['COM_PORT'])


# import views
from . import views

# import robot features
from . import actions
from . import services

device.set_name("PinkMan")

device.register_service(services.GetPositionService(cam))
device.register_service(services.CamCalibService(cam))
device.register_service(services.SetHomeService(arduino_device))
device.register_service(services.GetDistService(arduino_device))

device.register_action(actions.MoveAction(arduino_device))
device.register_action(actions.MoveHomeAction(arduino_device))
device.register_action(actions.CatchCubeAction(arduino_device, cam))
device.register_action(actions.PutCubeAction(arduino_device))
