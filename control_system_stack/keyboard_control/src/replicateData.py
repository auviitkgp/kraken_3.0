#!/usr/bin/env python

PKG = 'keyboard_control'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
from kraken_msgs.msg import thrusterData6Thruster

from resources import topicHeader

recieved = thrusterData6Thruster()

def thrustCB(data):

	global recieved

	recieved.data[0] = data.data[0]
	recieved.data[1] = data.data[1]
	recieved.data[2] = data.data[2]
	recieved.data[3] = data.data[3]
	recieved.data[4] = data.data[4]
	recieved.data[5] = data.data[5]

def startModule():

	thruster6Data=thrusterData6Thruster();
	
	# rospy.init_node('keep_thrusters_alive', anonymous=True)
	sub = rospy.Subscriber(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, thrustCB)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 10)

	r = rospy.Rate(10)

	while not rospy.is_shutdown():

		thruster6Data.data[0] = recieved.data[0]
		thruster6Data.data[1] = recieved.data[1]
		thruster6Data.data[2] = recieved.data[2]
		thruster6Data.data[3] = recieved.data[3]
		thruster6Data.data[4] = recieved.data[4]
		thruster6Data.data[5] = recieved.data[5]

		pub6.publish(thruster6Data)
		
		r.sleep()

import thread
thread.start_new_thread(startModule, ())
