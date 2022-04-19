from flask import Flask
import os, config
import logging
logging.basicConfig(
    filename='agent-server.log',
    level=logging.INFO,
    filemode="w",
    format = "%(asctime)s - %(levelname)s - %(funcName)s: %(lineno)d - %(message)s"
)

# создание экземпляра приложения
app = Flask(__name__)
#app.config.from_object(os.environ.get('FLASK_ENV') or 'config.DevelopementConfig')

from iqrdevice.device import IQRDevice
device = IQRDevice("agent")

from iqrdevice import views

#print(device.__repr__())

from iqrdevice.eventbus import EventBus

__all__ = [
    'app',
    'device',
    'EventBus'
]