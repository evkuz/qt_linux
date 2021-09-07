import argparse
import cv2
import numpy as np


color_range = ((80, 100, 190), (180, 200, 255))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument(
        '--cam-port', '-c',
        dest='cam_port',
        type=int,
        help='Camera port number',
        default=0
    )

    args = parser.parse_args()

    cap = cv2.VideoCapture()
    cap.open(args.cam_port)
    
    if not cap.isOpened():
        print("Can't open camera port!")
        exit(1)
        
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1024)

    while(True):
        ret, frame = cap.read()
        if frame is None:
            print("Can't get frame!")
            break

        cv2.imshow('frame', frame)
        
        key = cv2.waitKey(1)
        
        if key & 0xFF == ord('s'):
            cv2.imwrite("frame.png", frame)
	   
        if key & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
