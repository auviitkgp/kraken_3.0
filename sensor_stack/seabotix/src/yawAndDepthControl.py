#!/usr/bin/env python

PKG = 'seabotix'

import sys
import roslib; roslib.load_manifest(PKG)
import serial
import rospy
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import dvlData
from kraken_msgs.msg import imuData
from resources import topicHeader
from dynamic_reconfigure.server import Server
from seabotix.cfg import k_changerConfig

if len(sys.argv) < 3:

	print "Give more arguments!"
	exit()


offset = 0.8
depth = 0.0
depthGoal = float(sys.argv[2])

yaw = 0.0
yawGoal = float(sys.argv[1])

Kp_left = 1.27;
Kd_left = 0.016;
Ki_left = 0.00;

Kp_right = -1.27;
Kd_right = -0.016;
Ki_right = -0.00;

yaw_errorI = 0.0
yaw_errorP = 0.0
yaw_errorD = 0.0
yaw_prevError = 0.0

Kp = 69
Kd = 27
Ki = 25

# Use in case of mismatch
Kp_front = 200;
Kd_front = 0.5;
Ki_front = 1;

Kp_back = 200;
Kd_back = 0.5;
Ki_back = 1;

depth_errorI = 0.0
depth_errorP = 0.0
depth_errorD = 0.0
depth_prevError = 0.0
i= 0

depth0 = 0.0
depth1 = 0.0

setyawnow = False
setdepthnow = False

def imuCB(dataIn):
	global yaw
	global yaw_errorI
	global yaw_errorP
	global yaw_errorD
	global yaw_prevError

	yaw = dataIn.data[2]

	yaw_prevError = yaw_errorP
	yaw_errorP = yawGoal - yaw
	yaw_errorI = yaw_errorP + yaw_prevError
	yaw_errorD = yaw_errorP - yaw_prevError

	global setyawnow
	setyawnow = True

def dvlCB(dataIn):
	global depth
	global depthGoal
	global offset
	global depth_errorI
	global depth_errorP
	global depth_errorD
	global depth_prevError
	global i

	if(i < 3):
		offset = dataIn.data[6]
	  	i = i+1
	  	print i,offset

        depth = offset - dataIn.data[6]
	

	depth_prevError = depth_errorP
	depth_errorP = depthGoal - depth
	print 'Error: ', depth_errorP
	depth_errorI = depth_errorP + depth_prevError
	depth_errorD = depth_errorP - depth_prevError

	global setdepthnow
	setdepthnow = True

def depthFeedbackCB(data):
	global depth0
	global depth1
	depth0 = data.data[0]
	depth1 = data.data[1]
	
thruster6Data = thrusterData6Thruster()
thruster4Data = thrusterData4Thruster()

def callback(config,level):
	global Kp,Kd,Ki
	Kp=2*config.Kpl
	Kd=config.Kdl
	Ki=config.Kil
	return config
	

if __name__ == '__main__':

	rospy.init_node('CompleteControl', anonymous=True)
	sub = rospy.Subscriber(topicHeader.SENSOR_DVL, dvlData, dvlCB)
	sub1 = rospy.Subscriber(topicHeader.SENSOR_IMU, imuData, imuCB)
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)
	srv=Server(k_changerConfig,callback)
	sub2 = rospy.Subscriber(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, depthFeedbackCB)

	r = rospy.Rate(10)

	while not rospy.is_shutdown():

		#print Kp,Kd,Ki,errorP,errorD,errorI
		#print thruster6Data.data[0]

		thruster6Data.data[0] = depth0
		thruster6Data.data[1] = depth1
		thruster6Data.data[4] = 0.0
		thruster6Data.data[5] = 0.0

		if setdepthnow:
			thruster6Data.data[0] = Kp*depth_errorP + Kd*depth_errorD + Ki*depth_errorI
			thruster6Data.data[1] = Kp*depth_errorP + Kd*depth_errorD + Ki*depth_errorI
			setdepthnow = False
			print "Setting depth now!", depth0, " : ", depth1

		if setyawnow:
			thruster6Data.data[4] = Kp_left*yaw_errorP + Kd_left*yaw_errorD + Ki_left*yaw_errorI
			thruster6Data.data[5] = Kp_right*yaw_errorP + Kd_right*yaw_errorD + Ki_right*yaw_errorI
			setyawnow = False
			print "Setting yaw now!"

		thruster6Data.data[2] = 0.0
		thruster6Data.data[3] = 0.0

		thruster6Data.data[0] = 4
		thruster6Data.data[1] = 4 

		thruster4Data.data[0] = thruster6Data.data[0]
		thruster4Data.data[1] = thruster6Data.data[1]
		thruster4Data.data[2] = thruster6Data.data[4]
		thruster4Data.data[3] = thruster6Data.data[5]

		#pub4.publish(thruster4Data)
		pub6.publish(thruster6Data)
		

		r.sleep()
