#!/usr/bin/env python

# testThrusters.py

# This module uses the serial library in python.

# It creates a handle for the serial port in the Arduino and then writes
# the data to be fed to the thrusters directly to this serial port.
# It does so using a while loop.

# When this script is run, and then, Ctrl+C is pressed in the terminal, then
# it sends a speed value of 0 to the arduino and closes the serial port
# and exits the program. If the vehicle doesn't stop immediately, then, wait for
# some time. (A little lag is normal, and expected)

PKG = 'seabotix'

from time import sleep
import roslib; roslib.load_manifest(PKG)
import serial

import rospy
from kraken_msgs.msg import seabotix

import os
import signal
import sys

zero_speed_set = False

dataString = ''

sb = serial.Serial('/dev/ttyACM0', 9600)

# serial config
sb.stopbits = 1

def stopThrustersNow(s=None, f=None):

    print "Stopping thrusters now!"
    global zero_speed_set
    zero_speed_set = True
    # signal.alarm(2)
    # sys.exit(0)

def initSerial():
    if (sb.isOpen == 0):
	sb.open()

    if (sb.isOpen) :
        print 'Serial port opened successfully'
    
    else:
	    print 'Error in opening port'
   
if __name__ == '__main__':

    signal.signal(signal.SIGINT, stopThrustersNow)
    initSerial()

    rospy.init_node('testThrusters', anonymous=True) # required for using rospy.Rate
    
    # count = 0     # variable to check frequency   
    # add = [0X60,0X52,0X5A,0X50,0X5C,0X5E]
    # speed = [0X62,0X62,0X62,0X62,0X62,0X62]   5A  and 60 for depth and 
    # speedMax = [0X64,0X64,0X64,0X64,0X64,0X64] n   52 and 5C are surge thrusters

    data = [[0x60,0xAA,0x64],  # Depth back
	    [0x50,0xAA,0x64],  # Surge left
	    [0x5A,0xAA,0x64],  # Depth front
  	    [0x52,0x80,0x64],  #
	    [0x5C,0xAA,0x64],  # Surge right
            [0x5E,0x62,0x64]]

    dataZero = [[0x60,0x80,0x64],  # Depth back
	    [0x50,0x80,0x64],  # Surge left
	    [0x5A,0x80,0x64],  # Depth front
  	    [0x52,0x80,0x64],  #
	    [0x5C,0x80,0x64],  # Surge right
            [0x5E,0x80,0x64]]

    # add[0] = 50
    # add[1] = '56'
    # add[2] = '5A'
    # add[3] = '5E'
    # add[4] = '52'
    # add[5] = '58'
    # add[4] = '60'
    # add[5] = '5C'
    rate = 8
    default_rate = 8
    if len(sys.argv) >= 2:
	try:
	    rate = int(sys.argv[1])
	except:
	    rate = default_rate
    else:
        if rospy.has_param('/ros_rate'):
	    rate = rospy.get_param('/ros_rate');
	else:
	    rate = default_rate
  
    r = rospy.Rate(rate)
    rospy.loginfo("Running at ROS Rate of %0.2f Hz", rate);

    print "Entering While Loop"
    # while not rospy.is_shutdown():
    i = 0
    while i < 32:
	if zero_speed_set:
	    print "Sending zero speed"
        print "Cycle Started"
        for i in range(0,6):
            for j in range(0,3):
                sb.write(str(chr(int(data[i][j] if not zero_speed_set else dataZero[i][j]))))
                # print "Single Thuster Data-point completed." # Total 18 data-points

	if zero_speed_set:
            break
		
        print "Cycle Ended."
	i += 1
        r.sleep()
        
    sb.close()
    # stopThrustersNow()
