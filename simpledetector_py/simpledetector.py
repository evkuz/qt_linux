import argparse
import cv2
import numpy as np


color_range = ((80, 100, 190), (180, 200, 255))


def dectector(frame):
    global color_range
    blurred = cv2.GaussianBlur(frame, (51, 51), 0)
       
	# create NumPy arrays from the boundaries
    lower = np.array(color_range[0], dtype = "uint8")
    upper = np.array(color_range[1], dtype = "uint8")
	# find the colors within the specified boundaries and apply
	# the mask
    mask = cv2.inRange(blurred, lower, upper)
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    
    bestCountorArea = 0
    x, y, w, h = (0,0,0,0)
    for contour in contours:
        area = cv2.contourArea(contour)
        if(area > 100) and area > bestCountorArea:
            bestCountorArea = area
            x, y, w, h = cv2.boundingRect(contour)
            
    output = cv2.rectangle(
        frame, 
        (x, y),
        (x + w, y + h),
        (255, 0, 0),
        2
    )

    return output, (x, y, w, h)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument(
        '--gui', '-g',
        dest='gui',
        help="output file name",
        action="store_true",
        default=False
    )

    parser.add_argument(
        '--cam-port', '-c',
        dest='cam_port',
        type=int,
        help='Camera port number',
        default=0
    )

    parser.add_argument(
        '--com-port', '-p',
        dest='com_port',
        type=int,
        help='COM port number',
        default=0
    )

    args = parser.parse_args()

    showVideo = args.gui

    cap = cv2.VideoCapture()
    # The device number might be 0 or 1 depending on the device and the webcam
    cap.open(args.cam_port)

    while(True):
        ret, frame = cap.read()
        if frame is None:
            print("Can't get frame!")
            break

        det_frame, bounding_box = dectector(frame)
        if bounding_box[2] * bounding_box[3] > 0:
           x = int(bounding_box[0] + bounding_box[2]/2)
           y = int(bounding_box[1] + bounding_box[3]/2)
           print(x, y)

        if showVideo:
            cv2.imshow('frame', det_frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    cap.release()
    cv2.destroyAllWindows()