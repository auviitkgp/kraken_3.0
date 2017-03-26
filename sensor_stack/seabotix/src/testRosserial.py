#!/usr/bin/env python

"""
1. positive Force on vehicle is along positive X , i.e, forward direction
2. Rotation along the positive axes are considerd to be positive
"""

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
import sys
import numpy as np
import signal
import os

from std_msgs.msg import Float32MultiArray
from kraken_msgs.msg import thrusterData6Thruster
from resources import topicHeader
from math import *

ctrl_c_pressed = False

def stopThrustersNow(s, f):

        global ctrl_c_pressed
        ctrl_c_pressed = True


#thruster6Data.data = [0.0,0.0,0.0,0.0,0.0,0.0]
#thruster4Data.data = [0.0, 0.0, 0.0, 0.0]

if __name__ == '__main__':
	thruster6Data=thrusterData6Thruster();
        
        signal.signal(signal.SIGINT, stopThrustersNow)

	rospy.init_node('Control', anonymous=True)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

        if rospy.has_param('/ros_rate'):
            temp_rate = float(rospy.get_param('/ros_rate'))
        else:
            temp_rate = 8

	r = rospy.Rate(temp_rate)
        rospy.loginfo('Running with ROS_RATE of %0.2f Hz', temp_rate)

	while not rospy.is_shutdown():

		thruster6Data.data[0] = 00.0 # front
		thruster6Data.data[1] = 00.0 # back
		thruster6Data.data[2] = 00.0 # left
		thruster6Data.data[3] = 00.0 # right
		thruster6Data.data[4] = 00.0
		thruster6Data.data[5] = 00.0

		#print ctrl_c_pressed
                if ctrl_c_pressed:

		    thruster6Data.data = [0.0,0.0,0.0,0.0,0.0,0.0]

		pub6.publish(thruster6Data)
