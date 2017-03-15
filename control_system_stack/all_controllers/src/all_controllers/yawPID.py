#!/usr/bin/env python

PKG = 'seabotix'

from pid import PID
import roslib; roslib.load_manifest(PKG)
import serial
import rospy
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import absoluteRPY
from resources import topicHeader
from dynamic_reconfigure.server import Server
from seabotix.cfg import k_changerConfig
import numpy as np
from math import *
from std_msgs.msg import Float64

#PID instance
YAW = PID()

#Assign global variables
base_yaw = 0.0
FIRST_ITERATION = True
prevError = 0.0
Current_yaw = -1
Client_goal = 300#None

errorI = 0.0
errorP = 0.0
errorD = 0.0
prevError = 0.0
out = 0.0
def imuCB(dataIn):

	global base_yaw
	global Current_yaw
	global Client_goal
	global errorI
	global errorP
	global errorD
	global prevError
	global FIRST_ITERATION
	global out
        

	if  FIRST_ITERATION :
    		base_yaw = dataIn.yaw
    		FIRST_ITERATION = False
        	 
        Current_yaw = dataIn.yaw
	print 'Yaw',Current_yaw
	prevError = errorP
	error = (base_yaw + Client_goal - dataIn.yaw)* 3.14 / 180
	errorP = np.arctan2(sin(error),cos(error))*180/3.14
	print 'Error',errorP
	errorI = errorP + prevError
	errorD = errorP - prevError
        out = YAW.k[0]*errorP + YAW.k[1]*errorD + YAW.k[2]*errorI#Kp = YAW.k[0] , Kd = YAW.k[1] , Ki = YAW.k[2]


#Instance of 6,4 thrusterData
thruster6Data = thrusterData6Thruster()
thruster4Data = thrusterData4Thruster()


	

def runYAW():
        print 'Enter yaw goal : '
	Client_goal = int(raw_input())
        rospy.init_node('Control', anonymous=True)	
	sub = rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, imuCB)
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)
	puberror = rospy.Publisher('Error', Float64, queue_size = 2)
	

	r = rospy.Rate(10)

	while not rospy.is_shutdown():

		thruster6Data.data[0] = 0.0
		#print Kp,Kd,Ki,errorP,errorD,errorI
		#print thruster6Data.data[0]
		#print out
		thruster6Data.data[1] = 0.0
		thruster6Data.data[2] = 0.0
		thruster6Data.data[3] = 0.0

		thruster6Data.data[4] = out
		thruster6Data.data[5] = -1*out

		thruster4Data.data[0] = thruster6Data.data[0]
		thruster4Data.data[1] = thruster6Data.data[1]
		thruster4Data.data[2] = thruster6Data.data[4]
		thruster4Data.data[3] = thruster6Data.data[5]

		pub4.publish(thruster4Data)
		pub6.publish(thruster6Data)
		puberror.publish(errorP)		

		r.sleep()


