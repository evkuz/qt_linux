import sys
import pathlib
module_path = pathlib.Path(__file__).parent.parent.parent.resolve()
sys.path.append(str(module_path))

import os
from iqrdevice import device, app
import iqrdevice.utils.vision as vision

# создание экземпляра приложения
app.config.from_object(os.environ.get('FLASK_ENV') or 'config.DevelopementConfig')

from . import utils

cam = vision.OpenCVCamera(app.config['CAMERA_NUM'])
detector = vision.SimpleDetector(
    min_area=150,
    object_area_range=(0.005, 0.025),
    aspect_ratio_range=(0.6, 1.6),
    result_smoothing=5
)
streamer = vision.VideoStreamer(cam, detector)
socket_server = utils.SocketServer(app.config["SOCKET_PATH"], cam, detector)
socket_server.start()

# import views
from . import views

# import robot features
from . import actions
from . import services

device.set_name("Hiwonder")
device.register_service(services.CamCalibService(cam, detector))
device.register_service(services.RestartSocketServerService(socket_server))
device.register_service(services.CamDetectorService(cam, detector))
