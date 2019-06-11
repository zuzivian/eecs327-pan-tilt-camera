# import the necessary packages
from __future__ import print_function
from imutils.object_detection import non_max_suppression
from imutils import paths
import numpy as np
import imutils
import cv2

def find_person(image):
  res = (-1, -1)
  # initialize the HOG descriptor/person detector
  hog = cv2.HOGDescriptor()
  hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())
  # detect people in the image
  (rects, weights) = hog.detectMultiScale(image, winStride=(4, 4), padding=(8, 8), scale=1.5)
  # draw the original bounding boxes
  for (x, y, w, h) in rects:
    cv2.rectangle(image, (x, y), (x + w, y + h), (0, 128, 255), 2)
  # apply non-maxima suppression to the bounding boxes using a
  # fairly large overlap threshold to try to maintain overlapping
  # boxes that are still people
  rects = np.array([[x, y, x + w, y + h] for (x, y, w, h) in rects])
  pick = non_max_suppression(rects, probs=None, overlapThresh=0.25)
  # draw the final bounding boxes
  for (xA, yA, xB, yB) in pick:
    cv2.rectangle(image, (xA, yA), (xB, yB), (0, 255, 128), 2)
    if res[0] is -1:
      res = ( ( xA/2 , yA/2 ), (xB-xA, yB-yA) )
      cv2.imwrite('img/camera.jpeg',image)
  # show some information on the number of bounding boxes
  print("[PERSON]: {} original boxes, {} after suppression".format(
    len(rects), len(pick)))
  return res
