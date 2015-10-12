#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)

import sys
import numpy as np
from math import *

import std_msgs.msg
import rospy

from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import imuData
from kraken_msgs.msg import absoluteRPY
from resources import topicHeader
from kraken_msgs.msg import setYawFeedback

# from kraken_msgs.msg import controllerResult

from FuzzyControl.fuzzy import Fuzzy
from FuzzyControl import fuzzyParams as Fparam

if(len(sys.argv) < 2):
	print "Enter yaw, to run this script."
	exit()
# Global variables
yaw = 0.0
goal = float(sys.argv[1])
base_yaw = 0.0
FIRST_ITERATION = True
prevError = 0.0


def imuCB(dataIn):
	"""
	This is imu call back function.

	1. Updates the Current value of yaw - Current_yaw
	2. Calculates error and delta_error based on 4 quadrant tangent = arctan2()
	3. Debug messages
	"""

	global base_yaw
	global prevError
	global yaw
	global FIRST_ITERATION

	Current_yaw = dataIn.yaw

	if  FIRST_ITERATION:
		base_yaw = Current_yaw
		FIRST_ITERATION = False

	prevError = YAW.error
	error = (base_yaw + goal - Current_yaw)* 3.14 / 180
	YAW.error = np.arctan2(sin(error),cos(error))*180/3.14
	YAW.delta_error = YAW.error - prevError

	yawData.Desired_yaw = (goal + base_yaw)%360
	yawData.Current_yaw = Current_yaw
	yawData.Error = YAW.error
	# yawData.header = std_msgs.msg.Header()
	# yawData.header.stamp = rospy.Time.now() # Note you need to call rospy.init_node() before this will work


	rospy.loginfo("--------")
	rospy.loginfo("Current Yaw : %s",round(Current_yaw,3))
	rospy.loginfo("Error : %s",round(YAW.error,3))
	rospy.loginfo("Delta_error : %s",round(YAW.delta_error ,3))
	rospy.loginfo("Goal : %s",goal)
	rospy.loginfo("Thruster data L : %s",round(thruster6Data.data[4],3))
	rospy.loginfo("Thruster data R : %s",round(thruster6Data.data[5],3))


if __name__ == '__main__':
	"""
     1. Declare YAW as a Fuzzy object and Declare it's membership function and it's Range.
     2. Declares messages types for thruster4Data and thruster6Data
	 3. calculate the thrust from fuzzy control and send to the thruster converter.
    """
	YAW = Fuzzy(Fparam.mf_types, Fparam.f_ssets)
	YAW.io_ranges = Fparam.io_ranges

	thruster4Data=thrusterData4Thruster();
	thruster6Data=thrusterData6Thruster();

	yawData = setYawFeedback();

	rospy.init_node('main', anonymous=True)
	rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, imuCB)
	pub_thrusters4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub_thrusters6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

	# pub = rospy.Publisher('FuzzyPlot', controllerResult, queue_size=10)
	r = rospy.Rate(10)

	while not rospy.is_shutdown():

		thrust = YAW.run()
		thruster6Data.data[0] = 0.0
		thruster6Data.data[1] = 0.0
		thruster6Data.data[2] = 0.0
		thruster6Data.data[3] = 0.0
		thruster6Data.data[4] =  thrust     # Left Thruster
		thruster6Data.data[5] = -1 * thrust   # Rigt Thruster

		thruster4Data.data[0] = thruster6Data.data[0]
		thruster4Data.data[1] = thruster6Data.data[1]
		thruster4Data.data[2] = thruster6Data.data[4]
		thruster4Data.data[3] = thruster6Data.data[5]

		pub_thrusters4.publish(thruster4Data)
		pub_thrusters6.publish(thruster6Data)

		r.sleep()
