#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import imuData
from resources import topicHeader

thruster4Data=thrusterData4Thruster();
thruster6Data=thrusterData6Thruster();
	

def heave(vel):
	global thruster6Data
	thruster6Data.data[0] = vel
	thruster6Data.data[1] = vel
	pub6.publish(thruster6Data)

def stopHeave():
	global thruster6Data
	thruster6Data.data[0] = 0.0
	thruster6Data.data[1] = 0.0
	pub6.publish(thruster6Data)

def surge(vel):
	global thruster6Data
	thruster6Data.data[4] = vel
	thruster6Data.data[5] = vel
	pub6.publish(thruster6Data)

def stopSurge():
	global thruster6Data
	thruster6Data.data[4] = 0.0
	thruster6Data.data[5] = 0.0
	pub6.publish(thruster6Data)


# def imuCB(dataIn):
# 	global yaw
# 	global errorI
# 	global errorP
# 	global errorD
# 	global prevError

# 	yaw = dataIn.data[2]

# 	prevError = errorP
# 	errorP = goal - yaw
# 	print errorP
# 	errorI = errorP + prevError
# 	errorD = errorP - prevError

#thruster6Data.data = [0.0,0.0,0.0,0.0,0.0,0.0]
#thruster4Data.data = [0.0, 0.0, 0.0, 0.0]
	thruster6Data.data[0] = 0.0
	thruster6Data.data[1] = 0.0
	thruster6Data.data[2] = 0.0
	thruster6Data.data[3] = 0.0
	thruster6Data.data[4] = 0.0
	thruster6Data.data[5] = 0.0
	

if __name__ == '__main__':
	
	rospy.init_node('Control', anonymous=True)
	# sub = rospy.Subscriber(topicHeader.SENSOR_IMU, imuData, imuCB)
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

	r = rospy.Rate(10)

	# while not rospy.is_shutdown():

	# 	thruster6Data.data[0] = 0.0
	# 	thruster6Data.data[1] = 0.0
	# 	thruster6Data.data[2] = 0.0
	# 	thruster6Data.data[3] = 0.0

	# 	thruster6Data.data[4] = Kp_left*errorP + Kd_left*errorD + Ki_left*errorI
	# 	thruster6Data.data[5] = Kp_right*errorP + Kd_right*errorD + Ki_right*errorI

	# 	thruster4Data.data[0] = thruster6Data.data[0]
	# 	thruster4Data.data[1] = thruster6Data.data[1]
	# 	thruster4Data.data[2] = thruster6Data.data[4]
	# 	thruster4Data.data[3] = thruster6Data.data[5]

		#pub4.publish(thruster4Data)
	#pub6.publish(thruster6Data)
		
	heave(50)
	rospy.sleep(3)
	stopHeave()
	rospy.sleep(1)
	surge(5)
	rospy.sleep(3)
	stopHeave()
	stopSurge()
	# r.sleep()


