from __future__ import print_function
import cv2 as cv
import argparse
import odroid_wiringpi as wpi
import time
import serial  # Import the serial library

def detectAndDisplay(frame):
    frame_gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    frame_gray = cv.equalizeHist(frame_gray)
    #-- Detect faces
    faces = face_cascade.detectMultiScale(frame_gray)
    
    # Count the number of detected faces
    num_faces = len(faces)
    
    for (x, y, w, h) in faces:
        center = (x + w // 2, y + h // 2)
        frame = cv.ellipse(frame, center, (w // 2, h // 2), 0, 0, 360, (255, 0, 255), 4)
        faceROI = frame_gray[y:y + h, x:x + w]
    
    # Display the number of detected faces
    cv.putText(frame, f'Number of Faces: {num_faces}', (10, 30), cv.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    cv.imshow('Capture - Face detection', frame)
    return num_faces

parser = argparse.ArgumentParser(description='Code for Cascade Classifier tutorial.')
parser.add_argument('--face_cascade', help='Path to face cascade.', default='haarcascade_frontalface_alt.xml')
#parser.add_argument('--eyes_cascade', help='Path to eyes cascade.', default='haarcascade_eye_tree_eyeglasses.xml')
parser.add_argument('--camera', help='Camera divide number.', type=int, default=0)
args = parser.parse_args()
face_cascade_name = args.face_cascade

face_cascade = cv.CascadeClassifier()


#-- 1. Load the cascades
if not face_cascade.load(cv.samples.findFile(face_cascade_name)):
    print('--(!)Error loading face cascade')
    exit(0)

camera_device = args.camera

#-- 2. Read the video stream
cap = cv.VideoCapture(camera_device)
if not cap.isOpened():
    print('--(!)Error opening video capture')
    exit(0)

wpi.wiringPiSetup()
wpi.pinMode(8, 1)

# Configure the serial connection to the Arduino
arduino_port = '/dev/ttyS1'  # Replace with the correct port for your Arduino
arduino_baudrate = 115200
arduino_serial = wpi.serialOpen(arduino_port, arduino_baudrate)

while True:
    ret, frame = cap.read()
    if frame is None:
        print('--(!) No captured frame -- Break!')
        break
    number_of_face = detectAndDisplay(frame)

    try:
        wpi.serialPuts(arduino_serial, str(number_of_face))
        print(f'Sent Number of faces {number_of_face}')
        time.sleep(0.25)
    except Exception as e:
        print(f"Error sending count to Arduino: {str(e)}")
        
    if cv.waitKey(10) == 27:
        break