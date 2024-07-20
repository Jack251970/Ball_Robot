from flask import Flask, Response, render_template_string
from flask import Flask, Response, render_template_string
from picamera2 import Picamera2
import cv2

app = Flask(__name__)

def generate_frames():
    picam2 = Picamera2()
    config = picam2.create_still_configuration(main={"size": (640, 480)})
    picam2.configure(config)
    picam2.start()

    try:
        while True:
            frame = picam2.capture_array()
            frame[:, :, [0, 2]] = frame[:, :, [2, 0]]
            ret, buffer = cv2.imencode('.jpg', frame)
            if not ret:
                continue
            frame = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\n\r\n' + frame + b'\r\n')
    finally:
        picam2.close()

@app.route('/video_feed')
def video_feed():
    return Response(generate_frames(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
def index():
    return render_template_string('''
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>Video Stream</title>
        </head>
        <body>
            <h1>Video Stream</h1>
            <img src="{{ url_for('video_feed') }}" width="640" height="480">
        </body>
        </html>
    ''')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=True)
