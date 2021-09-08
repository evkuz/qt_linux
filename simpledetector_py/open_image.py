import cv2

im = cv2.imread("frame_upperLight.png")
cv2.imshow('frame', im)

cv2.waitKey(0)
cv2.destroyAllWindows()
