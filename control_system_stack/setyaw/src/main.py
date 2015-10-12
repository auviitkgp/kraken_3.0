#! /usr/bin/env python

import roslib
PKG = 'setyaw'
roslib.load_manifest(PKG)

import rospy
import actionlib
import sys
import numpy as np
import std_msgs.msg

import time

from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import absoluteRPY

from kraken_msgs.msg import setYawAction as actionMessageSetYawAction
from kraken_msgs.msg import setYawFeedback
from kraken_msgs.msg import setYawResult
from kraken_msgs.msg import setYawGoal

# from kraken_msgs.msg import controllerResult
from resources import topicHeader
from math import *
from all_controllers.fuzzy import Fuzzy

#global variables
base_yaw = 0.0
FIRST_ITERATION = True
prevError = 0.0
present_yaw = -1
client_goal = None
#-------------------------------------------------------------------------------
## Test case fuzzy subset for trimf
f_ssets = [[ # error
            [-60,-60,-30],   # -ve medium
            [-60,-30 , 0],    # -ve small
            [-30 , 0 , 30],   # zero
            [ 0 , 30 , 60],   # +ve small
            [ 30 ,60 , 60], # +ve medium
           ],
            # delta_error
           [
            [-60,-60,-30],   # -ve medium
            [-60,-30 , 0],    # -ve small
            [-30 , 0 , 30],   # zero
            [ 0 , 30 , 60],   # +ve small
            [ 30 ,60 , 60], # +ve medium
           ],
            # u
           [
            [-10,-10,-5],  # -ve medium
            [-10,-5 , 0],  # -ve small
            [-5 , 0 , 5],  # zero
            [ 0 , 5 , 10], # +ve small
            [ 5 ,10 , 10], # +ve medium
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

mf_types = ['trimf','trimf','trimf']
#-------------------------------------------------------------------------------

class SetYaw(object):
    # create messages that are used to publish feedback/result
    _feedback = setYawFeedback()
    _result = setYawResult()

    def __init__(self, name):
        self._action_name = name
        self._as = actionlib.SimpleActionServer(
            self._action_name, actionMessageSetYawAction, execute_cb=self.execute_cb, auto_start=False)
        self._as.start()
        rospy.loginfo('Server has started. Waiting for a goal.')


    def imuCB(self, dataIn):
        global present_yaw
        global base_yaw
        global prevError
        global client_goal
        global FIRST_ITERATION
        present_yaw = dataIn.yaw
        self._feedback.feedback_yaw = present_yaw
        if  FIRST_ITERATION:
    		base_yaw = present_yaw
    		FIRST_ITERATION = False
        prevError = YAW.error
        error = (base_yaw + client_goal.yaw - present_yaw)* 3.14 / 180
        YAW.error = np.arctan2(sin(error),cos(error))*180/3.14
        YAW.delta_error = YAW.error - prevError

        rospy.logdebug("--------")
        rospy.logdebug("Current Yaw : %s",round(present_yaw,3))
        rospy.logdebug("Error : %s",round(YAW.error,3))
        rospy.logdebug("Delta_error : %s",round(YAW.delta_error ,3))
        rospy.logdebug("Goal : %s",client_goal.yaw)



    def execute_cb(self, goal):

        global client_goal
        sub = rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, self.imuCB)

        r = rospy.Rate(20)
        client_goal = goal
        while(present_yaw == -1):
            r.sleep()

        rospy.loginfo('I was asked to set yaw to %f', goal.yaw)
        initial_time = time.time()
        rospy.loginfo('Started setting yaw at %f', initial_time)
        success = True
        rospy.loginfo('YAW.error : %s',abs(YAW.error) > 0.5)
        while abs(YAW.error) > 0.5 :
        	print abs(YAW.error)
            # check that preempt has not been requested by the client
            # if self._as.is_preempt_requested():
                # rospy.loginfo('%s: Preempted' % self._action_name)
                # self._as.set_preempted()
                # success = False
                # break
    		#publish thruster output to thruster
    		thruster6Data.data[0] = 0.0
    		thruster6Data.data[1] = 0.0
    		thruster6Data.data[2] = 0.0
    		thruster6Data.data[3] = 0.0
    		rospy.loginfo('thrust')

    		thrust = YAW.run()
    		rospy.loginfo('thrust : %s',thrust)
    		thruster6Data.data[4] =  thrust   # Left Thruster
    		thruster6Data.data[5] = -1 * thrust   # Rigt Thruster

    		thruster4Data.data[0] = thruster6Data.data[0]
    		thruster4Data.data[1] = thruster6Data.data[1]
    		thruster4Data.data[2] = thruster6Data.data[4]
    		thruster4Data.data[3] = thruster6Data.data[5]

    		rospy.loginfo("Thruster data L : %s",thruster6Data.data[4])
    		rospy.loginfo("Thruster data R : %s",thruster6Data.data[5])

    		pub_thrusters4.publish(thruster4Data)
    		pub_thrusters6.publish(thruster6Data)
            # publish the feedback
    		self._as.publish_feedback(self._feedback)
    		r.sleep()

        # Block Ends

        # TODO: When the yaw has been set for the vehicle, you need to send a
        # result back to the server. This is how you do it.

        self._result.elapsed_time = time.time()
        rospy.loginfo('%s: Succeeded' % self._action_name)
        self._as.set_succeeded(self._result)

if __name__ == '__main__':

	YAW = Fuzzy(mf_types, f_ssets)
	YAW.io_ranges = io_ranges

	thruster4Data=thrusterData4Thruster();
	thruster6Data=thrusterData6Thruster();

	pub_thrusters4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub_thrusters6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)


	rospy.init_node('setYaw')
	SetYaw(rospy.get_name())
	rospy.spin()
