from app import app
from flask import render_template, request, make_response

from . import robot_api
robotApi = robot_api.RobotApi()


@app.route('/')
def index():
    return render_template('index.html')


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
