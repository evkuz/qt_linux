from app import app
from flask import render_template, request, jsonify
from flask.wrappers import Response


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/pinkman')
def pinkman():
    return render_template('calibrate_pinkmanip.html')


@app.route('/simple')
def simple_page():
    return render_template('index_simple.html')


@app.route('/run', methods=['get'])
def run():
    args = request.args.to_dict()
    cmd = args.get("cmd")
    if cmd is not None:
        pass
    response = jsonify(None)
    response.headers.add("Access-Control-Allow-Origin", "*")
    
    return response
