from app import app
from flask import render_template, request, make_response
import json
from .supervisor import Supervisor
from supervisor.app.supervisor import Supervisor


supervisor = Supervisor(200)

def obj_to_dict(obj):
    return obj.__dict__

def make_json_responce(data:dict):
    response = make_response(json.dumps(data,  default = obj_to_dict))
    response.headers['Content-Type'] = 'application/json'
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response


@app.route('/')
def index():
    return render_template('index.html', devices=supervisor.ListDevices)


@app.route('/state')
def get_state():
    return make_json_responce(supervisor.State)

@app.route('/run_action', methods=['get'])
def run_action():
    args = request.args.to_dict()
    device = args.get("device")
    command = args.get("command")
    if device is None or command is None:
        return make_json_responce({'rc': -10})
    
    res = supervisor.run_action(device, command)
    return make_json_responce(res)

@app.route('/get_info', methods=['get'])
def get_info():
    args = request.args.to_dict()
    device = args.get("device")
    command = args.get("command")
    if device is None or command is None:
        return make_json_responce({'rc': -10})
    
    res = supervisor.get_info(device, command)
    return make_json_responce(res)

@app.route('/reset', methods=['get'])
def reset():
    args = request.args.to_dict()
    device = args.get("device")
    if device is None:
        return make_json_responce({'rc': -10})
    
    res = supervisor.reset_actions(device)
    return make_json_responce(res)








@app.route('/pinkman')
def pinkman():
    return render_template('calibrate_pinkmanip.html')


@app.route('/simple')
def simple_page():
    return render_template('index_simple.html')

