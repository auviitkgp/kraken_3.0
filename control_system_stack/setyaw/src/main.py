#! /usr/bin/env python

import roslib
PKG = 'setyaw'
roslib.load_manifest(PKG)

import sys
import numpy as np
import time
from math import *

import actionlib
import std_msgs.msg
import rospy
from resources import topicHeader
from resources import tools

from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import absoluteRPY

from kraken_msgs.msg import setYawAction as actionMessageSetYawAction
from kraken_msgs.msg import setYawFeedback
from kraken_msgs.msg import setYawResult
from kraken_msgs.msg import setYawGoal

from all_controllers.fuzzy import Fuzzy
from all_controllers import fuzzyParams as Fparam

# Global variables
base_yaw = 0.0
FIRST_ITERATION = True
prevError = 0.0
Current_yaw = -1
Client_goal = None

class SetYaw(object):
    """ Creates messages that are used to publish feedback/result to client
        feedback messages :
            1. Current value of yaw
            2. Desired_yaw
            3. Error
        result   : elapsed_time
    """
    _feedback = setYawFeedback()
    _result = setYawResult()

    def __init__(self, name):
        self._action_name = name
        self._as = actionlib.SimpleActionServer(
            self._action_name, actionMessageSetYawAction, execute_cb=self.execute_cb, auto_start=False)
        self._as.start()
        rospy.loginfo('Server has started. Waiting for a goal.')


    def imuCB(self, dataIn):
        """
        This is imu call back function.

        1. Updates the Current value of yaw - Current_yaw
        2. Calculates error and delta_error based on 4 quadrant tangent = arctan2()
        3. Updates feedback to be sent to the client
            - Desired_yaw
            - Current_yaw
            - Error
        4. Debug messages
        """
        global Current_yaw
        global base_yaw
        global prevError
        global Client_goal
        global FIRST_ITERATION

        Current_yaw = dataIn.yaw

        if  FIRST_ITERATION :
    		base_yaw = Current_yaw
    		FIRST_ITERATION = False

        prevError = YAW.error
        error = (base_yaw + Client_goal.yaw - Current_yaw)* 3.14 / 180
        YAW.error = np.arctan2(sin(error),cos(error))*180/3.14
        YAW.delta_error = YAW.error - prevError

        # Update the feedbacks to be sent
        self._feedback.Desired_yaw = (Client_goal.yaw + base_yaw)%360
        self._feedback.Current_yaw = Current_yaw
        self._feedback.Error = YAW.error

        # calculate the thrust from fuzzy control
        thrust = YAW.run()

        thruster6Data.data[0] = 0.0
        thruster6Data.data[1] = 0.0
        thruster6Data.data[2] = 0.0
        thruster6Data.data[3] = 0.0
        thruster6Data.data[4] =  thrust       # Left Thruster
        thruster6Data.data[5] = -1 * thrust   # Rigt Thruster

        thruster4Data.data[0] = thruster6Data.data[0]
        thruster4Data.data[1] = thruster6Data.data[1]
        thruster4Data.data[2] = thruster6Data.data[4]
        thruster4Data.data[3] = thruster6Data.data[5]

        # publish thrust values to thruster converter
        pub_thrusters4.publish(thruster4Data)
        pub_thrusters6.publish(thruster6Data)

        # Debug messages
        rospy.logdebug("--------")
        rospy.logdebug("Current Yaw : %s",round(Current_yaw,3))
        rospy.logdebug("Desired_yaw : %s",round(self._feedback.Desired_yaw,3))
        rospy.logdebug("Error : %s",round(YAW.error,3))
        rospy.logdebug("Delta_error : %s",round(YAW.delta_error ,3))
        rospy.logdebug("Relative_Goal : %s",round(Client_goal.yaw,3))
        rospy.logdebug("Thruster data L : %s",thruster6Data.data[4])
        rospy.logdebug("Thruster data R : %s",thruster6Data.data[5])



    def execute_cb(self, goal):
        """
        1. Update the goal each time client sends a new goal
        2. Wait for first imu data to be recieved, to calculate the error.
        3. calculate the thrust from fuzzy control and send to the thruster converter.
        4. publishes feedback back to the client
            - Desired_yaw
            - Current_yaw
            - Error
        """

        global Client_goal
        global FIRST_ITERATION
        global Current_yaw

        # Update the goal
        Client_goal = goal
        FIRST_ITERATION = True
        Current_yaw = -1

        rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, self.imuCB)
        r = rospy.Rate(20)

        while(Current_yaw == -1):
            r.sleep()

        rospy.loginfo('I was asked to change yaw by %f', goal.yaw)
        initial_time = time.time()
        rospy.loginfo('Started setting yaw at %f', initial_time)
        success = True

        rospy.loginfo('YAW.error : %s',abs(YAW.error) > 0.5)
        while abs(YAW.error) > 0.5 :
            # check that preempt has not been requested by the client
            # if self._as.is_preempt_requested():
                # rospy.loginfo('%s: Preempted' % self._action_name)
                # self._as.set_preempted()
                # success = False
                # break

            # publish the feedback
            self._feedback.header = std_msgs.msg.Header()
            self._feedback.header.stamp = rospy.Time.now()
            self._as.publish_feedback(self._feedback)
            r.sleep()

        self._result.elapsed_time = time.time() - initial_time
        rospy.loginfo('%s: Succeeded' % self._action_name)
        self._as.set_succeeded(self._result)

if __name__ == '__main__':
	"""
	 1. Declare YAW as a Fuzzy object and Declare it's membership function and it's Range.
	 2. Declares messages types for thruster4Data and thruster6Data
	"""
	YAW = Fuzzy(Fparam.mf_types, Fparam.f_ssets)
	YAW.io_ranges = Fparam.io_ranges

	thruster4Data=thrusterData4Thruster();
	thruster6Data=thrusterData6Thruster();

	pub_thrusters4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
	pub_thrusters6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)


	rospy.init_node('setyaw-action-server', log_level=(rospy.DEBUG if tools.getVerboseTag(sys.argv) else rospy.INFO))
	SetYaw('setYaw')
	rospy.spin()
