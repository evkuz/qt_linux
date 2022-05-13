from app import app
from flask import render_template, request, make_response
import json
from .supervisor import Supervisor


supervisor = Supervisor(0.500)

def obj_to_dict(obj):
    return obj.__dict__

def make_json_responce(data:dict):
    response = make_response(json.dumps(data,  default = obj_to_dict))
    response.headers['Content-Type'] = 'application/json'
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response


@app.route('/')
def index():
    return render_template('index.html', video_devices=supervisor.ListVideoDevices)


@app.route('/status')
def get_state():
    return make_json_responce(supervisor.State)

@app.route('/run_action', methods=['get'])
def run_action():
    args = request.args.to_dict()
    device = args.get("device")
    action = args.get("action")
    if device is None or action is None:
        return make_json_responce({'rc': -10})
    
    res = supervisor.run_action(device, action)
    return make_json_responce(res)

@app.route('/get_info', methods=['get'])
def get_info():
    args = request.args.to_dict()
    device = args.get("device")
    service = args.get("service")
    if device is None or service is None:
        return make_json_responce({'rc': -10})
    kwargs = {}
    for k, v in args.items():
        if k in ["device", 'service']:
            continue
        kwargs[k] = v
    res = supervisor.get_info(device, service, **kwargs)
    return make_json_responce(res)

@app.route('/reset', methods=['get'])
def reset():
    args = request.args.to_dict()
    device = args.get("device")
    if device is None:
        return make_json_responce({'rc': -10})
    
    res = supervisor.reset_actions(device)
    return make_json_responce(res)


@app.route('/techproc', methods=['get'])
def techproc():
    args = request.args.to_dict()
    set_value = args.get("set")
    if set_value is None:
        return make_json_responce({'rc': -10})
    
    val = set_value.lower()
    if val == 'true' or val == '1':
        supervisor.set_tech_process_state(True)
    elif val == 'false' or val == '0':
        supervisor.set_tech_process_state(False)
    else:
        return make_json_responce({'rc': -10})

    return make_json_responce({'rc': 0, 'info': 'success'})
