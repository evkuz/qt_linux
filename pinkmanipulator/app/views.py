from app import app
from flask import render_template, request, jsonify, make_response
from flask.wrappers import Response

from .camera import CameraDetector
from . import robot_api
from .serial_communication import SerialCommunication

s_ports = SerialCommunication.serial_ports()
activeCamera = CameraDetector(app.config['CAMERA_NUM'])
robotApi = robot_api.RobotApi(camera=activeCamera, port=s_ports[0])

def gen(camera):
    while True:
        frame = bytearray(camera.get_byte_frame())
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
        elif cmd == "camcalib":
            x1 = args.get("x1")
            y1 = args.get("y1")
            x2 = args.get("x2")
            y2 = args.get("y2")
            if x1 is not None and y1 is not None and x2 is not None and y2 is not None:
                activeCamera.calibrate_colors(
                    int(x1), int(y1),
                    int(x2), int(y2)
                )
            state = robotApi.status
        elif cmd == "status":
            state = robotApi.status
        else:
            state = robotApi.run_action(cmd)
        
    response = make_response(str(state))
    response.headers['Content-Type'] = 'application/json'
    response.headers.add("Access-Control-Allow-Origin", "*")
    
    return response
