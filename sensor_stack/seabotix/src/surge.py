#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import imuData
from resources import topicHeader

import sys

import time

def imuCallback(imu):

    print "Entered the callback!"

    global prev_acc
    global acc
    global first_iteration
    global prev_time
    global stop_vehicle
    global threshold

    acc = imu.data[3]

    if first_iteration:

        prev_acc = acc
        first_iteration = False

    pres_time = time.time()

    difftime = pres_time - prev_time

    prev_time = pres_time

    differential = (prev_acc - acc) / difftime

    if abs(differential) > threshold:

        stop_vehicle = True

prev_acc = 0.
acc = 0.
first_iteration = True
stop_vehicle = False
prev_time = time.time()
threshold = 100.
thrust_input = 40.

rospy.init_node('demonstration_surge_control', anonymous=True)
sub = rospy.Subscriber(topicHeader.SENSOR_IMU, imuData, imuCallback)
pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

while not rospy.is_shutdown():

    thruster4Data = thrusterData4Thruster()
    thruster6Data = thrusterData6Thruster()

    if stop_vehicle:

        thrust_input = 0.

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

    # pub4.publish(thruster4Data)
    pub6.publish(thruster6Data)

    if stop_vehicle:

        exit()
