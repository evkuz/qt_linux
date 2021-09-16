from iqr_image_client import ImageClient
import cv2



if __name__ == '__main__':
    client = ImageClient("159.93.32.168", 1999)
    image = client.get_frame()
    #print(image.rows, image.cols)
    cv2.imshow('frame', image)
    cv2.waitKey(0)