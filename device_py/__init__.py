from flask import Flask
import os, config

# создание экземпляра приложения
app = Flask(__name__)
#app.config.from_object(os.environ.get('FLASK_ENV') or 'config.DevelopementConfig')

from device_py.device import IQRDevice
device = IQRDevice("agent")

from device_py import views
