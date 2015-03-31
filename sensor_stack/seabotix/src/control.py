#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import imuData
from resources import topicHeader
from actionmsg.msg import buoyActionFeedback

PIXEL_RATIO=100.

yaw = 0.0
goal = 0.0
offset=0
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

	yaw = dataIn.data[2]

	prevError = errorP
	errorP = goal - yaw
	fabserror=abs(errorP)
	sign=1;
	if(errorP<0):
		sign=-1;
	if(fabserror>180):
		errorP=-1*(sign*(360-fabserror));
	


	print "Error P:", errorP, "; Yaw: ", yaw
	errorI = errorP + prevError
	errorD = errorP - prevError

#thruster6Data.data = [0.0,0.0,0.0,0.0,0.0,0.0]
#thruster4Data.data = [0.0, 0.0, 0.0, 0.0]

def goalCB(goalin):
	global goal
	goalin=goalin.feedback
	goal=(goal+ goalin.errorx/PIXEL_RATIO)%360
	print 'goal_new= ',goal,'goalin= ',goalin
	



if __name__ == '__main__':
	# offset=50;
	thruster4Data=thrusterData4Thruster();
	thruster6Data=thrusterData6Thruster();
	
	rospy.init_node('Control', anonymous=True)
	sub = rospy.Subscriber(topicHeader.SENSOR_IMU, imuData, imuCB)

	goalsub=rospy.Subscriber('/buoy/feedback',buoyActionFeedback,goalCB)

	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

	r = rospy.Rate(10)
	thruster6Data.data[0] = 0.0
	thruster6Data.data[1] = 0.0
	thruster6Data.data[2] = 0.0
	thruster6Data.data[3] = 0.0
	while not rospy.is_shutdown():

		

		thruster6Data.data[4] =Kp_left*errorP + Kd_left*errorD + Ki_left*errorI
		thruster6Data.data[5] =Kp_right*errorP + Kd_right*errorD + Ki_right*errorI

		if(abs(errorP)<10):
			thruster6Data.data[4]=offset;
			thruster6Data.data[5]=offset;			
			# print 'mind fuck'


		thruster4Data.data[0] = thruster6Data.data[0]
		thruster4Data.data[1] = thruster6Data.data[1]
		thruster4Data.data[2] = thruster6Data.data[4]
		thruster4Data.data[3] = thruster6Data.data[5]

		#pub4.publish(thruster4Data)
		pub6.publish(thruster6Data)
		

		r.sleep()


