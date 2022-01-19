from app import app
from flask import render_template, request, make_response
from flask.wrappers import Response

from .camera import Camera
from . import robot_api

activeCamera = Camera(app.config['CAMERA_NUM'])
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

@app.route('/status')
def status():
    state = robotApi.status
    
    response = make_response(str(state))
    response.headers['Content-Type'] = 'application/json'
    response.headers.add("Access-Control-Allow-Origin", "*")
    
    return response


@app.route('/run', methods=['get'])
def run():
    args = request.args.to_dict()
    cmd = args.get("cmd")
    state = "None"
    if cmd is not None:
        if cmd == "reset":
            state = robotApi.reset()
        elif cmd == "status":
            state = robotApi.status
        else:
            state = robotApi.run_action(cmd)
        
    response = make_response(str(state))
    response.headers['Content-Type'] = 'application/json'
    response.headers.add("Access-Control-Allow-Origin", "*")

    return response

