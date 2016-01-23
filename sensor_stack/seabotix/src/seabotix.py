#!/usr/bin/env python

import os
PKG = 'seabotix'

from time import sleep
import roslib; roslib.load_manifest(PKG)
import serial

import signal
import os


import rospy
import struct
from kraken_msgs.msg import seabotix
from resources import topicHeader

dataString = ''



sb = serial.Serial('/dev/ttyACM0', 9600)

#serial config
sb.stopbits = 1


def initSerial():
    if (not sb.isOpen) :
    	sb.open()

    if (sb.isOpen) :
        print 'Serial port opened successfully'
    
    else:
	    print 'Error in opening port'
    
#data to be sent to Arduino
data = [[0x5A,0,0x64],  #depth Front
	[0x60,0,0x64],  # depth back
	[0x50,0,0x64],  # surge left
	[0x5C,0,0x64],  # surge right
	[0x52,0,0x64],
	[0x5E,0,0x64]]


def seabotixCB(dataI):
    global data
    
    for i in range(len(data)):
	#for j in data[i]:
	
	#data[j][1] = chr(dataI.data[j])
	#print type(dataI.data[i]),(dataI.data[i])
        #data[i][1] = dataI.data[i]
	data[i][1] = struct.unpack('B',dataI.data[i])[0]
        '''
        dataString += chr(data.data[i])
        checksum += data.data[i]
    dataString += chr(checksum)
    sb.write(dataString)
    '''
	
        #print "%d" %data[i][1]

    
if __name__ == '__main__':

    initSerial()
   
    rospy.init_node('Thruster', anonymous=True)
    sub = rospy.Subscriber(topicHeader.CONTROL_SEABOTIX, seabotix, seabotixCB)
    
    
    #count = 0     # variable to check frequency   
    #add = [0X60,0X52,0X5A,0X50,0X5C,0X5E]
    #speed = [0X62,0X62,0X62,0X62,0X62,0X62]
    #speedMax = [0X64,0X64,0X64,0X64,0X64,0X64]
     #add[0] = 50
    #add[1] = '56'
    #add[2] = '5A'
    #add[3] = '5E'
    #add[4] = '52'
    #add[5] = '58'
    #add[4] = '60'
    #add[5] = '5C'
    
    default_rate = 8
    temp_rate = -1

    if rospy.has_param('/ros_rate'):
        temp_rate = float(rospy.get_param('/ros_rate'))
    else:
        temp_rate = default_rate

    r = rospy.Rate(temp_rate)
    
    print 'running'
    
    # print speed
    
    # print sb.readline()

    while not rospy.is_shutdown():
	for i in range(0,6):
		for j in range(0,3):
	    		sb.write(str(chr(int(data[i][j]))))
			# print sb.readline()
	
        r.sleep()
        
    
    sb.close()

    print "Running seabotix.py"	

    # os.system("python src/t.py")
