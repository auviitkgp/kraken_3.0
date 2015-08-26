#!/usr/bin/env python

# testThrusters.py

# This module uses the serial library in python.

# It creates a handle for the serial port in the Arduino and then writes
# the data to be fed to the thrusters directly to this serial port.

# It does so using a while loop.

PKG = 'seabotix'

from time import sleep
import roslib; roslib.load_manifest(PKG)
import serial

import rospy
from kraken_msgs.msg import seabotix

import os
import signal
import sys
import time

dataString = ''

sb = serial.Serial('/dev/ttyACM0', 9600)

# serial config
sb.stopbits = 1

def stopThrustersNow(signal, frame):

    print "Stopping thrusters now!"
    print os.getcwd()
    data = [[0x60,0x80,0x64],  #depth Back
	    [0x50,0x80,0x64],  #Surge Left
	    [0x5A,0x80,0x64],  #depth Front	    
            [0x52,0x80,0x64],  
            [0x5C,0x80,0x64],  #Surge Right  
	    [0x5E,0x80,0x64]]
    global sb
    print "Cycle Started"
    for i in range(0,6):
        for j in range(0,3):
            sb.write(chr(data[i][j]))
    	print "Thruster",(i+1),"stopped"	
    
    sb.close()
    exit()
    # time.sleep(1)
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
    # signal.signal(signal.SIGTERM, stopThrustersNow)
    initSerial()

    rospy.init_node('testThrusters', anonymous=True) # required for using rospy.Rate
    
    # count = 0     # variable to check frequency   
    # add = [0X60,0X52,0X5A,0X50,0X5C,0X5E]
    # speed = [0X62,0X62,0X62,0X62,0X62,0X62]   5A  and 60 for depth and 
    # speedMax = [0X64,0X64,0X64,0X64,0X64,0X64] n   52 and 5C are surge thrusters

    data = [
	    [0x60,0xAA,0x64],  #depth Back
	    [0x50,0xAA,0x64],  #Surge Left
	    [0x5A,0xCD,0x64],  #depth Front
	    [0x52,0xAA,0x64],  
            [0x5C,0xAA,0x64],  #Surge Right 
            [0x5E,0xAA,0x64]
	   ]

    # add[0] = 50
    # add[1] = '56'
    # add[2] = '5A'
    # add[3] = '5E'
    # add[4] = '52'
    # add[5] = '58'
    # add[4] = '60'
    # add[5] = '5C'
  
    r = rospy.Rate(10)
    
    # print "Entering While Loop"
    #print sb.readline()
    while not rospy.is_shutdown():
        print "Cycle Started :"
        for i in range(0,6):
            for j in range(0,3):
                sb.write(chr(data[i][j]))
	    print "Thruster ",(i+1)
		                
        r.sleep()
        
    sb.close()

    print "Exited from While Loop!"

    # os.system("python src/stopKraken.py")
