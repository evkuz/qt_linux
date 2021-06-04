import cv2

im = cv2.imread("../dataset/0007.jpg")
cv2.imshow('frame', im)

cv2.waitKey(0)
cv2.destroyAllWindows()