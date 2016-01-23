#!/usr/bin/env python

PKG = 'depth_sensor'

import roslib; roslib.load_manifest(PKG)
import serial
import math
import rospy
from kraken_msgs.msg import dvlData
#import numpy


depth = serial.Serial()


depth.name = '/dev/ttyUSB0'
depth.baudrate =  9600
depth.stopbits = 1

pub = rospy.Publisher('/kraken/depthData', depthData, queue_size = 10)
rospy.init_node('depthdata', anonymous=True)

def readResponse():
    response = ''
    garbage = ''
    while( garbage != '\r'):
        garbage = depth.read(1)
        response += garbage
    
    return response

def startSensor():
    depth.open()
    
    if (depth.isOpen) :
        print 'Serial port opened successfully'
    else:
        print 'Error in opening port'
    
    response = ''
    #set baudRate = 9600
    depth.write('*9900BR=9600\r\n')
    response = readResponse()
    print 'In response of *9900BR=9600 recieved signal is : ',response
    ###
    
    #set parity = none
    depth.write('*9900PT=N\r\n')
    response = readResponse()
    print 'In response of *9900PT=N recieved signal is : ',response
    ###
    
    #enable EEPROM write and set Pressure unit = m H20
    depth.write('*0100EW*0100UN=8\r\n')
    response = readResponse()
    print 'In response of *0100EW*0100UN=8 recieved signal is : ',response
    ###
    
    
def getDepth():
    response = ''
    depth.write('*0100P3\r\n')
    
    response = readResponse()
    print 'In response od *0100P3 recieved signal is',response
    return float(response[1:])
    
    
if __name__ == '__main__':
    startSensor()
    
    count = 0     # variable to check frequency
#    pubData = depthData()   
    r = rospy.Rate(50)
    while not rospy.is_shutdown():
        print 'data count : ',count
        getDepth()
        
        #rospy.loginfo(pubData)
#        pub.publish(pubData)
        
        count = count + 1
        r.sleep()
    
    depth.close()
    
