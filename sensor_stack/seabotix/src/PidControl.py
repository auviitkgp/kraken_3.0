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

from std_msgs.msg import Float32MultiArray
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import imuData
from kraken_msgs.msg import absoluteRPY
from resources import topicHeader
from math import *



if(len(sys.argv) < 2):

	print "Enter yaw, to run this script."
	exit()

yaw = 0.0
goal = float(sys.argv[1])

base_yaw = 0.0
FIRST_ITERATION = True
factor=0.5
Kp_left = 0.95
Kd_left = 0.035
Ki_left = 0.0005

Kp_right = -0.95
Kd_right = -0.035
Ki_right = -0.0005

# Kp_left = 1.27;
# Kd_left = 0.046;
# Ki_left = 0.00;

# Kp_right = -1.27;
# Kd_right = -0.016;
# Ki_right = -0.00;


errorI = 0.0
errorP = 0.0
errorD = 0.0
prevError = 0.0

def imuCB(dataIn):
	global yaw
	global errorI
	global errorP
	global errorD
	global prevError
	global FIRST_ITERATION
	global base_yaw

	yaw = dataIn.yaw

	if  FIRST_ITERATION :

		base_yaw = yaw
		FIRST_ITERATION = False

	prevError = errorP
	errorP = (base_yaw + goal - yaw)*3.14/180
	errorP = np.arctan2(sin(errorP),cos(errorP))*180/3.14
	errorI = errorP + errorI
	errorD = errorP - prevError

	msg = Float32MultiArray()
	PlotData = [0]*3
	PlotData[0] = goal
	PlotData[1] = yaw
	PlotData[2] = errorP
	msg.data=PlotData
	pub.publish(msg)

	rospy.loginfo("--------")
	rospy.loginfo("Current Yaw : %s",round(yaw,3))
	rospy.loginfo("Error : %s",round(errorP,3))
	rospy.loginfo("Thruster data L : %s",thruster6Data.data[4])
	rospy.loginfo("Thruster data R : %s",thruster6Data.data[5])




#thruster6Data.data = [0.0,0.0,0.0,0.0,0.0,0.0]
#thruster4Data.data = [0.0, 0.0, 0.0, 0.0]


if __name__ == '__main__':
	thruster4Data=thrusterData4Thruster();
	thruster6Data=thrusterData6Thruster();

	rospy.init_node('Control', anonymous=True)
	sub = rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, imuCB)
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)
	pub = rospy.Publisher('ControlPlot', Float32MultiArray, queue_size=10)

	r = rospy.Rate(10)

	while not rospy.is_shutdown():

		thruster6Data.data[0] = 0.0
		thruster6Data.data[1] = 0.0
		thruster6Data.data[2] = 0.0
		thruster6Data.data[3] = 0.0

		thruster6Data.data[4] = Kp_left*errorP + Kd_left*errorD + Ki_left*errorI
		thruster6Data.data[5] = Kp_right*errorP + Kd_right*errorD + Ki_right*errorI

		thruster4Data.data[0] = thruster6Data.data[0]
		thruster4Data.data[1] = thruster6Data.data[1]
		thruster4Data.data[2] = thruster6Data.data[4]
		thruster4Data.data[3] = thruster6Data.data[5]

		#pub4.publish(thruster4Data)
		pub6.publish(thruster6Data)
