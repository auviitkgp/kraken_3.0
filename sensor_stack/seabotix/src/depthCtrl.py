#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import dvlData
from resources import topicHeader
from dynamic_reconfigure.server import Server
from seabotix.cfg import k_changerConfig

offset = 0
depth = 0.0
depthGoal = 0.5

Kp = 100
Kd = 44
Ki = 0

# Use in case of mismatch
Kp_front = 200;
Kd_front = 0.5;
Ki_front = 1;

Kp_back = 200;
Kd_back = 0.5;
Ki_back = 1;

errorI = 0.0
errorP = 0.0
errorD = 0.0
prevError = 0.0
i= 0 ;
def dvlCB(dataIn):
	global depth
	global depthGoal
	global offset
	global errorI
	global errorP
	global errorD
	global prevError
    	global i

	if(i < 3):
		offset = dataIn.data[6]
	  	i = i+1
	  	print i,offset
	  	if dataIn.data[6]>2:
	  		return;
        depth = offset - dataIn.data[6]
	

	prevError = errorP
	errorP = depthGoal - depth


	print 'Error',errorP

	errorI = errorP + prevError
	errorD = errorP - prevError

	

thruster6Data = thrusterData6Thruster()
thruster4Data = thrusterData4Thruster()


def callback(config,level):
	global Kp,Kd,Ki
	Kp=2*config.Kpl
	Kd=config.Kdl
	Ki=config.Kil
	return config
	

if __name__ == '__main__':

	rospy.init_node('Control', anonymous=True)
	sub = rospy.Subscriber(topicHeader.SENSOR_DVL, dvlData, dvlCB)
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)
	# srv=Server(k_changerConfig,callback)

	r = rospy.Rate(10)
	thruster6Data.data[2] = 0.0
	thruster6Data.data[3] = 0.0

	thruster6Data.data[4] = 0.0
	thruster6Data.data[5] = 0.0
	while not rospy.is_shutdown():
		

		thruster6Data.data[0] = Kp*errorP + Kd*errorD + Ki*errorI

		#print Kp,Kd,Ki,errorP,errorD,errorI
		#print thruster6Data.data[0]
		thruster6Data.data[1] = Kp*errorP + Kd*errorD + Ki*errorI
		

		thruster4Data.data[0] = thruster6Data.data[0]
		thruster4Data.data[1] = thruster6Data.data[1]
		thruster4Data.data[2] = thruster6Data.data[4]
		thruster4Data.data[3] = thruster6Data.data[5]

		#pub4.publish(thruster4Data)
		pub6.publish(thruster6Data)
		

		r.sleep()


