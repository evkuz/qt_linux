from flask import Flask
from flask_restful import Api, Resource, abort
import os, config

# создание экземпляра приложения
app = Flask(__name__)
app.config.from_object(os.environ.get('FLASK_ENV') or 'config.DevelopementConfig')

# import views
from . import views
# from . import forum_views
# from . import admin_views
