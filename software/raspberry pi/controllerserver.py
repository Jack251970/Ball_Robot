from flask import Flask, request, jsonify, render_template_string
import serial
import threading
import json

import pigpio
from time import sleep

# Initialize hardware interface (USB port 0/1)
try:
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
except:
    try:
        ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
    except:
        ser = None
        print("No Arduino device found")
    

# Initialize pigpio
pi = pigpio.pi()

servo_pin_cam = 18
servo_pin_arm = 17
pi.set_mode(servo_pin_cam, pigpio.OUTPUT)
pi.set_mode(servo_pin_arm, pigpio.OUTPUT)

min_pulse_width = 500  # 0.5ms
max_pulse_width = 2500  # 2.5ms

def set_servo_pulsewidth(pin, angle):
    pulse_width = (angle / 180.0) * (max_pulse_width - min_pulse_width) + min_pulse_width
    pi.set_servo_pulsewidth(pin, pulse_width)

angle_cam = 120
set_servo_pulsewidth(servo_pin_cam, angle_cam)
angle_arm = 150
set_servo_pulsewidth(servo_pin_arm, angle_arm)

# Initialize Flask app
app = Flask(__name__)

current_command = 'X'
current_response = 'No response yet'

def read_serial():
    global current_response
    while True:
        if ser:
            response = ser.readline().decode('utf-8').strip()
            if response:
                current_response = response.upper()

@app.route('/')
def index():
    return render_template_string('''
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>Control Panel</title>
            <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
            <script>
                $(document).ready(function() {
                    $(document).keydown(function(event) {
                        var command = '';
                        if (event.which == 87) command = 'W';  // W key
                        if (event.which == 65) command = 'A';  // A key
                        if (event.which == 83) command = 'S';  // S key
                        if (event.which == 68) command = 'D';  // D key
                        if (event.which == 88) command = 'X';  // X key
                        if (event.which == 85) command = 'U';  // U key
                        if (event.which == 78) command = 'N';  // N key
                        if (event.which == 66) command = 'B';  // B key
                        if (event.which == 70) command = 'F';  // F key
                        if (event.which == 82) command = 'R';  // R key
                        if (event.which == 67) command = 'C';  // C key
                        if (event.which == 80) command = 'P';  // P key
                        if (command != '') {
                            $.post('/control', {command: command}, function(data) {
                                $('#currentCommand').text('Current Command: ' + data.command);
                            }, 'json');
                        }
                    });
                    //$(document).keyup(function(event) {
                    //    var command = 'X';  // Stop command
                    //    $.post('/control', {command: command}, function(data) {
                    //        $('#currentCommand').text('Current Command: ' + data.command);
                    //    }, 'json');
                    //});

                    function fetchResponse() {
                        $.get('/response', function(data) {
                            $('#currentResponse').text('Current Response: ' + data.response);
                        }, 'json');
                    }
                    setInterval(fetchResponse, 1000);  // Fetch response every second
                });
            </script>
        </head>
        <body>
            <h1>Control Panel</h1>
            <p>Press 'W', 'A', 'S', 'D', 'X' to control motors, 'U', 'B', 'N' to control servo of camera, 'F', 'R' to control arms, 'C', 'P' to control actions.</p>
            <p id="currentCommand">Current Command: X</p>
            <p id="currentResponse">Current Response: No response yet</p>
        </body>
        </html>
    ''')

@app.route('/control', methods=['POST'])
def control():
    global current_command
    global angle_cam
    command = request.form['command']
    current_command = command
    if command in ['W', 'S', 'A', 'D', 'X', 'C']:
        if ser is not None:
            ser.write(command.encode('utf-8'))
            return json.dumps({'command': command})
        else:
            return json.dumps({'command': 'X'})
    elif command in ['U', 'N', 'B']:
        if command == 'B':
            angle_cam = 120
        elif command == 'U':
            angle_cam = angle_cam + 3
        elif command == 'N':
            angle_cam = angle_cam - 3
            
        if angle_cam > 180:
            angle_cam = 180
        elif angle_cam < 68:
            angle_cam = 68
        
        set_servo_pulsewidth(servo_pin_cam, angle_cam)
        return json.dumps({'command': command})
    elif command in ['F', 'R']:
        global angle_arm
        if command == 'F':
            for angle in range(angle_arm, 24, -1):
                set_servo_pulsewidth(servo_pin_arm, angle)
                sleep(0.016)
            angle_arm = 25
        elif command == 'R':
            for angle in range(angle_arm, 151, 1):
                set_servo_pulsewidth(servo_pin_arm, angle)
                sleep(0.016)
            angle_arm = 150
        return json.dumps({'command': command})
    elif command in ['P']:
        for angle in range(150, 175):
            set_servo_pulsewidth(servo_pin_arm, angle)
            sleep(0.016)
        for _ in range(3):
            for angle in range(175, 125, -1):
                set_servo_pulsewidth(servo_pin_arm, angle)
                sleep(0.016)
            for angle in range(125, 175):
                set_servo_pulsewidth(servo_pin_arm, angle)
                sleep(0.016)
        for angle in range(175, 150):
            set_servo_pulsewidth(servo_pin_arm, angle)
            sleep(0.016)
        return json.dumps({'command': command})
    else:
        return "Invalid command", 400

@app.route('/response', methods=['GET'])
def response():
    global current_response
    return jsonify({'response': current_response})

def run_flask():
    app.run(host='0.0.0.0', port=5000)

# Start the Flask app in a background thread
flask_thread = threading.Thread(target=run_flask)
flask_thread.daemon = True
flask_thread.start()

# Start the serial reading thread
serial_thread = threading.Thread(target=read_serial)
serial_thread.daemon = True
serial_thread.start()

# Keep the main thread alive
while True:
    pass
