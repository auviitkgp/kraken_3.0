#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
import sys
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import imuData
from kraken_msgs.msg import absoluteRPY
from resources import topicHeader

yaw = 0.0

if len(sys.argv) < 2:
	print 'You must give your desired Yaw as the first argument.'
	exit(0)

goal = float(sys.argv[1])

Kp_left = 1.27;
Kd_left = 0.016;
Ki_left = 0.00;

Kp_right = -1.27;
Kd_right = -0.016;
Ki_right = -0.00;

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

        yaw = dataIn.yaw

	prevError = errorP
	errorP = goal - yaw
	print errorP
	errorI = errorP + prevError
	errorD = errorP - prevError

#thruster6Data.data = [0.0,0.0,0.0,0.0,0.0,0.0]
#thruster4Data.data = [0.0, 0.0, 0.0, 0.0]


if __name__ == '__main__':
	thruster4Data=thrusterData4Thruster();
	thruster6Data=thrusterData6Thruster();

	rospy.init_node('Control', anonymous=True)
	sub = rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, imuCB)
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

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


