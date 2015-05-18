#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import positionData
from resources import topicHeader

import sys

def distance_between(a, b):

	'''
	returns the distance between two points a and b

	a and b are positionData() instances.

	Check the kraken_msgs package, (msg/positionData.msg) file 
	for message details.
	'''

	return float(((a.x_coordinate - b.x_coordinate) ** 2 + (a.y_coordinate - b.y_coordinate) ** 2) ** 0.5)

# Distance to be moved must be passed as the
# first argument to the command

total_dist = float(sys.argv[1])

FIRST_ITERATION = True

Kp = 1.27;
Kd = 0.016;
Ki = 0.00;

errorI = 0.0
errorP = 0.0
errorD = 0.0
prevError = 0.0
initial_pose = positionData()

def positionCB(pose):
	global yaw
	global errorI
	global errorP
	global errorD
	global prevError
	global FIRST_ITERATION
	global base_yaw
	global initial_pose

	if FIRST_ITERATION:

		initial_pose.x_coordinate = pose.x_coordinate
		initial_pose.y_coordinate = pose.y_coordinate
		FIRST_ITERATION = False

	prevError = errorP
	errorP = total_dist - distance_between(pose, initial_pose)
	print errorP
	errorI = errorP + prevError
	errorD = errorP - prevError

if __name__ == '__main__':

	thruster4Data = thrusterData4Thruster();
	thruster6Data = thrusterData6Thruster();
	
	rospy.init_node('surge_control_node', anonymous=True)
	sub  = rospy.Subscriber(topicHeader.SENSOR_IMU, positionData, positionCB)
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

	r = rospy.Rate(10)

	while not rospy.is_shutdown():

		thruster6Data.data[0] = 0.0
		thruster6Data.data[1] = 0.0
		thruster6Data.data[2] = 0.0
		thruster6Data.data[3] = 0.0

		thruster6Data.data[4] = Kp*errorP + Kd*errorD + Ki*errorI
		thruster6Data.data[5] = thruster6Data.data[4]
		# thruster6Data.data[5] = Kp_right*errorP + Kd_right*errorD + Ki_right*errorI

		thruster4Data.data[0] = thruster6Data.data[0]
		thruster4Data.data[1] = thruster6Data.data[1]
		thruster4Data.data[2] = thruster6Data.data[4]
		thruster4Data.data[3] = thruster6Data.data[5]

		# pub4.publish(thruster4Data)
		pub6.publish(thruster6Data)
		
		r.sleep()
