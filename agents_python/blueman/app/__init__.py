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
arduino_device = utils.SerialCommunication(app.config['COM_PORT'])
arduino_device.open_device()


# import views
from . import views

# import robot features
from . import actions
from . import services

device.set_name("BlueMan")

device.register_service(services.GetPositionService(cam))
device.register_service(services.CamCalibService(cam))

device.register_action(actions.CatchCubeAction(arduino_device, cam))
device.register_action(actions.PutCubeAction(arduino_device))
