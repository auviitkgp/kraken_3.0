#!/usr/bin/env python

PKG = 'seabotix'

# stopKraken.py

# This module uses the serial library in python.

# It creates a handle for the serial port in the Arduino and then writes
# the data to be fed to the thrusters directly to this serial port.

# It does so using a while loop. (which may be redundant)

# The value "0x7F" is the zero for the seabotix thrusters. This is
# the reason that this value is part of the data that is being written
# to the serial port.

# TODO:

# - Check how the thrusters work
#     If only publishing once is enough, then the while loop is redundant, 
#     and we can stop the loop after publishing once.
# - Write a bash script to start this up directly from the repo root.


from time import sleep
import roslib; roslib.load_manifest(PKG)
import serial

import rospy
from kraken_msgs.msg import seabotix

dataString = ''

sb = serial.Serial('/dev/ttyACM0', 9600)

# serial config
sb.stopbits = 1

def initSerial():
    if (sb.isOpen == 0):
	sb.open()

    if (sb.isOpen) :
        print 'Serial port opened successfully'
    
    else:
	    print 'Error in opening port'
    
if __name__ == '__main__':

    initSerial()
   
    rospy.init_node('stopKraken', anonymous=True)
    
    # count = 0     # variable to check frequency   
    # add = [0X60,0X52,0X5A,0X50,0X5C,0X5E]
    # speed = [0X62,0X62,0X62,0X62,0X62,0X62]
    # speedMax = [0X64,0X64,0X64,0X64,0X64,0X64]
    data = [[0x60,0x7F,0x64],
	    [0x52,0x7F,0x64],
	    [0x5A,0x7F,0x64],
  	    [0x50,0x7F,0x64],
	    [0x5C,0x7F,0x64],
            [0x5E,0x7F,0x64]]

    # add[0] = 50
    # add[1] = '56'
    # add[2] = '5A'
    # add[3] = '5E'
    # add[4] = '52'
    # add[5] = '58'
    # add[4] = '60'
    # add[5] = '5C'
    
    r = rospy.Rate(1)
    
    print 'While Loop Started'
    
    while not rospy.is_shutdown():
        print 'Cycle Started'
        for i in range(0,6):
            for j in range(0,3):
                sb.write(str(chr(int(data[i][j]))))

        print 'Cycle Ended'	    
        r.sleep()
        
    sb.close()

    print "Exited from the While Loop"
    