#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial

import rospy
from kraken_msgs.msg import seabotix

dataString = ''

sb = serial.Serial('/dev/ttyACM2', 9600)

## serial config
sb.stopbits = 1
##
def initSerial():
    
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
    
    
    count = 0     # variable to check frequency   
    
    speed = ''
    for i in range(1,7):
        speed += chr(0x90);
        speed += chr(0xA0);
    
    r = rospy.Rate(50)
    print 'running'
    
    print speed
    
    while not rospy.is_shutdown():
        sb.write(speed)
        #print speed
        count = count + 1
        #print count
        #print dataString
        r.sleep()
        
    
    sb.close()
