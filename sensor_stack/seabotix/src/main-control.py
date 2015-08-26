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

import time

goal = 0.
first_iteration = True
threshold= 0.0
Kp_left = 0.1
Kd_left = 0.00
Ki_left = 0.0000

Kp_depth = 0.16
Kd_depth = 0.1
Ki_depth = 0

errorI = 0.0
errorP = 0.0
errorD = 0.0
prevError = 0.0

errorI2 = 0.0
errorP2 = 0.0
errorD2 = 0.0
prevError2 = 0.0

stop_vehicle = False

thrust_input = 20.

def imuCallback(imu):

    global prev_acc
    global acc
    global first_iteration
#    global prev_time
    global stop_vehicle
    global threshold

    acc = imu.data[3]

    print acc

    if first_iteration:

        prev_acc = acc
        first_iteration = False

    pres_time = time.time()

    difftime = pres_time - prev_time

    prev_time = pres_time

    differential = (prev_acc - acc) #/ difftime

    if abs(differential) > threshold:

        #stop_vehicle = True
	pass
	

prev_acc = 0.
acc = 0.
first_iteration = True
stop_vehicle = False
prev_time = time.time()
threshold = 100.
dt =0.

def buoyCoordCB(buoy_coord):
	global errorI
	global errorP
	global errorD
	global prevError
	global first_iteration
	global goal
	global prev_time
	global errorI2
	global errorP2
	global errorD2
	global prevError2
	global dt
	if first_iteration:
		prev_time = time.time()
		first_iteration = False
		dt=1.0
	else:
		pres_time = time.time()
		dt = pres_time - prev_time
		prev_time = pres_time	
	x = buoy_coord.x_coordinate
	y = buoy_coord.y_coordinate

	prevError = errorP
	errorP = x - goal
	print 'errorP: ', errorP
	errorI = (errorP + errorI)*dt
	errorD = (errorP - prevError)/dt
	
	prevError2 = errorP2
	errorP2 = goal - y
	print 'errorP2: ', errorP2
	errorI2 = (errorP2 + errorI2)*dt
	errorD2 = (errorP2 - prevError2)/dt

if __name__ == '__main__':

	thruster4Data=thrusterData4Thruster();
	thruster6Data=thrusterData6Thruster();
	
	rospy.init_node('buoy_control_node', anonymous=True)
	sub1 = rospy.Subscriber('/kraken/buoy/coordinates', positionData, buoyCoordCB)
    #sub = rospy.Subscriber(topicHeader.SENSOR_IMU, imuData, imuCallback)
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 1)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 1)

	r = rospy.Rate(10)

	while not rospy.is_shutdown():

#		x = int(raw_input("Enter x: "))
#		buoyCoord(x)

		thruster6Data.data[0] = 0.0
		thruster6Data.data[1] = 0.0
		thruster6Data.data[2] = 0.0
		thruster6Data.data[3] = 0.0

		thruster6Data.data[4] =    (Kp_left*errorP + Kd_left*errorD + Ki_left*errorI) + thrust_input
		thruster6Data.data[5] = -1*(Kp_left*errorP + Kd_left*errorD + Ki_left*errorI) + thrust_input

		thruster6Data.data[0] = 0. # Kp_depth*errorP2 + Kd_depth*errorD2 + Ki_depth*errorI2
		thruster6Data.data[1] = 0. # thruster6Data.data[0]

		thruster4Data.data[0] = thruster6Data.data[0]
		thruster4Data.data[1] = thruster6Data.data[1]
		thruster4Data.data[2] = thruster6Data.data[4]
		thruster4Data.data[3] = thruster6Data.data[5]

		if stop_vehicle:

			thruster6Data.data[4] = 0.
			thruster6Data.data[5] = 0.

			thruster6Data.data[0] = 0.
			thruster6Data.data[1] = 0.
		# pub4.publish(thruster4Data)
		pub6.publish(thruster6Data)

		r.sleep()
