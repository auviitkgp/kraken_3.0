#! /usr/bin/env python

import roslib
PKG = 'setyaw'
roslib.load_manifest(PKG)

import rospy
import actionlib

import time

from kraken_msgs.msg import setYawAction as actionMessageSetYawAction
from kraken_msgs.msg import setYawFeedback
from kraken_msgs.msg import setYawResult
from kraken_msgs.msg import setYawGoal

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

    def execute_cb(self, goal):

        # TODO: Fill out the fuzzy control stuff here.
        # You can add more functions, subscribers etc, as you please.

        rospy.loginfo('I was asked to set yaw to %f', goal.yaw)
        initial_time = time.time()
        rospy.loginfo('Started setting yaw at %f', initial_time)

        r = rospy.Rate(1)
        success = True

        i = 0
        ''' Code to test this package without modifying anything

        Run this package as is to ensure all dependencies are required.
        If it runs, you can comment out the block below and then start
        writing your own code.
        '''

        # Block Starts

        while i < 5:
            # check that preempt has not been requested by the client
            if self._as.is_preempt_requested():
                rospy.loginfo('%s: Preempted' % self._action_name)
                self._as.set_preempted()
                success = False
                break
            self._feedback.feedback_yaw = i
            i += 1
            # publish the feedback
            self._as.publish_feedback(self._feedback)
            # this step is not necessary, the sequence is computed at 1 Hz for
            # demonstration purposes
            r.sleep()

        # Block Ends

        # TODO: When the yaw has been set for the vehicle, you need to send a
        # result back to the server. This is how you do it.

        self._result.elapsed_time = time.time()
        rospy.loginfo('%s: Succeeded' % self._action_name)
        self._as.set_succeeded(self._result)

if __name__ == '__main__':
    rospy.init_node('setYaw')
    SetYaw(rospy.get_name())
    rospy.spin()
