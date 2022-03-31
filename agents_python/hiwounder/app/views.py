from . import app, streamer
#from flask import render_template, request, jsonify, make_response
from flask.wrappers import Response


def gen(cam):
    while True:
        frame = bytearray(cam.get_next_frame())
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/video_feed')
def video_feed():
    response = Response(
        gen(streamer),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )
    return response