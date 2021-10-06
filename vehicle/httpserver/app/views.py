from app import app
from flask import render_template, request, jsonify
from flask.wrappers import Response

from .camera import Camera
#from .ROSCamera import ROSCamera
from .test_camera import TestCamera
from . import robot_api

activeCamera = TestCamera()
#activeCamera = Camera(app.config['CAMERA_NUM'])
robotApi = robot_api.RobotApi()

def gen(camera):
    while True:
        frame = bytearray(camera.get_frame())
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/video_feed')
def video_feed():
    response = Response(
        gen(activeCamera),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )
    return response


@app.route('/run', methods=['get'])
def run():
    args = request.args.to_dict()
    cmd = args.get("cmd")
    if cmd is not None:
        if cmd == "start":
            robotApi.move_robot(cmd)
        elif cmd == "reset":
            robotApi.reset()

    response = jsonify(robotApi.status.__dict__)
    response.headers.add("Access-Control-Allow-Origin", "*")
    
    return response
