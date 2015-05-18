#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import imuData
from kraken_msgs.msg import positionData
from resources import topicHeader

import sys

position_topic_name = topicHeader.PRESENT_POSE
initial_pose_x = 0.
initial_pose_y = 0.
total_dist = 0.
thrust_input = 0.

goalx = float(raw_input("Enter goalx: "))
goaly = float(raw_input("Enter goaly: "))

# goalx = 5
# goaly = 4

## Equations

max_thrust_input = 100.0

FIRST_ITERATION = True
THRUST_SET = True

# if(len(sys.argv) < 2):

# 	print "Enter yaw, to run this script."
# 	exit()

def distance_between(a, b):

	'''
	Distance between two points a and b
	a = (x1, y1)
	b = (x2, y2)

	Both a and b are tuples of length 2.
	'''

	return float(((a[0] - b[0])**2 + (a[1] - b[1])**2)**0.5)

def positionCallback(pose):

	global initial_pose_x
	global initial_pose_y
	global goalx
	global goaly
	global total_dist
	global max_thrust_input
	global thrust_input
	global a
	global FIRST_ITERATION
	global THRUST_SET

	pose_x = pose.x_coordinate
	pose_y = pose.y_coordinate

	if FIRST_ITERATION:

		initial_pose_x = pose_x
		initial_pose_y = pose_y
		total_dist = distance_between((initial_pose_x, initial_pose_y), (goalx, goaly))
		# Calculate the constant a
		a = max_thrust_input / (0.25*total_dist)**2

		print total_dist, a

		FIRST_ITERATION = False

	present_dist = distance_between((initial_pose_x, initial_pose_y), (pose_x, pose_y))

	if present_dist == 0.:

		thrust_input = 100.0

	elif (present_dist < total_dist / 8.0):

		thrust_input = a * (present_dist)**2

		print "Parabola 1"

	elif (present_dist < total_dist / 4.0):

		thrust_input = max_thrust_input - a * (total_dist/4. - present_dist)**2

		print "Parabola 2"

	elif (present_dist < total_dist * 0.75):

		thrust_input = max_thrust_input

		print "Constant 3"

	elif (present_dist < total_dist * (0.75 + 1/8.0)):

		thrust_input = max_thrust_input - a * (present_dist - total_dist*0.75)**2

		print "Parabola 3"

	else:

		thrust_input = a * (total_dist - present_dist)**2

		print "Parabola 4"

	thrust_input = thrust_input * 100
	print present_dist, thrust_input
	THRUST_SET = True

if __name__ == '__main__':

	thruster4Data=thrusterData4Thruster()
	thruster6Data=thrusterData6Thruster()

	rospy.init_node('surge_control', anonymous=True)

	rospy.Subscriber(name=position_topic_name, data_class=positionData, callback=positionCallback)
	
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

	r = rospy.Rate(10)

	while not rospy.is_shutdown():

		if THRUST_SET:

			thruster6Data.data[0] = 0.0
			thruster6Data.data[1] = 0.0
			thruster6Data.data[2] = 0.0
			thruster6Data.data[3] = 0.0
			thruster6Data.data[4] = thrust_input
			thruster6Data.data[5] = thrust_input

			thruster4Data.data[0] = thruster6Data.data[0]
			thruster4Data.data[1] = thruster6Data.data[1]
			thruster4Data.data[2] = thruster6Data.data[4]
			thruster4Data.data[3] = thruster6Data.data[5]

			#pub4.publish(thruster4Data)
			pub6.publish(thruster6Data)

			print "Published data!"
			THRUST_SET = False
		
		r.sleep()
