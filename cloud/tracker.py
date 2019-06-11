# import the necessary packages
from imutils.video import VideoStream
from imutils.video import FPS
import argparse
import imutils
import time
import cv2

# initialize a dictionary that maps strings to their corresponding
# OpenCV object tracker implementations
OPENCV_OBJECT_TRACKERS = {
	"csrt": cv2.TrackerCSRT_create,
	"kcf": cv2.TrackerKCF_create,
	"boosting": cv2.TrackerBoosting_create,
	"mil": cv2.TrackerMIL_create,
	"tld": cv2.TrackerTLD_create,
	"medianflow": cv2.TrackerMedianFlow_create,
	"mosse": cv2.TrackerMOSSE_create
}

#cvtracker = OPENCV_OBJECT_TRACKERS["csrt"]
cvtracker = cv2.TrackerMOSSE_create()

def reset():
  global cvtracker
  print('[TRACK] reset')
  cvtracker = cv2.TrackerCSRT_create()

def new_coords(coords, size, img):
  global cvtracker
  print('[TRACK] initialized tracker')
  # do something about initBB
  initBB = (coords[0], coords[1], size[0], size[1])
  cvtracker = cv2.TrackerCSRT_create()
  cvtracker.init(img, initBB)

def find_tracker(img):
  global cvtracker
  res = (-1, -1)
  (success, box) = cvtracker.update(img)
  if success:
    (x, y, w, h) = [int(v) for v in box]
    cv2.rectangle(img, (x, y), (x + w, y + h),(0, 255, 255), 2)
    res = ( (x, y) , (w,h) )
    cv2.imwrite("img/camera.jpeg", img)
    print("[TRACK] feature found at " + str(x) + ", " + str(y))
  return res
