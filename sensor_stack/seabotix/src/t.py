#!/usr/bin/env python

PKG = 'seabotix'

from time import sleep
import roslib; roslib.load_manifest(PKG)
import serial


import rospy
from kraken_msgs.msg import seabotix

dataString = ''

sb = serial.Serial('/dev/ttyACM0', 9600)

#serial config
sb.stopbits = 1

def initSerial():
    if (sb.isOpen == 0):
	sb.open()

    if (sb.isOpen) :
        print 'Serial port opened successfully'
    
    else:
	    print 'Error in opening port'
    

def seabotixCB(data):
    global dataString
    dataString = ''
    checksum = '0x00'
    
    for i in data:
        dataString += chr(data.data[i])
        checksum += data.data[i]
    dataString += chr(checksum)
    sb.write(dataString)


    
if __name__ == '__main__':

    initSerial()
   
    rospy.init_node('Thruster', anonymous=True)
    sub = rospy.Subscriber('/kraken/seabotix', seabotix, seabotixCB)
    
    
    #count = 0     # variable to check frequency   
    #add = [0X60,0X52,0X5A,0X50,0X5C,0X5E]
    #speed = [0X62,0X62,0X62,0X62,0X62,0X62]
    #speedMax = [0X64,0X64,0X64,0X64,0X64,0X64]
    data = [[0x60,0x7F,0x64],
	    [0x52,0x7F,0x64],
	    [0x5A,0x7F,0x64],
  	    [0x50,0x7F,0x64],
	    [0x5C,0x7F,0x64],
            [0x5E,0x7F,0x64]]
    #add[0] = 50
    #add[1] = '56'
    #add[2] = '5A'
    #add[3] = '5E'
    #add[4] = '52'
    #add[5] = '58'
    #add[4] = '60'
    #add[5] = '5C'

  

    r = rospy.Rate(1)
    
    print 'running'
    
    #print speed
    
    
    print sb.readline()
    while not rospy.is_shutdown():
	for i in range(0,6):
		for j in range(0,3):
	    		sb.write(str(chr(int(data[i][j]))))
			print sb.readline()
	
        r.sleep()
        
    
    sb.close()
