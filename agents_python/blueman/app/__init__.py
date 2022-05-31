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
result_smoothing = 1
detector = vision.SimpleDetector(
    min_area=150,
    object_area_range=(0.002, 0.6),
    aspect_ratio_range=(0.2, 3),
    result_smoothing=result_smoothing
)
streamer = vision.VideoStreamer(cam, detector)

arduino_device = ArduinoManipulator("Blueman_arduino")
arduino_device.open_device(app.config['COM_PORT'])


# Import views
from . import views

# Import robot features
from . import actions
from . import services
from . import nodes

import iqrdevice.services as iqrservices
import iqrdevice.actions as iqractions
import iqrdevice.nodes as iqrnodes

grip_closed_val = app.config['GRIP_CLOSED']
grip_opened_val = app.config['GRIP_OPENED']
min_catch_dist = app.config['CATCH_DIST']

device.set_name("BlueMan")


# Configuring nodes
hascube_node = nodes.MyHasCubeNode()
position_node = nodes.ManipPositionNode()
grip_node = iqrnodes.ManipGripNode(grip_closed_val, grip_opened_val, min_catch_dist)

# this is needed for connecting events with subscripers
mainbus = device.main_bus
arduino_device.set_event_bus(mainbus) # all events will be written to this bus

mainbus.add_subscriber(arduino_device.name + '/position', grip_node)
mainbus.add_subscriber("actions", position_node)
mainbus.add_subscriber("actions", hascube_node)

#this is needed to show nodes in device state
device.register_node(hascube_node)
device.register_node(position_node)
device.register_node(grip_node)


# Configuring Services
device.register_service(iqrservices.GetObjectPositionService(cam, detector))
device.register_service(iqrservices.CamCalibService(cam,detector))
device.register_service(iqrservices.SetHomeService(arduino_device))
device.register_service(iqrservices.GetDistService(arduino_device))


# Configuring Actions
device.register_action(iqractions.MoveManipulatorAction(arduino_device))
device.register_action(iqractions.MoveHomeAction(arduino_device))
device.register_action(actions.CatchCubeAction(arduino_device, cam, detector, grip_opened_val, grip_closed_val, min_catch_dist))
device.register_action(actions.PutCubeAction(arduino_device, grip_opened_val, grip_closed_val))
device.register_action(iqractions.OpenGripAction(arduino_device, grip_opened_val))
device.register_action(iqractions.CloseGripAction(arduino_device, grip_closed_val))
