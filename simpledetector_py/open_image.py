import cv2
import numpy as np


def fit_y_func(x):
    coeffs = [2.07066421e-03, 5.98578944e-01, 5.92908052e+01, 2.12814072e+03]
    return coeffs[0] * x * x * x + coeffs[1] * x * x + coeffs[2] * x + coeffs[3]
    
def fit_h_func(x):
    coeffs = [-5.27230123e-05, 3.18824287e-02, -6.93440162e+00, 6.52666043e+02]
    return coeffs[0] * x * x * x + coeffs[1] * x * x + coeffs[2] * x + coeffs[3]
    

def detect(frame, color_range):
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        blurred = cv2.GaussianBlur(hsv, (51, 51), 0)

        # create NumPy arrays from the boundaries
        lower = np.array(color_range[0], dtype="uint8")
        upper = np.array(color_range[1], dtype="uint8")
        # find the colors within the specified boundaries and apply
        # the mask
        mask = cv2.inRange(blurred, lower, upper)
        contours, _ = cv2.findContours(
            mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

        bestCountorArea = 0
        x, y, w, h = (0, 0, 0, 0)
        for contour in contours:
            area = cv2.contourArea(contour)
            if(area > 100) and area > bestCountorArea:
                bestCountorArea = area
                x, y, w, h = cv2.boundingRect(contour)
        
        detected = False
        ox, oy = (0, 0)

        if bestCountorArea > 200:
            ox = int(x + w / 2) - frame.shape[1]/2
            oy = frame.shape[0]/2 - int(y + h / 2)

            border_coef = w / h
            cw = w / frame.shape[1]
            ch = h / frame.shape[0]

            #if border_coef > 0.1 and border_coef < 4 and cw < 0.85 and cw > 0.08 and ch < 0.9 and ch > 0.08:
            if cw < 0.85 and cw > 0.05 and ch < 0.9 and ch > 0.05:    
                detected = True

        if detected:
            output = cv2.rectangle(
                hsv, 
                (x, y),
                (x + w, y + h),
                (0, 255, 0),
                2
            )
        else:
            output = cv2.rectangle(
                hsv, 
                (x, y),
                (x + w, y + h),
                (0, 0, 255),
                2
            )
        return output, (detected, ox, oy, w, h)


#im = cv2.imread("frame4.png")
im = cv2.imread("Untitled3.png")
im = cv2.flip(im, 0)

scale = 800. / im.shape[1]
width = int(im.shape[1] * scale)
height = int(im.shape[0] * scale)
dim = (width, height)

# resize image
resized = cv2.resize(im, dim, interpolation = cv2.INTER_AREA)

#color_range = ((20, 50, 200), (35, 85, 255)) # green
color_range = ((0, 230, 110), (20, 255, 190)) # orange
output, _ = detect(resized, color_range)

cv2.imshow('frame', output)

while True:
    key = cv2.waitKey(0)
    if key == 27:
        break
cv2.destroyAllWindows()
