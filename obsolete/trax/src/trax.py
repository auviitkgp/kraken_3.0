#!/usr/bin/env python

PKG = 'tracks'

import roslib; roslib.load_manifest(PKG)
import serial
import math
import struct
import numpy
import rospy
from kraken_msgs.msg import imuData


pub = rospy.Publisher('/kraken/imuData', imuData, queue_size = 10)
rospy.init_node('imudata', anonymous=True)

imu = serial.Serial('/dev/ttyUSB0', 38400)

## IMU config
imu.stopbits = 2
##

# Function to get module info
def getInfo():
    modInfo = chr(0x00)
    modInfo += chr(0x05)
    modInfo += chr(0x01)
    modInfo += chr(0xEF)
    modInfo += chr(0xD4)
    imu.write(modInfo)


# Function to set all data as input    
def setAllMode():
    sensor_used = 0x0D
    payload = chr(0x00)  # MSB of sizeOf(payload)
    payload += chr(0x13) # LSB of sizeOf(payload)
    payload += chr(0x03) # Frame ID for setMode
    payload += chr(0x0D) # Number of device IDs
    payload += chr(0x19) # r
    payload += chr(0x18) # p
    payload += chr(0x05) # y
    payload += chr(0x15) # ax
    payload += chr(0x16) # ay
    payload += chr(0x17) # az
    payload += chr(0x1B) # mx
    payload += chr(0x1C) # my
    payload += chr(0x1D) # mz
    payload += chr(0x4A) # gx
    payload += chr(0x4B) # gy
    payload += chr(0x4C) # gz
    payload += chr(0x07) # temp
    
    payload += chr(0x86) # MSB(crc16 of payload)
    payload += chr(0xB4) # LSB(crc16 of payload)

    imu.write(payload)

# Function to convert hex data to float32
def hexToFloat(hexData):
    strData = ''
    #strData = hexData.encode('hex')
    
    intData = numpy.frombuffer(hexData,numpy.uint8)
    for i in range(0,4):
        data = (hex(intData[i]))[2:5]
        if (len(data) == 1):
            strData += '0'
        elif (len(data) == 0):
            strData += '00'
        strData += data
    
    floatData = struct.unpack('!f',strData.decode('hex'))[0]
    print floatData
    return floatData

    
# Function to send request for data
def getData():
    payload = chr(0x00)
    payload += chr(0x05)
    payload += chr(0x04)
    payload += chr(0xBF)
    payload += chr(0x71)
    imu.write(payload)
    response = imu.read(71)
    return response
    
# Function to get float32 values from response of IMU    
def getValue(response):
    extract = response[4:-2]       # initial 4 and last 2 bytes are useless
#    print response,len(response)
#    print len(extract)
    
    dataArray = imuData()
    
    for i in range(0,65,5):
        #print numpy.frombuffer(extract[i+1:i+5], numpy.uint8))
        dataArray.data[i/5] = hexToFloat(extract[i+1:i+5])
    return dataArray


    
if __name__ == '__main__':

    imu.open()

    if (imu.isOpen) :
        print 'Serial port opened successfully'
    else:
        print 'Error in opening port'
    
    getInfo()
    response = imu.read(13)
    print response[3:-2]
    
    pubData = [0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0]
    setAllMode()
    count = 0     # variable to check frequency
    pubData = imuData()   
    r = rospy.Rate(50)
    while not rospy.is_shutdown():
        print 'data count : ',count
        res = getData()
        pubData = getValue(res)
        pubData.data[0]*=(2.0*3.14)/360
        pubData.data[1]*=(2.0*3.14)/360
        pubData.data[2]*=(2.0*3.14)/360
        
        
        #rospy.loginfo(pubData)
        pub.publish(pubData)
        
        count = count + 1
        r.sleep()
        
    
    
    imu.close()