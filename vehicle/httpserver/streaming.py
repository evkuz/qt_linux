from flask import Flask, render_template, Response
from camera import Camera
from test_camera import TestCamera
import cv2


app = Flask(__name__)
cam = Camera(0)
#cam = TestCamera()


@app.route('/')
def index():
    return render_template('index.html')

def gen(camera):
    while True:
        frame = camera.get_frame()
        (flag, encodedImage) = cv2.imencode(".jpeg", frame)
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + encodedImage.tobytes() + b'\r\n')

@app.route('/video_feed')
def video_feed():
    global cam
    return Response(
        gen(cam),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=False)