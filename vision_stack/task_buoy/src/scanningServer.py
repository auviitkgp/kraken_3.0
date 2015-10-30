#! /usr/bin/env python

import roslib
PKG = 'task_buoy'
roslib.load_manifest(PKG)

import sys
import numpy as np
import time
from math import *

import actionlib
import std_msgs.msg
import rospy
from resources import topicHeader
# tools to get verbose
from resources import tools

from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import absoluteRPY

from kraken_msgs.msg import scanAction as actionMessagescanAction
from kraken_msgs.msg import scanFeedback
from kraken_msgs.msg import scanResult
from kraken_msgs.msg import scanGoal

# Global variables
base_yaw = 0.0
FIRST_ITERATION = True
Current_yaw = -1

class scanningServer(object):
    """ Creates messages that are used to publish feedback/result to client
        Messages :
          *  _feedback :
                1. Current value of yaw
                2. Desired_yaw
                3. Error
          *  _result  :
                1. elapsed_time
          *  YawFeedback :
                - Same as _feedback
          *  thruster4Data :
                - thrusterData4Thruster
          *  thruster6Data :
                - thrusterData6Thruster
        Publishers :
          *  pub_thrusters4 : publish as 4 thruster model on thruster converter
          *  pub_thrusters4 : publish as 6 thruster model on thruster converter
          *  pub_yawFeedback : Publish on 'YawFeedback' topic for plotting feedback
          *  sub_ABSrpy : Subscribe absolute rpy from absoluteRPY publisher

    """

    # Initializing the messages to be used
    _feedback = scanFeedback()
    _result = scanResult()
    thruster4Data=thrusterData4Thruster()
    thruster6Data=thrusterData6Thruster()

    def __init__(self, name):

        self._action_name = name
        self._as = actionlib.SimpleActionServer(
            self._action_name, actionMessagescanAction, execute_cb=self.execute_cb, auto_start=False)
        self._as.start()

        # Declare all Publisher and Subscribers here :
        self.pub_thrusters4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
        self.pub_thrusters6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)
        self.pub_yawFeedback = rospy.Publisher('YawFeedback',scanFeedback, queue_size = 2)
        self.sub_ABSrpy = rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, self.imuCB)

        rospy.loginfo('Server has started. Waiting for a goal.')


    def imuCB(self, dataIn):
        """
        This is imu call back function.

        1. Updates the Current value of yaw - Current_yaw
        2. Updates feedback to be sent to the client
        3. Update the data to be published on all the topics and publish contoller output and feedback to be plotted
        4. Debug messages
        """
        """
        global Current_yaw
        global base_yaw
        global FIRST_ITERATION

        Current_yaw = dataIn.yaw

        # Get the updated base_yaw, when a new goal is recieved
        if  FIRST_ITERATION:
    		base_yaw = Current_yaw
    		FIRST_ITERATION = False

            # 6 Thruster model 
            self.thruster6Data.data = [0, 0, 0, 0, -5, 5]
            self.thruster6Data.header  = std_msgs.msg.Header()
            self.thruster6Data.header.stamp = rospy.Time.now()

            # 4 Thruster model
            self.thruster4Data.data = [0, 0, -5, 5]
            self.thruster4Data.header  = std_msgs.msg.Header()
            self.thruster4Data.header.stamp = rospy.Time.now()

            # Debug messages
            rospy.logdebug("--------")
            rospy.logdebug("Current Yaw : %s",round(Current_yaw,3))
            rospy.logdebug("Thruster data L : %s",self.thruster6Data.data[4])
            rospy.logdebug("Thruster data R : %s",self.thruster6Data.data[5])
        """



    def execute_cb(self, goal):
        """
        1. Update the start flag
        2. Wait for first imu data to be recieved.
        3. sends constant thrust to the thruster converter.
        4. publishes feedback back to the client
        """
        _speed = goal.speed
        rospy.loginfo("I was asked to rotate at speed %s", _speed)

        """
        global FIRST_ITERATION
        global Current_yaw

        # Update the goal
        FIRST_ITERATION = True
        Current_yaw = -1

        r = rospy.Rate(20)

        # wait to receive starting value of yaw
        while(Current_yaw == -1):
            r.sleep()

        rospy.loginfo('I was asked to change yaw by %f', goal.yaw)
        initial_time = time.time()
        rospy.loginfo('Started setting yaw at %f', initial_time)
        success = True

        while abs(YAW.error) > 0.5 :
            # check that preempt has not been requested by the client
            if self._as.is_preempt_requested():
                rospy.loginfo('%s: Preempted' % self._action_name)
                self._as.set_preempted()
                success = False

                # Set the thrust values to 0, to stop the thrusters
                self.thruster6Data.data = [0,0,0,0,0,0]
                self.thruster4Data.data = [0,0,0,0]
                self.pub_thrusters6.publish(self.thruster6Data)
                self.pub_thrusters4.publish(self.thruster4Data)

                break

            # publish the feedback
            self._as.publish_feedback(self._feedback)

            # publish controller output values to thruster converter
            self.pub_thrusters4.publish(self.thruster4Data)
            self.pub_thrusters6.publish(self.thruster6Data)
            r.sleep()

        if success :
            self._result.elapsed_time = time.time() - initial_time
            rospy.loginfo('%s: Succeeded' % self._action_name)
            self._as.set_succeeded(self._result)

        """

if __name__ == '__main__':
	"""
     1. Calls the class searchBuoy constructor
	"""
	rospy.init_node('search_buoy_action_server', log_level=(rospy.DEBUG if tools.getVerboseTag(sys.argv) else rospy.INFO))
	scanningServer('scanning_server')
	rospy.spin()
