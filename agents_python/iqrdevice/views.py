from flask import render_template, request, make_response

from .responces import ActionResponce
from .responces import BaseResponce
from .responces import ServiceResponce
from .responces import HistResponce
from .responces import StatusResponce
from .responces import LockResponce

from iqrdevice import app
from iqrdevice import device


def make_my_responce(resp:BaseResponce):
    response = make_response(resp.to_json())
    response.headers['Content-Type'] = 'application/json'
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response


@app.route('/')
def index():
    return render_template('index.html', device_addr=request.host_url)


@app.route('/status', methods=['get'])
def status():
    #TODO: error if wrong parameters
    args = request.args.to_dict()
    action_arg = args.get("action")
    actions = [] if action_arg is None else action_arg.split(",")
    state = device.get_status(actions)

    return make_my_responce(state)

@app.route('/reset', methods=['get'])
def reset():
    args = request.args.to_dict()
    action_arg = args.get("action")
    actions = [] if action_arg is None else action_arg.split(",")
    resp = device.reset_action(actions)

    return make_my_responce(resp)

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


@app.route('/control', methods=['get'])
def control():
    args = request.args.to_dict()
    set_arg = args.get("set")
    key_arg = args.get("key")
    if key_arg is None:
        return make_my_responce(LockResponce(device.is_locked, -2, "key wasn't set"))
    
    if set_arg == "lock":
        res = device.set_lock(key_arg)
    elif set_arg == "unlock":
        res = device.set_unlock(key_arg)
    else:
        return make_my_responce(
            LockResponce(
                device.is_locked,
                -2, "set must be one of [lock, unlock]"
            )
        )

    if res:
        return make_my_responce(
            LockResponce(device.is_locked, 0, "success")
        )
    else:
        return make_my_responce(
            LockResponce(device.is_locked, -1, "wrong validation key")
        )
