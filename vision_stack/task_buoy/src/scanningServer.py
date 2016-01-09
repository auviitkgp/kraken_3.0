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

from kraken_msgs.msg import scanAction as actionMessagescanAction
from kraken_msgs.msg import scanFeedback
from kraken_msgs.msg import scanResult
from kraken_msgs.msg import scanGoal

class scanningServer(object):
    """ Creates messages that are used to publish feedback/result to client
        Messages :
          *  _feedback :
          *  _result  :
                1. elapsed_time
          *  YawFeedback :
          *  thruster4Data :
                - thrusterData4Thruster
          *  thruster6Data :
                - thrusterData6Thruster
        Publishers :
          *  pub_thrusters4 : publish as 4 thruster model on thruster converter
          *  pub_thrusters4 : publish as 6 thruster model on thruster converter
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

        rospy.loginfo('Server has started.')


    def execute_cb(self, goal):
        """
        1. Update the rotation speed and direction
        2. sends constant thrust to the thruster converter as received.
        """

        # Update the Messages
        _speed = goal.speed
        _direction = goal.direction

        rospy.loginfo("I was asked to rotate at speed %s", _speed)

        if _direction==1:
            rospy.loginfo("I was asked to rotate in clockwise direction")
            self.thruster6Data.data = [0,0,0,0,_speed,-1*_speed]
            self.thruster4Data.data = [0,0,-1*_speed,_speed]

        else:
            rospy.loginfo("I was asked to rotate in counter clockwise direction")
            self.thruster6Data.data = [0,0,0,0,-1*_speed,_speed]
            self.thruster4Data.data = [0,0,_speed,-1*_speed]


        # rospy.loginfo("I was asked to rotate in" + " " if _direction == 1.0 else " counter-" + "clockwise direction")

        if rospy.has_param('/ros_rate'):
            r = float(rospy.get_param('/ros_rate'))
        else:
            r = 8

        initial_time = time.time()
        success = True

        while success :
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

if __name__ == '__main__':
	"""
     1. Calls the class searchBuoy constructor
	"""
	rospy.init_node('search_buoy_action_server', log_level=(rospy.DEBUG if tools.getVerboseTag(sys.argv) else rospy.INFO))
	scanningServer('scanning_server')
	rospy.spin()
