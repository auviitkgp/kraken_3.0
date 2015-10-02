#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
import sys

from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import imuData
from kraken_msgs.msg import absoluteRPY
from resources import topicHeader
from math import *
from FuzzyControl.fuzzy import Fuzzy
#------------------------------------------------------------------
# yapf: disable
## Test case fuzzy subset for trimf
# f_ssets = [[ # error
#             [-10,-10,-5],   # -ve medium
#             [-10,-5 , 0],    # -ve small
#             [-5 , 0 , 5],   # zero
#             [ 0 , 5 , 10],   # +ve small
#             [ 5 ,10 , 10], # +ve medium
#            ],
#             # delta_error
#            [
#             [-10,-10,-5],   # -ve medium
#             [-10,-5 , 0],    # -ve small
#             [-5 , 0 , 5],   # zero
#             [ 0 , 5 , 10],   # +ve small
#             [ 5 ,10 , 10], # +ve medium
#            ],
#             # u
#            [
#             [-10,-10,-5],  # -ve medium
#             [-10,-5 , 0],  # -ve small
#             [-5 , 0 , 5],  # zero
#             [ 0 , 5 , 10], # +ve small
#             [ 5 ,10 , 10], # +ve medium
#            ]
#           ]
# # yapf: enable

# io_ranges = [  # range of e
#               [-10,10],
#                # range of d_e
#               [-10,10],
#                # range of u
#               [-10,10]
#             ]

# mf_types = ['trimf','trimf','trimf']

#-------------------------------------------------------------------------------
## fuzzy subset test case for gaussmf
f_ssets = [[ # error
            [-180,70], # -ve medium
            [-50,20], # -ve small
            [ 0 ,20], # zero
            [50 ,20], # +ve small
            [180 ,70], # +ve medium
           ],
            # delta_error
           [
            [-180,70], # -ve medium
            [-50,20], # -ve small
            [ 0 ,20], # zero
            [50 ,20], # +ve small
            [180 ,70], # +ve medium
           ],
            # u
           [
            [-3,2], # -ve medium
            [-1,2], # -ve small
            [ 0,1], # zero
            [ 1,2], # +ve small
            [ 3,2], # +ve medium
           ]
          ]
# yapf: enable

io_ranges = [  # range of e
              [-180,180],
               # range of d_e
              [-180,180],
               # range of u
              [-10,10]
            ]

mf_types = ['gaussmf','gaussmf','gaussmf']

#---------------------------------------------------------------------------

if(len(sys.argv) < 2):

	print "Enter yaw, to run this script."
	exit()

yaw = 0.0
goal = float(sys.argv[1])
base_yaw = 0.0
FIRST_ITERATION = True
prevError = 0.0


def imuCB(dataIn):

    global base_yaw	
    global prevError
    global yaw
    global FIRST_ITERATION

    yaw = dataIn.yaw
    if  FIRST_ITERATION:
		base_yaw = yaw
		FIRST_ITERATION = False

    prevError = YAW.error
    print 'type(goal) : ',type(goal)
    print 'type base_yaw :',type(base_yaw)
    print 'type YAW.error : ',type(YAW.error) 
    YAW.error = base_yaw + goal - yaw
    YAW.delta_error = YAW.error - prevError


	##### Not needed here since imu data is already absoulute
	# errorP = errorP * 3.14 / 180
	# errorP = np.arctan2(sin(errorP),cos(errorP))
	# errorP = errorP * 180 / 3.14
    rospy.loginfo("--------")
    rospy.loginfo("Current Yaw : %s",round(yaw,3))
    rospy.loginfo("Error : %s",round(YAW.error,3))
    rospy.loginfo("Thruster data L : %s",thruster6Data.data[4])
    rospy.loginfo("Thruster data R : %s",thruster6Data.data[5])



#thruster6Data.data = [0.0,0.0,0.0,0.0,0.0,0.0]
#thruster4Data.data = [0.0, 0.0, 0.0, 0.0]


if __name__ == '__main__':
	YAW = Fuzzy(mf_types, f_ssets)
 	YAW.io_ranges = io_ranges

	thruster4Data=thrusterData4Thruster();
	thruster6Data=thrusterData6Thruster();

	rospy.init_node('main', anonymous=True)
	sub = rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, imuCB)
	pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

	r = rospy.Rate(10)

	while not rospy.is_shutdown():

		thruster6Data.data[0] = 0.0
		thruster6Data.data[1] = 0.0
		thruster6Data.data[2] = 0.0
		thruster6Data.data[3] = 0.0

		thruster6Data.data[4] =  YAW.run()   # Left Thruster
		thruster6Data.data[5] = -YAW.run()   # Rigt Thruster

		thruster4Data.data[0] = thruster6Data.data[0]
		thruster4Data.data[1] = thruster6Data.data[1]
		thruster4Data.data[2] = thruster6Data.data[4]
		thruster4Data.data[3] = thruster6Data.data[5]

		#pub4.publish(thruster4Data)
		pub6.publish(thruster6Data)


		r.sleep()
