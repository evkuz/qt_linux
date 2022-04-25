import sys
import pathlib
module_path = pathlib.Path(__file__).parent.parent.parent.resolve()
sys.path.append(str(module_path))

import os
from iqrdevice import device, app

# создание экземпляра приложения
app.config.from_object(os.environ.get('FLASK_ENV') or 'config.DevelopementConfig')

from iqrdevice.utils import vision
cam = vision.OpenCVCamera(
    app.config['CAMERA_NUM'],
    filters=[ vision.FlipFilter(True, True)]    
)
detector = vision.SimpleDetector(
    min_area=150,
    object_area_range=(0.004, 0.1),
    aspect_ratio_range=(0.6, 1.6),
    result_smoothing=0
)
streamer = vision.VideoStreamer(cam, detector)

# import views
from . import views

# import robot features
from . import actions
from . import services

device.set_name("mobile camera")
getPositionService = services.CamDetectorService(cam, detector)
device.register_service(services.CamCalibService(cam, detector))

device.register_service(getPositionService)
device.register_action(actions.MoveToTakeCube("http://192.168.1.205:8082", getPositionService, "http://192.168.1.201:8383"))
