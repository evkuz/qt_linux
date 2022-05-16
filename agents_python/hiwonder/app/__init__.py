import sys
import pathlib

from flask import config
module_path = pathlib.Path(__file__).parent.parent.parent.resolve()
sys.path.append(str(module_path))

import os
from iqrdevice import device, app
import iqrdevice.utils.vision as vision

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

socket_server = utils.SocketServer(app.config["SOCKET_PATH"], cam, detector, result_smoothing)
socket_server.start()

qt_device = utils.HiwonderQt(app.config["QT_DEVICE_ADDR"], "hiwonder_qt", 0.3)
qt_device.start()

# import views
from . import views

# Import robot features
from . import actions
from . import services
from . import nodes

import iqrdevice.services as iqrservices
import iqrdevice.actions as iqractions
import iqrdevice.nodes as iqrnodes


device.set_name("Hiwonder")


# Configuring nodes
hascube_node = nodes.MyHasCubeNode()
position_node = nodes.ManipPositionNode()

# this is needed for connecting events with subscripers
mainbus = device.main_bus
qt_device.set_event_bus(mainbus) # all events will be written to this bus

mainbus.add_subscriber(qt_device.name + '/connected', grip_node)
mainbus.add_subscriber("actions", position_node)
mainbus.add_subscriber("actions", hascube_node)

#this is needed to show nodes in device state
device.register_node(hascube_node)
device.register_node(position_node)
device.register_node(grip_node)


# Configuring Services
device.register_service(services.CamCalibService(cam, detector))
device.register_service(services.RestartSocketServerService(socket_server))
device.register_service(services.CamDetectorService(cam, detector))
device.register_service(services.StartRMCommunicationService(qt_device))
device.register_service(services.StopRMCommunicationService(qt_device))


# Configuring Actions
device.register_action(iqractions.MoveManipulatorAction(arduino_device))
device.register_action(iqractions.MoveHomeAction(arduino_device))
device.register_action(actions.CatchCubeAction(arduino_device, cam, detector, grip_opened_val, grip_closed_val, min_catch_dist))
device.register_action(actions.PutCubeAction(arduino_device, grip_opened_val, grip_closed_val))
device.register_action(iqractions.OpenGripAction(arduino_device, grip_opened_val))
device.register_action(iqractions.CloseGripAction(arduino_device, grip_closed_val))
