#!/usr/bin/env python

PKG = 'sparton'

import roslib; roslib.load_manifest(PKG)
import serial
import math
import struct
import numpy
import rospy
from kraken_msgs.msg import imuData



pub = rospy.Publisher('/kraken/imu_data', imuData, queue_size = 2)
rospy.init_node('imudata', anonymous=True)
imu = serial.Serial('/dev/ttyUSB0', 115200)


## DVL config
imu.stopbits = 2
##

## Variables
roll = 0.0
pitch = 0.0
yaw = 0.0
ax = 0.0
ay = 0.0
az = 0.0
mx = 0.0
my = 0.0
mz = 0.0
gx = 0.0
gy = 0.0
gz = 0.0
temp = 0.0
magError = 0.0
##
    
 
def setBaud(rate):
    if (rate == 300):
        message = '$PSPA,BAUD=0\r\n'
    elif (rate == 1200) :
        message = '$PSPA,BAUD=1\r\n'
    elif (rate == 2400) :
        message = '$PSPA,BAUD=2\r\n'
    elif (rate == 4800) :
        message = '$PSPA,BAUD=3\r\n'
    elif (rate == 9600) :
        message = '$PSPA,BAUD=4\r\n'
    elif (rate == 19200) :
        message = '$PSPA,BAUD=5\r\n'
    elif (rate == 38400) :
        message = '$PSPA,BAUD=6\r\n'
    elif (rate == 57600) :
        message = '$PSPA,BAUD=7\r\n'
    elif (rate == 115200) :
        message = '$PSPA,BAUD=8\r\n'
    
    imu.write(message)
    garbage = ''
    total = ''
    while (garbage != '\r'):
        garbage = imu.read()
        total += garbage
    #print total

def temperature():
    command = '$PSPA,TEMP\r\n'
    imu.write(command)
    garbage = ''
    total = ''
    data = []
    global temp 
    while (garbage != '\r'):
        garbage = imu.read()
        total += garbage
    #print total
    
    data = total.split(',')
    temp = float((data[1]).split('='))

def rpyt():
    command = '$xxXDR,\r\n'
    imu.write(command)
    garbage = ''
    total = ''
    data = []
    
    global roll 
    global pitch 
    global yaw 
    global temp 
    global magError 
    
    while (garbage != '\r'):
        garbage = imu.read()
        total += garbage
    #print total
    
    data = total.split(',')
    
    yaw = float(data[5])
    pitch = float(data[8])
    roll = float(data[11])
    temp = float(data[14])
    magError = float(((data[17]).split('*'))[0])
    
def accelero():
    command = '$PSPA,A\r\n'
    imu.write(command)
    garbage = ''
    total = ''
    global ax
    global ay 
    global az 
    
    data = []
    
    while (garbage != '\r'):
        garbage = imu.read()
        total += garbage
    #print total
    
    data = total.split(',')
    ax = float(((data[1]).split('='))[1])
    ay = float(((data[2]).split('='))[1])
    az = float(((((data[3]).split('='))[1]).split('*'))[0])
    
def gyro():
    command = '$PSPA,G\r\n'
    imu.write(command)
    garbage = ''
    total = ''
    data = []
    global gx 
    global gy 
    global gz

    while (garbage != '\r'):
        garbage = imu.read()
        total += garbage
    #print total
    
    data = total.split(',')
    gx = float(((data[1]).split('='))[1])
    gy = float(((data[2]).split('='))[1])
    gz = float(((((data[3]).split('='))[1]).split('*'))[0])

def magneto():
    command = '$PSPA,M\r\n'
    imu.write(command)
    garbage = ''
    total = ''
    data = []
    global mx 
    global my 
    global mz

    while (garbage != '\r'):
        garbage = imu.read()
        total += garbage
    #print total
    
    data = total.split(',')
    mx = float(((data[1]).split('='))[1])
    my = float(((data[2]).split('='))[1])
    mz = float(((((data[3]).split('='))[1]).split('*'))[0])

def pitchRoll():
    command = '$PSPA,PR\r\n'
    imu.write(command)
    garbage = ''
    total = ''
    data = []
    global pitch
    global roll

    while (garbage != '\r'):
        garbage = imu.read()
        total += garbage
    #print total
    
    data = total.split(',')
    ax = float(((data[1]).split('='))[1])
    ay = float(((data[2]).split('='))[1])
    az = float(((((data[3]).split('='))[1]).split('*'))[0])
 
def getData():
    global roll
    global pitch
    global yaw
    global ax
    global ay
    global az
    global mx
    global my
    global mz
    global gx
    global gy
    global gz
    global temp
    global magError
    
    accelero()
    gyro()
    magneto()
    rpyt()
    
    allData = imuData()
    
    allData.data[0] = roll
    allData.data[1] = pitch
    allData.data[2] = yaw
    allData.data[3] = ax
    allData.data[4] = ay
    allData.data[5] = az
    allData.data[6] = mx
    allData.data[7] = my
    allData.data[8] = mz
    allData.data[9] = gx
    allData.data[10] = gy
    allData.data[11] = gz
    allData.data[12] = temp
    
    return allData
    
 
if __name__ == '__main__':
    """    
    global roll
    global pitch
    global yaw
    global ax
    global ay
    global az
    global mx
    global my
    global mz
    global gx
    global gy
    global gz
    global temp

    """
    pubData = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
    
    imu.open()

    if (imu.isOpen) :
        print 'Serial port opened successfully'
    else:
        print 'Error in opening port'

    r = rospy.Rate(10)
    count = 1
    while not rospy.is_shutdown():

        pubData = getData()
        pub.publish(pubData)
        #print roll,pitch,yaw,ax,ay,az,mx,my,mz,gx,gy,gz,temp
        
        r.sleep()
        
    
    
    imu.close()