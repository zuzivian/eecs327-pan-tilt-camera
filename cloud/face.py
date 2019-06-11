import numpy as np
import cv2

def find_haar(img, haarPath):
  uri = '/usr/local/lib/python3.5/dist-packages/cv2/data/'
  cascade = cv2.CascadeClassifier(uri + haarPath)
  gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
  features = cascade.detectMultiScale(gray, 1.3, 5)
  res = (-1, -1)
  for (x,y,w,h) in features:
    print("[HAAR] " + haarPath)
    print("[HAAR] feature detected at " + str(x) + ", " + str(y))
    cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
    roi_gray = gray[y:y+h, x:x+w]
    roi_color = img[y:y+h, x:x+w]
    if res[0] is -1:
      res = ( (x, y), (w,h) )
      cv2.imwrite('img/camera.jpeg',img)
  return res

if __name__ == "__main__":
  find_haar()
