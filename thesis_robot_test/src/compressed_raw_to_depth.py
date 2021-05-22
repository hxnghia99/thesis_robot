#!/usr/bin/env python

import cv2
import numpy as np
import rospy
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
import time
cvbridge = CvBridge()

class ImageConvert:
    def __init__(self):
        self.my_image = None
        self.frame_sub = rospy.Subscriber('camera/depth/image_raw', Image, self.aligned_callback)

    def aligned_callback(self, aligned_image):
        self.my_image = cv2.cvtColor(cvbridge.imgmsg_to_cv2(aligned_image, desired_encoding='passthrough'), cv2.COLOR_RGB2BGR)
        print(self.my_image.shape)
        print("abc")


def main():
    print("hello con khi kho")
    rospy.init_node('x_aligned_image', anonymous=True)
    time.sleep(1)
    while not rospy.is_shutdown():
        image = imagecvt.my_image
        #cv2.imshow("output", image)
        #print(image.shape)
        
    


if __name__ == '__main__':
    imagecvt = ImageConvert()
    main()
