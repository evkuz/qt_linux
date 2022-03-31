import cv2
import numpy as np
from os.path import isfile
from .basedetector import BaseDetector
from typing import List


class SimpleDetector(BaseDetector):
    def __init__(self, conf_file_name:str="cam_color_range.txt", min_area=100,
                       object_area_range=(0.02, 0.4), aspect_ratio_range=(1/2.5,2.5),
                       result_smoothing:int=3):
        self.filename = conf_file_name
        self.min_area = min_area
        self.object_area_range = object_area_range
        self.aspect_ratio_range = aspect_ratio_range
        self.result_smoothing = result_smoothing

        self.default_color_range = [
            np.array([0, 230, 155], dtype="uint8"),
            np.array([10, 255, 180], dtype="uint8")
        ]
        self.color_range = self.read_color_range()
        self.previous_results = []

    def calibrate(self, frame:np.ndarray, x1:int, y1:int, x2:int, y2:int)->str:
        """makes detector calibration

        Args:
            frame (_type_): frame that detector calibrates on
            x1 (int): top left X value
            y1 (int): top left Y value
            x2 (int): bottom right X value
            y2 (int): bottom right Y value

        Raises:
            NotImplementedError: must be reimplemented in derived
        Returns:
            str: information about calibration
        """
        color_lower = np.array([255, 255, 255], dtype="uint8")
        color_upper = np.array([0, 0, 0], dtype="uint8")

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        roi = hsv[y1:y2,x1:x2,:]
        for i in range(roi.shape[0]):
            for j in range(roi.shape[1]):
                b = roi[i, j, 0] # B
                g = roi[i, j, 1] # G
                r = roi[i, j, 2] # R
			
                if b < color_lower[0]:
                    color_lower[0] = b
                if b > color_upper[0]:
                    color_upper[0] = b

                if g < color_lower[1]:
                    color_lower[1] = g;
                if g > color_upper[1]:
                    color_upper[1] = g;

                if r < color_lower[2]:
                    color_lower[2] = r;
                if r > color_upper[2]:
                    color_upper[2] = r;

        minValue = 3;
        maxValue = 255 - minValue;

        if color_lower[0] <= minValue:
            color_lower[0] = 0
        else:
            color_lower[0] -= minValue
        
        if color_lower[1] <= minValue:
            color_lower[1] = 0
        else:
            color_lower[1] -= minValue

        if color_lower[2] <= minValue:
            color_lower[2] = 0
        else:
            color_lower[2] -= minValue


        if color_upper[0] >= maxValue:
            color_upper[0] = 255
        else:
            color_upper[0] += minValue
        
        if color_upper[1] >= maxValue:
            color_upper[1] = 255
        else:
            color_upper[1] += minValue

        if color_upper[2] >= maxValue:
            color_upper[2] = 255
        else:
            color_upper[2] += minValue

        self.color_range = (color_lower, color_upper)
        res = f"(({color_lower[0]}, {color_lower[1]}, {color_lower[2]}), ({color_upper[0]}, {color_upper[1]}, {color_upper[2]}))"
        return res

    def detect(self, image:np.ndarray, draw:bool=False)->dict:
        """Detect object on image

        Args:
            image: image frame to detect object on
            draw (bool, optional): if set to True, output dictionary will contain key 'output' with drowen detection results. Defaults to True.

        Raises:
            NotImplementedError: This method must be implemented in derived

        Returns:
            dict: result dictionary that must contain keys: 
                * 'detected' - bool value
             Addition mandatory keys in case of 'detected' is True
                * 'x' - float value of object center x in range [0,1]
                * 'y' - float value of object center y in range [0,1]
                * 'width' - float value of object width in range [0,1]
                * 'height' - float value of object height in range [0,1]
        """
        im_width = image.shape[0]
        im_height = image.shape[1]

        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        blurred = cv2.GaussianBlur(hsv, (11, 11), 0)

        # create NumPy arrays from the boundaries
        lower = self.color_range[0]
        upper = self.color_range[1]
        # find the colors within the specified boundaries and apply
        # the mask
        mask = cv2.inRange(blurred, lower, upper)
        contours, _ = cv2.findContours(
            mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

        # Searching for best countor
        bestCountorArea = 0
        bestCountor = None
        for contour in contours:
            area = cv2.contourArea(contour)
            if(area > self.min_area) and area > bestCountorArea:
                bestCountorArea = area
                bestCountor = contour

        if bestCountor is None:
            # nothing was detected
            return { 'detected' : False }
        
        x, y, w, h = cv2.boundingRect(bestCountor)
        res = {
            'detected' : False,
            'x'        : (x + 0.5*w - 0.5*im_width) / im_width,
            'y'        : (0.5*im_height - (y + 0.5*h)) / im_height,
            'width'    : w / im_width,
            'height'   : h / im_height
        }

        bestCountorArea = bestCountorArea / (im_width*im_height)

        if self.object_area_range[0] <= bestCountorArea <= self.object_area_range[1]:
            ar = w / h
            if self.aspect_ratio_range[0] <= ar <= self.aspect_ratio_range[0]:    
                res['detected'] = True
        
        res = self.smoothing_of_result(res)

        if draw:
            res['output'] = self.draw_result(image, res)
        
        return res

    def draw_result(self, frame:np.ndarray, detection_result:dict)->np.ndarray:
        """Draws given result of detection on given frame

        Args:
            frame: frame to draw result
            detection_result (dict): result of detection

        Raises:
            NotImplementedError: Must be implemented in derived
        """
        if detection_result['detected']:
            color = (0, 255, 0)
        else:
            color = (0, 0, 255)

        if not all([i in detection_result for i in ['x', 'y', 'width', 'height']]):
            return frame
        
        top_left = (
            int((detection_result['x']-0.5*detection_result['width']) * frame.shape[0]),
            int((detection_result['y']-0.5*detection_result['height']) * frame.shape[1])
        )
        bottom_right = (
            int((detection_result['x']+0.5*detection_result['width']) * frame.shape[0]),
            int((detection_result['y']+0.5*detection_result['height']) * frame.shape[1])
        )

        return cv2.rectangle(
            frame, 
            top_left,
            bottom_right,
            color,
            2
        )

    def read_color_range(self)->List[np.ndarray]:
        """Reads color ranges from file

        Returns:
            List[np.ndarray]: lower and upper color bounds
        """
        if not isfile(self.filename):
            return self.default_color_range
        
        with open(self.filename, 'r') as f:
            lines = f.readlines()
        if len(lines) < 2:
            return self.default_color_range

        res = [
            np.array([int(x.strip()) for x in lines[0].split(' ')], dtype="uint8"),
            np.array([int(x.strip()) for x in lines[1].split(' ')], dtype="uint8"),
        ]
        return res

    def write_color_range(self):
        """writes current color_range to file
        """
        with open(self.filename, 'w') as f:
             for l in self.color_range:
                 f.write(f"{l[0]} {l[1]} {l[2]}\n")

    def smoothing_of_result(self, current_res:dict)->dict:
        """Culculates detection result based on previous results.
        All result values calculated as average value except 'detected'.
        'detected' will be True only if all previous results have 'detected' = True

        Args:
            current_res (dict): current result you want to smooth

        Returns:
            dict: Fixed detection result
        """
        if self.result_smoothing == 0:
            return current_res
        
        if len(self.previous_results) > self.result_smoothing:
            self.previous_results.pop(0)
        self.previous_results.append(current_res)
        
        num_of_elements = len(self.previous_results)
        if num_of_elements == 1:
            return current_res

        return {
            'detected' : all([i['detected'] for i in self.previous_results]),
            'x'        : sum([i['x'] for i in self.previous_results])/num_of_elements,
            'y'        : sum([i['y'] for i in self.previous_results])/num_of_elements,
            'width'    : sum([i['width'] for i in self.previous_results])/num_of_elements,
            'height'   : sum([i['height'] for i in self.previous_results])/num_of_elements,
        }

__all__ = [ 'SimpleDetector' ]
