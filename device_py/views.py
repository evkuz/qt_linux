from device_py import app
from flask import render_template, request, make_response

from device_py import device
from device_py.action import ActionResponce
from device_py.baseresponce import BaseResponce
from device_py.status import StatusResponce
from device_py.service import ServiceResponce
from device_py.history import HistResponce


def make_my_responce(resp:BaseResponce):
    response = make_response(resp.to_json())
    response.headers['Content-Type'] = 'application/json'
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/status', methods=['get'])
def status():
    #TODO: error if wrong parameters
    args = request.args.to_dict()
    action_arg = args.get("action")
    actions = [] if action_arg is None else action_arg.split(",")
    state = device.get_status(actions)

    return make_my_responce(state)


@app.route('/action', methods=['get'])
def action():
    args = request.args.to_dict()
    name = args.get("name")
    if name is None:
        resp = ActionResponce("none", -1, "no action name was specified")
    else:
        try:
            resp = device.run_action(name, args)
        except Exception as e:
            resp = ActionResponce(name, -4, str(e))
    
    return make_my_responce(resp)


@app.route('/service', methods=['get'])
def service():
    args = request.args.to_dict()
    name = args.get("name")
    if name is None:
        resp = ServiceResponce("none", -1, "no service name was specified")
    else:
        try:
            resp = device.get_service_info(name, args)
        except Exception as e:
            resp = ServiceResponce(name, -4, str(e))
    
    return make_my_responce(resp)


@app.route('/hist', methods=['get'])
def hist():
    args = request.args.to_dict()
    type = args.get("type")
    
    if type is None:
        resp = HistResponce(-1, "no hist type was specified")
    else:
        try:
            name = args.get("name")
            name = [] if name is None else name.split(',')
            n = args.get("n")
            n = 20 if n is None else int(n)
            resp = device.get_hist(type, name, n)
        except Exception as e:
            resp = HistResponce(-2, str(e))
    
    return make_my_responce(resp)


@app.route('/run', methods=['get'])
def run():
    args = request.args.to_dict()
    cmd = args.get("cmd")
    if cmd is None:
        resp = BaseResponce(-127, "unknown cmd")
    else:
        if cmd == "status":
            action_arg = args.get("action")
            actions = [] if action_arg is None else action_arg.split(",")
            resp = device.get_status(actions)
        else:
            try:
                resp = device.run_action(cmd, args)
            except Exception as e:
                resp = ActionResponce(cmd, -4, str(e))

    return make_my_responce(resp)
