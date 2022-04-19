import logging
logging.basicConfig(filename='blueman.log', level=logging.INFO, filemode="w")

import sys
import pathlib

module_path = pathlib.Path(__file__).parent.parent.parent.resolve()
sys.path.append(str(module_path))

import os
from iqrdevice import device, app
import iqrdevice.utils.vision as vision
from iqrdevice.utils.controllers import ArduinoManipulator

# создание экземпляра приложения
app.config.from_object(os.environ.get('FLASK_ENV') or 'config.DevelopementConfig')

from . import utils

cam = vision.OpenCVCamera(app.config['CAMERA_NUM'])
result_smoothing = 5
detector = vision.SimpleDetector(
    min_area=150,
    object_area_range=(0.005, 0.025),
    aspect_ratio_range=(0.6, 1.6),
    result_smoothing=result_smoothing
)
streamer = vision.VideoStreamer(cam, detector)

arduino_device = ArduinoManipulator("Blueman_arduino")
arduino_device.open_device(app.config['COM_PORT'])


# import views
from . import views

# import robot features
from . import actions
#from . import services
import iqrdevice.services as iqrservices
import iqrdevice.actions as iqractions

device.set_name("BlueMan")

device.register_service(iqrservices.GetObjectPositionService(cam, detector))
device.register_service(iqrservices.CamCalibService(cam,detector))
device.register_service(iqrservices.SetHomeService(arduino_device))
device.register_service(iqrservices.GetDistService(arduino_device))

grip_closed_val = app.config['GRIP_CLOSED']
grip_opened_val = app.config['GRIP_OPENED']

device.register_action(iqractions.MoveManipulatorAction(arduino_device))
device.register_action(iqractions.MoveHomeAction(arduino_device))
device.register_action(actions.CatchCubeAction(arduino_device, cam, detector, grip_opened_val, grip_closed_val))
device.register_action(actions.PutCubeAction(arduino_device, grip_opened_val, grip_closed_val))
device.register_action(iqractions.OpenGripAction(arduino_device, grip_opened_val))
device.register_action(iqractions.CloseGripAction(arduino_device, grip_closed_val))
