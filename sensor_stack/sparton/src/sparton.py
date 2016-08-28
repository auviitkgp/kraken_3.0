#!/usr/bin/env python

PKG = 'sparton'

import roslib; roslib.load_manifest(PKG)
import serial
import math
import struct
import numpy as np
import rospy
import sys
import os
import tf

# No need to use the below import, use the import in line 18 instead.
#from kraken_msgs.msg import imuData_new

from sensor_msgs.msg import Imu
from kraken_msgs.msg import imuData
from kraken_msgs.msg import magnetoTemp
from geometry_msgs.msg import Vector3 , Quaternion
from resources import topicHeader as th
pub1 = rospy.Publisher(th.SENSOR_IMU, imuData, queue_size = 2)
pub2 = rospy.Publisher(th.SENSOR_IMU_NEW, Imu, queue_size = 2)
#pub3 = rospy.Publisher(topic name, magnetoTemp, queue_size = 2)

rospy.init_node('imudata', anonymous=True)
## Code to find port automatically
find = os.popen('dmesg | grep FTDI')
port = find.read()
num = port.split('now attached to ')
portName = '/dev/'+(num[1].split('\n'))[0]
##
'''
if (len(sys.argv) == 2):
    num = str(sys.argv[1])
else:
    num = '0'
'''
imu = serial.Serial(portName, 115200)


# DVL config
imu.stopbits = 2
#

# Variables
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
    #print data
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

def getOrientationCovariance():

    # global roll
    # global pitch
    # global yaw

    if rospy.has_param('OrientationCov_mat'):
        cov_mat = rospy.get_param('OrientationCov_mat')
    else:
        cov_mat = [0.0] * 9
    return cov_mat

def getAngularVelocityCovariance():

    # global gx
    # global gy
    # global gz
    if rospy.has_param('AngularVelCov_mat'):
        cov_mat = rospy.get_param('AngularVelCov_mat')
    else:
        cov_mat = [0.0] * 9
    return cov_mat

def getLinearAccelerationCovariance():

    # global ax
    # global ay
    # global az
    if rospy.has_param('LinearAccelerationCov_mat'):
        cov_mat = rospy.get_param('LinearAccelerationCov_mat')
    else:
        cov_mat = [0.0] * 9
    return cov_mat

# Use inbuilt function `tf transform` instead
# def getQuaternion():

#     global roll
#     global pitch
#     global yaw

#     # roll, pitch, yaw should be in radians.
#     # Get quaternion from roll , pitch , yaw.

#     c1 = math.cos(yaw/2)
#     s1 = math.sin(yaw/2)
#     c2 = math.cos(pitch/2)
#     s2 = math.sin(pitch/2)
#     c3 = math.cos(roll/2)
#     s3 = math.sin(roll/2)
#     w = c1*c2*c3 - s1*s2*s3
#     x = c1*c2*s3 + s1*s2*c3
#     y = s1*c2*c3 + c1*s2*s3
#     z = c1*s2*c3 - s1*c2*s3
#     return Quaternion(w,x,y,z)

def new_msg_format():

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

    #msg1 = imuData_new()
    
    msg1 = Imu()
    msg2 = magnetoTemp()


    quaternion = tf.transformations.quaternion_from_euler(roll, pitch, yaw)
    msg1.orientation = Quaternion(quaternion[0],quaternion[1],quaternion[2],quaternion[3])
    msg1.orientation_covariance = getOrientationCovariance()
    msg1.angular_velocity = Vector3(gx,gy,gz)
    msg1.angular_velocity_covariance = getAngularVelocityCovariance() 
    msg1.linear_acceleration = Vector3(ax,ay,az)
    msg1.linear_acceleration_covariance = getLinearAccelerationCovariance()

    msg2.magnetometer = Vector3(mx,my,mz)
    msg2.temperature = temp

    return msg1 , msg2

if __name__ == '__main__':

    pubData = [0.0] * 13
    
    if (not imu.isOpen) :
        imu.close()
        imu.open()

    if (imu.isOpen) :
        print 'Serial port opened successfully'
    else:
        print 'Error in opening port'

    r = rospy.Rate(10)
    
    while not rospy.is_shutdown():

        pubData = getData()
        new_msg1 , new_msg2 = new_msg_format()
        pub1.publish(pubData)
        pub2.publish(new_msg1)
        pub2.publish(new_msg2)
        r.sleep()

    imu.close()
