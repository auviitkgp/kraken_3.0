#!/usr/bin/env python

PKG = 'nqDVL'

import roslib; roslib.load_manifest(PKG)
import serial
import math
import struct
import numpy
import rospy
from resources import topicHeader
from kraken_msgs.msg import dvlData
#dataList = ['error_code[0]' , 'error_code[1]' , 'error_code[2]' , 'error_code[3]' , 'good_or_bad[0]' , 'good_or_bad[1]' , 'good_or_bad[2]' , 'good_or_bad[3]' , 'v_altitude[0]' , 'v_altitude[1]' , 'v_altitude[2]' , 'v_altitude[3]' , 'velo_rad[0]' , 'velo_rad[1]' , 'velo_rad[2]' , 'velo_rad[3]' , 'wvelo_rad[0]' , 'wvelo_rad[1]' , 'wvelo_rad[2]' , 'wvelo_rad[3]' , 'wvelo_credit[0]' , 'wvelo_credit[1]' , 'wvelo_credit[2]' , 'wvelo_credit[3]' , 'velo_instrument_x' , 'velo_instrument_y' , 'velo_instrument_z' , 'velo_instrument_flag' , 'velo_earth_n' , 'velo_earth_e' , 'velo_earth_d' , 'velo_earth_flag' , 'water_velo_instrument_x' , 'water_velo_instrument_y' , 'water_velo_instrument_z' , 'water_velo_instrument_flag' , 'water_velo_earth_n' , 'water_velo_earth_e' , 'water_velo_earth_d' , 'water_velo_earth_flag' , 'roll' , 'pitch' , 'heading' , 'altitude_estimate' , 'temperature' , 'salinity' , 'sound-speed' , 'check-sum',]

dataList = ['pitch' , 'roll', 'yaw', 'timeStamp', 'temp', 'depth', 'soundVel', 'BIT', 'transVelCurr', 'longVelCurr', 'normVelCurr', 'NA1', 'statusCurr', 'transVel', 'longVel', 'normVel', 'NA2', 'status', 'garbage', 'eastDist' , 'northDist', 'upDist', 'altitude', 'timedata']
#BIT : built-in-test record
#transVelCurr : tranverse velocity relative to current
#longVelCurr : longitudinal velocity relative to current
#normVelCurr : normal velocity relative to current
#NA1 : this data is not used
#statusCurr : 'A' => Valid data , 'I' => Invalid data
#transVel : transverse velocity
#longVel : longitudinal velocity
#normVel : normal velocity
#NA2 : this data is not used
#status : 'A' => Valid data , 'I' => Invalid data
#eastDist : listed as 'Not Used' in application note
#northDist :  listed as 'Not Used' in application note
#upDist :  listed as 'Not Used' in application note
#altitude : DVL altitude, '9999.99' means invalid altitude information
#timedata :  listed as 'Not Used' in application note

pub = rospy.Publisher('/kraken/dvl_data', dvlData, queue_size = 2)
rospy.init_node('dvldata', anonymous=True)
dvl = serial.Serial('/dev/ttyUSB0', 115200)

rawData = ''

## DVL config
dvl.stopbits = 2
##

def readDVLdata():
    global rawData
    garbage = ''
    rawData = ''
    
    while not rospy.is_shutdown():
        garbage = dvl.read(1)
        
        if (garbage != -1):
            
            if (garbage[0] == '$'):
                garbage = dvl.read(1)
                
                if (garbage[0] == '#'):
                    break
                
        else:
            print 'Unable to read'
            return
                
    while True :
        rawdata = '$#'
        garbage = dvl.read(1)
        
        if (garbage != -1):
            rawData += garbage
            

        
        if (garbage[0] == '$'):
            rawData += garbage
            while(garbage[0] != '\r'):
                garbage = dvl.read(1)
                rawdata += garbage
            break

    
def getVal(rawdata):
    floatData = []
    data = rawdata.split('\r')
    for i in range(1,len(data)-1):
        datal = (data[i]).split(',')
        
        for j in range(1,len(datal)):
            if ((datal[j] == 'A') or (datal[j] == 'I')):
                floatData += [datal[j]]
            else:
                floatData += [float(datal[j])]
           
            
    print floatData
    
    
if __name__ == '__main__':
    

    dvl.open()

    if (dvl.isOpen) :
        print 'Serial port opened successfully'
    else:
        print 'Error in opening port'
#    while True :
        
#        k = ''
#        while(k != '\r'):
#            k = dvl.read(1)
#            if (k == '$'):
#                print 'got it'
#            print k
#        print 'out'
#        k = ''    
    
    r = rospy.Rate(50)
    count = 1
    while not rospy.is_shutdown():
       
        readDVLdata()
        print 'data count : ',count 
        getVal(rawData)
        print ''
        count += 1
        
        #for i,a in enumerate(dataList):
        #    print i,' : ',a
        #rospy.loginfo(pubData)
        #pub.publish(pubData)
        
       
        r.sleep()
        
    
    
    dvl.close()