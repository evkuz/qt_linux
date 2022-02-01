from . import app, cam
#from flask import render_template, request, jsonify, make_response
from flask.wrappers import Response


def gen(camera):
    while True:
        frame = bytearray(camera.get_byte_frame())
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/video_feed')
def video_feed():
    response = Response(
        gen(cam),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )
    return response