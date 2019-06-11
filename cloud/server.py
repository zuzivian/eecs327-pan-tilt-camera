import sys
import tornado.ioloop
import tornado.web
import datetime
import time
import random
import face
import person
import cv2
import tracker
import random
from pid import PID

#curr_coords = None
#initBB = None
panPID = PID(0.16, 0.00, 0.00)
# panPID.initialize()
tiltPID = PID(0.24, 0.00, 0.00)
#tiltPID.initialize()

class MainHandler(tornado.web.RequestHandler):
    def set_extra_headers(self):
        self.set_header("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0")
    def get(self):
        self.write('<img src="img/camera.jpeg?t=' + str(time.time()) + '" />')

    def post(self):
        global cvtracker
        data = self.request.body
        starttime = int(time.time() * 1000)
        # print('POST request  received')
        if 'Content-Type' in self.request.headers and self.request.headers['Content-Type'] == "image/jpeg":
            f = open("img/camera.jpeg", "wb")
            f.write(data)
            f.close()
            #midtime = int(time.time() * 1000)
            print('[POST] new image received: ' + str(sys.getsizeof(data))+ ' bytes' )
            # send response to camera client
            coords = -1
            tracked = False
            haars = [
              'haarcascade_frontalface_default.xml',
              #'haarcascade_frontalface_alt.xml',
              #'haarcascade_frontalface_alt2.xml',
              #'haarcascade_profileface.xml',
              #'haarcascade_upperbody.xml',
              #'haarcascade_lowerbody.xml',
              #'haarcascade_fullbody.xml'
            ]
            img = cv2.imread("img/camera.jpeg")
            if random.random() > 0.02:
              (coords, size) = tracker.find_tracker(img)
              if coords is not -1:
                tracked = True
            else:
              tracker.reset()
            #midtime = int(time.time() * 1000)
            for haar in haars:
              if coords != -1:
                break
              (coords, size) = face.find_haar(img, haar)
            if coords is -1:
              coords = (640/2, 480/2)
              # tracker.reset()
            elif tracked is False:
              tracker.new_coords(coords, size, img)
            midtime = int(time.time() * 1000)
            # calculate new PID angles
            angX = panPID.update( coords[0] - 640/2, not tracked )
            angY = tiltPID.update( coords[1] - 480/2, not tracked )
            # send results to server
            #midtime = int(time.time() * 1000)
            self.write("&&&" + str(int(-angX)) + ",,," + str(int(angY)) +"\r\n")
            print('detected x,y: ' + str(coords[0]) + ", " + str(coords[1]) )
            print('angles x,y:' + str(int(-angX)) + ", " + str(int(angY)))
            endtime = int(time.time()*1000)
            print('save img time: ' + str(midtime - starttime) + ' ms')
            print('procesing time: ' + str(endtime - midtime) + ' ms')
            print('total time: ' + str(endtime - starttime) + ' ms')
        else:
            self.write("500 Internal Server Error\r\n")

def make_app():
    return tornado.web.Application([
        (r"/", MainHandler),
	(r"/img/(.*)", tornado.web.StaticFileHandler, {"path": "/home/rubikcode/img/"}),
	(r"/webcam/(.*)", tornado.web.StaticFileHandler, {"path": "/home/rubikcode/webcam"}),
    ])

if __name__ == "__main__":
    # init PID controllers
    #curr_coords = None
    #initBB = None
    #panPID = PID(0.09, 0.08, 0.002)
    panPID.initialize()
    #tiltPID = PID(0.11, 0.10, 0.002)
    tiltPID.initialize()
    # start server
    app = make_app()
    app.listen(80)
    tornado.ioloop.IOLoop.current().start()

